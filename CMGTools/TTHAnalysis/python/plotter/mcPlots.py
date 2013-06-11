#!/usr/bin/env python
#from mcAnalysis import *
from CMGTools.TTHAnalysis.plotter.mcAnalysis import *

class PlotFile:
    def __init__(self,fileName,options):
        self._options = options
        self._plots = []
        infile = open(fileName,'r')
        for line in infile:
            if re.match("\s*#.*", line) or len(line.strip())==0: continue
            while line.strip()[-1] == "\\":
                line = line.strip()[:-1] + infile.next()
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
            if len(options.plotselect):
                skipMe = True
                for p0 in options.plotselect:
                    for p in p0.split(","):
                        if re.match(p+"$", field[0]): skipMe = False
                if skipMe: continue
            if options.globalRebin: extra['rebinFactor'] = options.globalRebin
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

def doTinyCmsPrelim(textLeft="_default_",textRight="_default_",hasExpo=False,textSize=0.033):
    global options
    if textLeft  == "_default_": textLeft  = options.lspam
    if textRight == "_default_": textRight = options.rspam
    if textLeft not in ['', None]:
        doSpam(textLeft, .28 if hasExpo else .17, .955, .40, .995, align=12, textSize=textSize)
    if textRight not in ['', None]:
        if "%(lumi)" in textRight: 
            textRight = textRight % { 'lumi':options.lumi }
        doSpam(textRight,.48, .955, .99, .995, align=32, textSize=textSize)

def reMax(hist,hist2,islog,factorLin=1.3,factorLog=2.0):
    if  hist.ClassName() == 'THStack':
        hist = hist.GetHistogram()
    max0 = hist.GetMaximum()
    max2 = hist2.GetMaximum()*(factorLog if islog else factorLin)
    if max2 > max0:
        max0 = max2;
        if islog: hist.GetYaxis().SetRangeUser(0.9,max0)
        else:     hist.GetYaxis().SetRangeUser(0,max0)
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
    if options.noStackSig:
        total = sum([v.Integral() for k,v in pmap.iteritems() if not hasattr(v,'summary') and mca.isBackground(k) ])
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

def doStackSigScaledNormData(pspec,pmap):
    if "data"       not in pmap: return (None,-1.0)
    if "signal"     not in pmap: return (None,-1.0)
    data = pmap["data"]
    sig = pmap["signal"].Clone("sig_refloat")
    bkg = None
    if "background" in pmap:
        bkg = pmap["background"]
    else:
        bkg = sig.Clone(); bkg.Reset()
    sf = (data.Integral()-bkg.Integral())/sig.Integral()
    sig.Scale(sf)
    sig.Add(bkg)
    sig.SetFillStyle(0)
    sig.SetLineColor(206)
    sig.SetLineWidth(3)
    sig.SetLineStyle(2)
    sig.Draw("HIST SAME")
    return (sig,sf)

def doScaleSigNormData(pspec,pmap,mca):
    if "data"       not in pmap: return -1.0
    if "signal"     not in pmap: return -1.0
    data = pmap["data"]
    sig = pmap["signal"].Clone("sig_refloat")
    bkg = None
    if "background" in pmap:
        bkg = pmap["background"]
    else:
        bkg = sig.Clone(); bkg.Reset()
    sf = (data.Integral()-bkg.Integral())/sig.Integral()
    signals = [ "signal" ] + mca.listSignals()
    for p,h in pmap.iteritems():
        if p in signals: h.Scale(sf)
    return sf


