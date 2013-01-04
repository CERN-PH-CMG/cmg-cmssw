#!/usr/bin/env python
#from mcAnalysis import *
from CMGTools.TTHAnalysis.plotter.mcAnalysis import *

class PlotFile:
    def __init__(self,fileName,options):
        self._options = options
        self._plots = []
        for line in open(fileName,'r'):
            if re.match("\s*#.*", line): continue
            extra = {}
            if ";" in line:
                (line,more) = line.split(";")[:2]
                more = more.replace("\\,",";")
                for setting in [f.strip().replace(";",",") for f in more.split(',')]:
                    if "=" in setting: 
                        (key,val) = [f.strip() for f in setting.split("=")]
                        extra[key] = eval(val)
                    else: extra[setting] = True
            line = re.sub("#.*","",line) 
            field = [f.strip().replace(";",":") for f in line.replace("\\:",";").split(':')]
            if len(field) <= 2: continue
            self._plots.append(PlotSpec(field[0],field[1],field[2],extra))
    def plots(self):
        return self._plots[:]

def doSpam(text,x1,y1,x2,y2,align=12,fill=False,textSize=0.033,_noDelete={}):
    cmsprel = ROOT.TPaveText(x1,y1,x2,y2,"NDC");
    cmsprel.SetTextSize(textSize);
    cmsprel.SetFillColor(0);
    cmsprel.SetFillStyle(1001 if fill else 0);
    cmsprel.SetLineStyle(2);
    cmsprel.SetLineColor(0);
    cmsprel.SetTextAlign(align);
    cmsprel.SetTextFont(42);
    cmsprel.AddText(text);
    cmsprel.Draw("same");
    _noDelete[text] = cmsprel; ## so it doesn't get deleted by PyROOT
    return cmsprel

def doTinyCmsPrelim(textLeft="_default_",textRight="_default_"):
    global options
    if textLeft  == "_default_": textLeft  = options.lspam
    if textRight == "_default_": textRight = options.rspam
    if textLeft not in ['', None]:
        doSpam(textLeft, .15, .955, .40, .995, align=12)
    if textRight not in ['', None]:
        if "%(lumi)" in textRight: 
            textRight = textRight % { 'lumi':options.lumi }
        doSpam(textRight,.48, .955, .99, .995, align=32)

def reMax(hist,hist2,factor=1.3):
    if  hist.ClassName() == 'THStack':
        hist = hist.GetHistogram()
    max0 = hist.GetMaximum()
    max2 = hist2.GetMaximum()*factor
    if max2 > max0:
        max0 = max2;
        hist.GetYaxis().SetRangeUser(0,max0)
def doDataNorm(pspec,pmap):
    if "data" not in pmap: return None
    total = sum([v.Integral() for k,v in pmap.iteritems() if k != 'data' and not hasattr(v,'summary')])
    sig = pmap["data"].Clone(pspec.name+"_data_norm")
    sig.SetFillStyle(0)
    sig.SetLineColor(1)
    sig.SetLineWidth(3)
    sig.SetLineStyle(2)
    if sig.Integral() > 0:
        sig.Scale(total/sig.Integral())
    sig.Draw("HIST SAME")
    return sig

def doStackSignalNorm(pspec,pmap):
    #total = 0
    #if "background" in pmap: total = pmap["background"].Integral()
    #else: total = sum([pmap[x].Integral() for x in mca.listBackgrounds() if pmap.has_key(x) ])
    total = sum([v.Integral() for k,v in pmap.iteritems() if k != 'data' and not hasattr(v,'summary')])
    sig = None
    if "signal" in pmap: sig = pmap["signal"].Clone(pspec.name+"_signal_norm")
    else: 
        sigs = [pmap[x] for x in mca.listBackgrounds() if pmap.has_key(x) and pmap[x].Integral() > 0]
        sig = sigs[0].Clone(sigs.GetName()+"_norm")
    sig.SetFillStyle(0)
    sig.SetLineColor(206)
    sig.SetLineWidth(4)
    if sig.Integral() > 0:
        sig.Scale(total/sig.Integral())
    sig.Draw("HIST SAME")
    return sig


