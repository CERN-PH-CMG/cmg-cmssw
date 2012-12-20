#!/usr/bin/env python
from CMGTools.TTHAnalysis.plotter.mcAnalysis import *

class PlotFile:
    def __init__(self,fileName,options):
        self._options = options
        self._plots = []
        for line in open(fileName,'r'):
            if re.match("\s*#.*", line): continue
            line = re.sub("#.*","",line) 
            extra = {}
            if ";" in line:
                (line,more) = line.split(";")[:2]
                for setting in [f.strip() for f in more.split(',')]:
                    if "=" in setting: 
                        (key,val) = [f.strip() for f in setting.split("=")]
                        extra[key] = eval(val)
                    else: extra[setting] = True
            field = [f.strip() for f in line.split(':')]
            if len(field) <= 2: continue
            self._plots.append((field[0],field[1],field[2],extra))
    def plots(self):
        return self._plots[:]

class Legend:
    def __init__(self,pmap,mca,x1=.7,y1=.7,x2=.93,y2=.93,textSize=0.035,cutoff=1e-2):
        self._leg = ROOT.TLegend(x1,y1,x2,y2)
        self._leg.SetFillColor(0)
        self._leg.SetShadowColor(0)
        self._leg.SetTextFont(42)
        self._leg.SetTextSize(textSize)
        if 'data' in pmap: 
            self._leg.AddEntry(pmap['data'], 'Data', 'LP')
        total = sum([x.Integral() for x in pmap.itervalues()])
        for p in mca.listBackgrounds():
            if p in pmap and pmap[p].Integral() >= cutoff*total: 
                self._leg.AddEntry(pmap[p],p,'F')
        for p in mca.listSignals():
            if p in pmap and pmap[p].Integral() >= cutoff*total: 
                self._leg.AddEntry(pmap[p],p,'F')
    def Draw(self):
        self._leg.Draw()

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
            for (name,expr,bins,options) in plots.plots():
                print "    plot: ",name
                pmap = mca.getPlots(name,expr,bins,cut,makeSummary=True)
                for k,v in pmap.iteritems():
                    v.SetDirectory(dir)
                    dir.WriteTObject(v)
                #
                if not makeStack: continue
                stack = ROOT.THStack(name+"_stack",name)
                for p in mca.listBackgrounds():
                    if p in pmap: stack.Add(pmap[p])
                for p in mca.listSignals():
                    if p in pmap: stack.Add(pmap[p])
                dir.WriteTObject(stack)
                # 
                if not makeCanvas: continue
                c1 = ROOT.TCanvas(name+"_canvas",name)
                stack.Draw()
                if 'data' in pmap: pmap['data'].Draw("E SAME")
                leg = Legend(pmap,mca); leg.Draw()
                dir.WriteTObject(c1)
                c1.Print("~/public_html/drop/plot.png")
                c1.Close()
def addPlotMakerOptions(parser):
    addMCAnalysisOptions(parser)
    parser.add_option("--ss", "--scale-signal", dest="signalPlotScale", default=1.0, help="scale the signal in the plots by this amount");

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