def doRatioHists(pspec,pmap,total,totalSyst,maxRange):
    if "data" not in pmap: return (None,None,None,None)
    ratio = pmap["data"].Clone("data_div"); 
    ratio.Divide(total)
    unity  = totalSyst.Clone("sim_div");
    unity0 = total.Clone("sim_div");
    rmin, rmax =  1,1
    for b in xrange(1,unity.GetNbinsX()+1):
        e,e0,n = unity.GetBinError(b), unity0.GetBinError(b), unity.GetBinContent(b)
        unity.SetBinContent(b, 1 if n > 0 else 0)
        unity.SetBinError(b, e/n if n > 0 else 0)
        unity0.SetBinContent(b,  1 if n > 0 else 0)
        unity0.SetBinError(b, e0/n if n > 0 else 0)
        if ratio.GetBinContent(b) != 0:
            rmin = min([ rmin, 1-2*e/n if n > 0 else 1, ratio.GetBinContent(b) - 2*ratio.GetBinError(b) ]) 
            rmax = max([ rmax, 1+2*e/n if n > 0 else 1, ratio.GetBinContent(b) + 2*ratio.GetBinError(b) ])  
    if rmin < maxRange[0]: rmin = maxRange[0]; 
    if rmax > maxRange[1]: rmax = maxRange[1];
    unity.SetFillStyle(1001);
    unity.SetFillColor(ROOT.kCyan);
    unity.SetMarkerStyle(1);
    unity.SetMarkerColor(ROOT.kCyan);
    unity0.SetFillStyle(1001);
    unity0.SetFillColor(53);
    unity0.SetMarkerStyle(1);
    unity0.SetMarkerColor(53);
    ROOT.gStyle.SetErrorX(0.5);
    unity.Draw("E2");
    unity0.Draw("E2 SAME");
    unity.GetYaxis().SetRangeUser(rmin,rmax);
    unity.GetXaxis().SetTitleSize(0.14)
    unity.GetYaxis().SetTitleSize(0.14)
    unity.GetXaxis().SetLabelSize(0.11)
    unity.GetYaxis().SetLabelSize(0.11)
    unity.GetYaxis().SetNdivisions(505)
    unity.GetYaxis().SetDecimals(True)
    unity.GetYaxis().SetTitle("Data/Sim.")
    unity.GetYaxis().SetTitleOffset(0.52);
    total.GetXaxis().SetLabelOffset(999) ## send them away
    total.GetXaxis().SetTitleOffset(999) ## in outer space
    total.GetYaxis().SetLabelSize(0.05)
    #ratio.SetMarkerSize(0.7*ratio.GetMarkerSize()) # no it is confusing
    #$ROOT.gStyle.SetErrorX(0.0);
    line = ROOT.TLine(unity.GetXaxis().GetXmin(),1,unity.GetXaxis().GetXmax(),1)
    line.SetLineWidth(2);
    line.SetLineColor(58);
    line.Draw("L")
    ratio.Draw("E SAME");
    return (ratio, unity, unity0, line)

def doStatTests(total,data,test,legendCorner):
    #print "Stat tests for %s:" % total.GetName()
    ksprob = data.KolmogorovTest(total,"XN")
    #print "\tKS  %.4f" % ksprob
    chi2l, chi2p, nb = 0, 0, 0
    for b in xrange(1,data.GetNbinsX()+1):
        oi = data.GetBinContent(b)
        ei = total.GetBinContent(b)
        if ei <= 0: continue
        nb += 1
        chi2l += - 2*(oi*log(ei/oi)+(oi-ei) if oi > 0 else -ei)
        chi2p += (oi-ei)**2 / ei
    #print "\tc2p %.4f (%6.2f/%3d)" % (ROOT.TMath.Prob(chi2p, nb), chi2p, nb)
    #print "\tc2l %.4f (%6.2f/%3d)" % (ROOT.TMath.Prob(chi2l, nb), chi2l, nb)
    if test == "chi2l" or test == "chi2p":
        chi2 = chi2l if test == "chi2l" else chi2p
        pval = ROOT.TMath.Prob(chi2, nb)
        text = "#chi^{2} p-value %.3f" % pval if pval < 0.02 else "#chi^{2} p-value %.2f" % pval
    else:
        text = "Unknown test %s" % test
    if legendCorner == "TR":
        doSpam(text, .30, .85, .48, .93, align=32, textSize=0.05)
    elif legendCorner == "TL":
        doSpam(text, .75, .85, .93, .93, align=32, textSize=0.05)