legend_ = None;
def doLegend(pmap,mca,corner="TR",textSize=0.035,cutoff=1e-2):
        if (corner == None): return
        total = sum([x.Integral() for x in pmap.itervalues()])
        sigEntries = []; bgEntries = []
        for p in mca.listSignals():
            if p in pmap and pmap[p].Integral() >= cutoff*total: 
                sigEntries.append( (pmap[p],p,'F') )
        for p in mca.listBackgrounds():
            if p in pmap and pmap[p].Integral() >= cutoff*total: 
                bgEntries.append( (pmap[p],p,'F') )
        nentries = len(sigEntries) + len(bgEntries) + ('data' in pmap)

        (x1,y1,x2,y2) = (.7, .75 - textSize*max(nentries-3,0), .93, .93)
        if corner == "TR":
            (x1,y1,x2,y2) = (.7, .75 - textSize*max(nentries-3,0), .93, .93)
        elif corner == "TL":
            (x1,y1,x2,y2) = (.2, .75 - textSize*max(nentries-3,0), .43, .93)
        
        leg = ROOT.TLegend(x1,y1,x2,y2)
        leg.SetFillColor(0)
        leg.SetShadowColor(0)
        leg.SetTextFont(42)
        leg.SetTextSize(textSize)
        if 'data' in pmap: 
            leg.AddEntry(pmap['data'], 'Data', 'LP')
        total = sum([x.Integral() for x in pmap.itervalues()])
        for (plot,label,style) in sigEntries: leg.AddEntry(plot,label,style)
        for (plot,label,style) in  bgEntries: leg.AddEntry(plot,label,style)
        leg.Draw()
        ## assign it to a global variable so it's not deleted
        global legend_;
        legend_ = leg 
        return leg

class PlotMaker:
    def __init__(self,tdir):
        self._options = options
        self._dir = tdir
        ROOT.gROOT.ProcessLine(".x tdrstyle.cc")
        ROOT.gStyle.SetOptStat(0)
        ROOT.gStyle.SetOptTitle(0)
    def run(self,mca,cuts,plots,makeStack=True,makeCanvas=True):
        sets = [ (None, 'all cuts', cuts.allCuts()) ]
        if not self._options.final:
            allcuts = cuts.sequentialCuts()
            if self._options.nMinusOne: allcuts = cuts.nMinusOneCuts()
            for i,(cn,cv) in enumerate(allcuts):
                cnsafe = "cut_%02d_%s" % (i, re.sub("[^a-zA-Z0-9_.]","",cn.replace(" ","_")))
                sets.append((cnsafe,cn,cv))
        for subname, title, cut in sets:
            print "cut set: ",title
            dir = self._dir
            if subname:
                if self._dir.Get(subname):
                    dir = self._dir.Get(subname)
                else:
                    dir = self._dir.mkdir(subname,title)
            dir.cd()
            for pspec in plots.plots():
                print "    plot: ",pspec.name
                pmap = mca.getPlots(pspec,cut,makeSummary=True)
                for k,v in pmap.iteritems():
                    v.SetDirectory(dir)
                    dir.WriteTObject(v)
                #
                if not makeStack: continue
                stack = ROOT.THStack(pspec.name+"_stack",pspec.name)
                hists = [v for k,v in pmap.iteritems() if k != 'data']
                total = hists[0].Clone(pspec.name+"_total"); total.Reset()
                for p in mca.listBackgrounds() + mca.listSignals():
                    if p in pmap: 
                        plot = pmap[p]
                        if plot.Integral() <= 0: continue
                        if self._options.plotmode == "stack":
                            stack.Add(plot)
                            total.Add(plot)
                        else:
                            plot.SetLineColor(plot.GetFillColor())
                            plot.SetLineWidth(3)
                            plot.SetFillStyle(0)
                            if self._options.plotmode == "norm":
                                plot.Scale(1.0/plot.Integral())
                            stack.Add(plot)
                            total.SetMaximum(max(total.GetMaximum(),1.3*plot.GetMaximum()))
                        plot.SetMarkerStyle(0)
                stack.Draw("GOFF")
                stack.GetYaxis().SetTitle(pspec.getOption('YTitle',"Events"))
                stack.GetXaxis().SetTitle(pspec.getOption('XTitle',pspec.name))
                stack.GetXaxis().SetNdivisions(pspec.getOption('XNDiv',510))
                dir.WriteTObject(stack)
                # 
                if not makeCanvas and not self._options.printPlots: continue
                c1 = ROOT.TCanvas(pspec.name+"_canvas",pspec.name)
                if pspec.hasOption('Logy'):
                    c1.SetLogy(1)
                    total.SetMaximum(2*total.GetMaximum())
                total.Draw("HIST")
                stack.Draw("SAME HIST" if self._options.plotmode == "stack" else "SAME HIST NOSTACK")
                #stack.Draw("HF SAME")
                if 'data' in pmap: 
                    reMax(total,pmap['data'])
                    pmap['data'].Draw("E SAME")
                doLegend(pmap,mca,corner=pspec.getOption('Legend','TR'))
                doTinyCmsPrelim()
                signorm = None; datnorm = None
                if options.showSigShape: 
                    signorm = doStackSignalNorm(pspec,pmap)
                    signorm.SetDirectory(dir); dir.WriteTObject(signorm)
                    reMax(total,signorm)
                if options.showDatShape: 
                    datnorm = doDataNorm(pspec,pmap)
                    datnorm.SetDirectory(dir); dir.WriteTObject(datnorm)
                    reMax(total,datnorm)
                if makeCanvas: dir.WriteTObject(c1)
                #
                if self._options.printPlots:
                    for ext in self._options.printPlots.split(","):
                        fdir = self._options.printDir;
                        if subname: fdir += "/"+subname;
                        if not os.path.exists(fdir): 
                            os.makedirs(fdir); 
                            if os.path.exists("/afs/cern.ch"): os.system("cp /afs/cern.ch/user/g/gpetrucc/php/index.php "+fdir)
                        c1.Print("%s/%s.%s" % (fdir, pspec.name, ext))
                c1.Close()
def addPlotMakerOptions(parser):
    addMCAnalysisOptions(parser)
    parser.add_option("--ss",  "--scale-signal", dest="signalPlotScale", default=1.0, help="scale the signal in the plots by this amount");
    parser.add_option("--lspam", dest="lspam",   type="string", default="CMS Simulation", help="Spam text on the right hand side");
    parser.add_option("--rspam", dest="rspam",   type="string", default="#sqrt{s} = 8 TeV, L = %(lumi).1f fb^{-1}", help="Spam text on the right hand side");
    parser.add_option("--print", dest="printPlots", type="string", default=None, help="print out plots in this format or formats (e.g. 'png,pdf')");
    parser.add_option("--pdir", "--print-dir", dest="printDir", type="string", default="plots", help="print out plots in this directory");
    parser.add_option("--showSigShape", dest="showSigShape", action="store_true", default=False, help="Stack a normalized signal shape")
    parser.add_option("--showDatShape", dest="showDatShape", action="store_true", default=False, help="Stack a normalized data shape")
    parser.add_option("--plotmode", dest="plotmode", type="string", default="stack", help="Show as stacked plot (stack), a non-stacked comparison (nostack) and a non-stacked comparison of normalized shapes (norm)")

if __name__ == "__main__":
    from optparse import OptionParser
    parser = OptionParser(usage="%prog [options] mc.txt cuts.txt plots.txt")
    addPlotMakerOptions(parser)
    parser.add_option("-o", "--out", dest="out", default=None, help="Output file name. by default equal to plots -'.txt' +'.root'");
    (options, args) = parser.parse_args()
    mca  = MCAnalysis(args[0],options)
    cuts = CutsFile(args[1],options)
    plots = PlotFile(args[2],options)
    outname  = options.out if options.out else (args[2].replace(".txt","")+".root")
    outfile  = ROOT.TFile(outname,"RECREATE")
    plotter = PlotMaker(outfile)
    plotter.run(mca,cuts,plots)
    outfile.Close()