legend_ = None;
def doLegend(pmap,mca,corner="TR",textSize=0.035,cutoff=1e-2):
        if (corner == None): return
        total = sum([x.Integral() for x in pmap.itervalues()])
        sigEntries = []; bgEntries = []
        for p in mca.listSignals():
            if p in pmap and pmap[p].Integral() >= cutoff*total: 
                lbl = mca.getProcessOption(p,'Label',p)
                sigEntries.append( (pmap[p],lbl,'F') )
        for p in mca.listBackgrounds():
            if p in pmap and pmap[p].Integral() >= cutoff*total: 
                lbl = mca.getProcessOption(p,'Label',p)
                bgEntries.append( (pmap[p],lbl,'F') )
        nentries = len(sigEntries) + len(bgEntries) + ('data' in pmap)

        (x1,y1,x2,y2) = (.7, .75 - textSize*max(nentries-3,0), .93, .93)
        if corner == "TR":
            (x1,y1,x2,y2) = (.75, .75 - textSize*max(nentries-3,0), .93, .93)
        elif corner == "TL":
            (x1,y1,x2,y2) = (.2, .75 - textSize*max(nentries-3,0), .38, .93)
        
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
        ROOT.gROOT.ProcessLine(".L smearer.cc+")
        ROOT.gStyle.SetOptStat(0)
        ROOT.gStyle.SetOptTitle(0)
    def run(self,mca,cuts,plots,makeStack=True,makeCanvas=True):
        sets = [ (None, 'all cuts', cuts.allCuts()) ]
        if not self._options.final:
            allcuts = cuts.sequentialCuts()
            if self._options.nMinusOne: allcuts = cuts.nMinusOneCuts()
            for i,(cn,cv) in enumerate(allcuts[:-1]): # skip the last one which is equal to all cuts
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
                #
                # blinding policy
                blind = pspec.getOption('Blinded','None') if 'data' in pmap else 'None'
                xblind = [9e99,-9e99]
                if re.match(r'(bin|x)\s*([<>]?)\s*(\+|-)?\d+(\.\d+)?|(\+|-)?\d+(\.\d+)?\s*<\s*(bin|x)\s*<\s*(\+|-)?\d+(\.\d+)?', blind):
                    xfunc = (lambda h,b: b)             if 'bin' in blind else (lambda h,b : h.GetXaxis().GetBinCenter(b));
                    test  = eval("lambda bin : "+blind) if 'bin' in blind else eval("lambda x : "+blind) 
                    hdata = pmap['data']
                    for b in xrange(1,hdata.GetNbinsX()+1):
                        if test(xfunc(hdata,b)):
                            #print "blinding bin %d, x = [%s, %s]" % (b, hdata.GetXaxis().GetBinLowEdge(b), hdata.GetXaxis().GetBinUpEdge(b))
                            hdata.SetBinContent(b,0)
                            hdata.SetBinError(b,0)
                            xblind[0] = min(xblind[0],hdata.GetXaxis().GetBinLowEdge(b))
                            xblind[1] = max(xblind[1],hdata.GetXaxis().GetBinUpEdge(b))
                    #print "final blinded range x = [%s, %s]" % (xblind[0],xblind[1])
                elif blind != "None":
                    raise RuntimeError, "Unrecongnized value for 'Blinded' option, stopping here"
                #
                for k,v in pmap.iteritems():
                    v.SetDirectory(dir)
                    dir.WriteTObject(v)
                #
                if not makeStack: continue
                stack = ROOT.THStack(pspec.name+"_stack",pspec.name)
                hists = [v for k,v in pmap.iteritems() if k != 'data']
                total = hists[0].Clone(pspec.name+"_total"); total.Reset()
                totalSyst = hists[0].Clone(pspec.name+"_totalSyst"); totalSyst.Reset()
                if options.scaleSignalToData: doScaleSigNormData(pspec,pmap,mca)
                for p in mca.listBackgrounds() + mca.listSignals():
                    if p in pmap: 
                        plot = pmap[p]
                        if plot.Integral() <= 0: continue
                        if mca.isSignal(p) and options.noStackSig == True: continue 
                        if self._options.plotmode == "stack":
                            stack.Add(plot)
                            total.Add(plot)
                            totalSyst.Add(plot)
                            if mca.getProcessOption(p,'NormSystematic',0.0) > 0:
                                syst = mca.getProcessOption(p,'NormSystematic',0.0)
                                if "TH1" in plot.ClassName():
                                    for b in xrange(1,plot.GetNbinsX()+1):
                                        totalSyst.SetBinError(b, hypot(totalSyst.GetBinError(b), syst*plot.GetBinContent(b)))
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
                doRatio = self._options.showRatio and 'data' in pmap
                islog = pspec.hasOption('Logy'); 
                # define aspect ratio
                if doRatio: ROOT.gStyle.SetPaperSize(20.,25.)
                else:       ROOT.gStyle.SetPaperSize(20.,20.)
                # create canvas
                c1 = ROOT.TCanvas(pspec.name+"_canvas", pspec.name, 600, (750 if doRatio else 600))
                c1.Draw()
                p1, p2 = c1, None # high and low panes
                # set borders, if necessary create subpads
                if doRatio: 
                    c1.SetWindowSize(600 + (600 - c1.GetWw()), (750 + (750 - c1.GetWh())));
                    p1 = ROOT.TPad("pad1","pad1",0,0.31,1,1);
                    p1.SetBottomMargin(0);
                    p1.Draw();
                    p2 = ROOT.TPad("pad2","pad2",0,0,1,0.31);
                    p2.SetTopMargin(0);
                    p2.SetBottomMargin(0.3);
                    p2.SetFillStyle(0);
                    p2.Draw();
                    p1.cd();
                else:
                    c1.SetWindowSize(600 + (600 - c1.GetWw()), 600 + (600 - c1.GetWh()));
                p1.SetLogy(islog)
                if pspec.hasOption('Logx'):
                    p1.SetLogx(True)
                    if p2: p2.SetLogx(True)
                    total.GetXaxis().SetNoExponent(True)
                    total.GetXaxis().SetMoreLogLabels(True)
                if islog: total.SetMaximum(2*total.GetMaximum())
                if not islog: total.SetMinimum(0)
                total.Draw("HIST")
                if self._options.plotmode == "stack":
                    stack.Draw("SAME HIST")
                    total.Draw("AXIS SAME")
                else: 
                    stack.Draw("SAME HIST NOSTACK")
                if pspec.getOption('MoreY',1.0) > 1.0:
                    total.SetMaximum(pspec.getOption('MoreY',1.0)*total.GetMaximum())
                if 'data' in pmap: 
                    reMax(total,pmap['data'],islog)
                    pmap['data'].Draw("E SAME")
                    if xblind[0] < xblind[1]:
                        blindbox = ROOT.TBox(xblind[0],total.GetYaxis().GetXmin(),xblind[1],total.GetMaximum())
                        blindbox.SetFillColor(ROOT.kBlue+3)
                        blindbox.SetFillStyle(3944)
                        blindbox.Draw()
                        xblind.append(blindbox) # so it doesn't get deleted
                    if options.doStatTests:
                        doStatTests(total,pmap['data'], options.doStatTests, legendCorner=pspec.getOption('Legend','TR'))
                if pspec.hasOption('YMin') and pspec.hasOption('YMax'):
                    total.GetYaxis().SetRangeUser(pspec.getOption('YMin',1.0), pspec.getOption('YMax',1.0))
                doLegend(pmap,mca,corner=pspec.getOption('Legend','TR'),
                                  cutoff=pspec.getOption('LegendCutoff', 1e-5 if c1.GetLogy() else 1e-2),
                                  textSize=(0.039 if doRatio else 0.035))
                doTinyCmsPrelim(hasExpo = total.GetMaximum() > 9e4 and not c1.GetLogy(),textSize=(0.036 if doRatio else 0.033))
                signorm = None; datnorm = None; sfitnorm = None
                if options.showSigShape: 
                    signorm = doStackSignalNorm(pspec,pmap)
                    if signorm != None:
                        signorm.SetDirectory(dir); dir.WriteTObject(signorm)
                        reMax(total,signorm,islog)
                if options.showDatShape: 
                    datnorm = doDataNorm(pspec,pmap)
                    if datnorm != None:
                        datnorm.SetDirectory(dir); dir.WriteTObject(datnorm)
                        reMax(total,datnorm,islog)
                if options.showSFitShape: 
                    (sfitnorm,sf) = doStackSigScaledNormData(pspec,pmap)
                    if sfitnorm != None:
                        sfitnorm.SetDirectory(dir); dir.WriteTObject(sfitnorm)
                        reMax(total,sfitnorm,islog)
                if makeCanvas: dir.WriteTObject(c1)
                rdata,rnorm,rnorm2,rline = (None,None,None,None)
                if doRatio:
                    p2.cd(); 
                    rdata,rnorm,rnorm2,rline = doRatioHists(pspec,pmap,total,totalSyst, maxRange=options.maxRatioRange)
                if self._options.printPlots:
                    for ext in self._options.printPlots.split(","):
                        fdir = self._options.printDir;
                        if subname: fdir += "/"+subname;
                        if not os.path.exists(fdir): 
                            os.makedirs(fdir); 
                            if os.path.exists("/afs/cern.ch"): os.system("cp /afs/cern.ch/user/g/gpetrucc/php/index.php "+fdir)
                        if ext == "txt":
                            dump = open("%s/%s.%s" % (fdir, pspec.name, ext), "w")
                            maxlen = max([len(mca.getProcessOption(p,'Label',p)) for p in mca.listSignals() + mca.listBackgrounds()]+[7])
                            fmt    = "%%-%ds %%9.2f +/- %%9.2f (stat)" % (maxlen+1)
                            for p in mca.listSignals() + mca.listBackgrounds() + ["signal", "background"]:
                                if p not in pmap: continue
                                plot = pmap[p]
                                if plot.Integral() <= 0: continue
                                norm = plot.Integral()
                                stat = sqrt(sum([plot.GetBinError(b)**2 for b in xrange(1,plot.GetNbinsX()+1)]))
                                syst = norm * mca.getProcessOption(p,'NormSystematic',0.0) if p not in ["signal", "background"] else 0;
                                if p == "signal": dump.write(("-"*(maxlen+45))+"\n");
                                dump.write(fmt % (mca.getProcessOption(p,'Label',p) if p not in ["signal", "background"] else p.upper(), norm, stat))
                                if syst: dump.write(" +/- %9.2f (syst)"  % syst)
                                dump.write("\n")
                            if 'data' in pmap: 
                                dump.write(("-"*(maxlen+45))+"\n");
                                dump.write(("%%%ds %%7.0f\n" % (maxlen+1)) % ('DATA', pmap['data'].Integral()))
                            dump.close()
                        else:
                            c1.Print("%s/%s.%s" % (fdir, pspec.name, ext))
                c1.Close()
def addPlotMakerOptions(parser):
    addMCAnalysisOptions(parser)
    parser.add_option("--ss",  "--scale-signal", dest="signalPlotScale", default=1.0, help="scale the signal in the plots by this amount");
    #parser.add_option("--lspam", dest="lspam",   type="string", default="CMS Simulation", help="Spam text on the right hand side");
    parser.add_option("--lspam", dest="lspam",   type="string", default="CMS Preliminary", help="Spam text on the right hand side");
    parser.add_option("--rspam", dest="rspam",   type="string", default="#sqrt{s} = 8 TeV, L = %(lumi).1f fb^{-1}", help="Spam text on the right hand side");
    parser.add_option("--print", dest="printPlots", type="string", default="png,pdf,txt", help="print out plots in this format or formats (e.g. 'png,pdf,txt')");
    parser.add_option("--pdir", "--print-dir", dest="printDir", type="string", default="plots", help="print out plots in this directory");
    parser.add_option("--showSigShape", dest="showSigShape", action="store_true", default=False, help="Stack a normalized signal shape")
    parser.add_option("--noStackSig", dest="noStackSig", action="store_true", default=False, help="Don't add the signal shape to the stack (useful with --showSigShape)")
    parser.add_option("--showDatShape", dest="showDatShape", action="store_true", default=False, help="Stack a normalized data shape")
    parser.add_option("--showSFitShape", dest="showSFitShape", action="store_true", default=False, help="Stack a shape of background + scaled signal normalized to total data")
    parser.add_option("--showRatio", dest="showRatio", action="store_true", default=False, help="Add a data/sim ratio plot at the bottom")
    parser.add_option("--scaleSigToData", dest="scaleSignalToData", action="store_true", default=False, help="Scale all signal processes so that the overall event yield matches the observed one")
    parser.add_option("--maxRatioRange", dest="maxRatioRange", type="float", nargs=2, default=(0.0, 5.0), help="Min and max for the ratio")
    parser.add_option("--doStatTests", dest="doStatTests", type="string", default=None, help="Do this stat test: chi2p (Pearson chi2), chi2l (binned likelihood equivalent of chi2)")
    parser.add_option("--plotmode", dest="plotmode", type="string", default="stack", help="Show as stacked plot (stack), a non-stacked comparison (nostack) and a non-stacked comparison of normalized shapes (norm)")
    parser.add_option("--rebin", dest="globalRebin", type="int", default="0", help="Rebin all plots by this factor")
    parser.add_option("--select-plot", "--sP", dest="plotselect", action="append", default=[], help="Select only these plots out of the full file")

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
    if (not options.out) and options.printDir:
        outname = options.printDir + "/"+os.path.basename(args[2].replace(".txt","")+".root")
    if os.path.dirname(outname) and not os.path.exists(os.path.dirname(outname)):
        os.system("mkdir -p "+os.path.dirname(outname))
        if os.path.exists("/afs/cern.ch"): os.system("cp /afs/cern.ch/user/g/gpetrucc/php/index.php "+os.path.dirname(outname))
    print "Will save plots to ",outname
    outfile  = ROOT.TFile(outname,"RECREATE")
    plotter = PlotMaker(outfile)
    plotter.run(mca,cuts,plots)
    outfile.Close()


