#!/usr/bin/env python
#from mcPlots import *
from CMGTools.TTHAnalysis.plotter.mcPlots import *


def addROCMakerOptions(parser):
    addMCAnalysisOptions(parser)
    parser.add_option("--select-plot", "--sP", dest="plotselect", action="append", default=[], help="Select only these plots out of the full file")
    parser.add_option("--exclude-plot", "--xP", dest="plotexclude", action="append", default=[], help="Exclude these plots from the full file")

def doLegend(rocs,options,textSize=0.035):
        if options.legend == "TR":
            (x1,y1,x2,y2) = (.6, .85 - textSize*max(len(rocs)-3,0), .93, .98)
        elif options.legend == "TL":
            (x1,y1,x2,y2) = (.2, .85 - textSize*max(len(rocs)-3,0), .53, .98)
        else:
            (x1,y1,x2,y2) = (.6, .30 + textSize*max(len(rocs)-3,0), .93, .18)
        leg = ROOT.TLegend(x1,y1,x2,y2)
        leg.SetFillColor(0)
        leg.SetShadowColor(0)
        leg.SetTextFont(42)
        leg.SetTextSize(textSize)
        for key,val in rocs:
            leg.AddEntry(val, key, "LP")
        leg.Draw()
        ## assign it to a global variable so it's not deleted
        global legend_;
        legend_ = leg 
        return leg

def stackEffs(outname,x,effs,options):
    alleffs = ROOT.THStack("all","all")
    for title,eff in effs:
        alleffs.Add(eff)
    c1 = ROOT.TCanvas("eff_canvas","eff_canvas")
    c1.SetGridy(options.showGrid)
    c1.SetGridx(options.showGrid)
    c1.SetLogx(x.getOption('Logx',False))
    alleffs.Draw("APL");
    h0 = effs[0][1].Clone("frame"); h0.Reset();
    h0.GetYaxis().SetDecimals()
    h0.GetYaxis().SetTitle(options.ytitle)
    h0.Draw("AXIS");
    alleffs.Draw("NOSTACK P SAME")
    if options.xrange:
        h0.GetXaxis().SetRangeUser(options.xrange[0], options.xrange[1])
    if options.yrange:
        h0.GetYaxis().SetRangeUser(options.yrange[0], options.yrange[1])
    leg = doLegend(effs,options)
    if options.fontsize: leg.SetTextSize(options.fontsize)
    c1.Print(outname.replace(".root","")+".png")
    c1.Print(outname.replace(".root","")+".eps")
    c1.Print(outname.replace(".root","")+".pdf")

def makeEff(mca,cut,idplot,xvarplot):
    import copy
    is2D = (":" in xvarplot.expr.replace("::","--"))
    options = copy.copy(idplot.opts)
    options.update(xvarplot.opts)
    if is2D: options['Profile2D']=True
    else:    options['Profile1D']=True
    pspec = PlotSpec("%s_vs_%s"  % (idplot.name, xvarplot.name), 
                     "%s:%s" % (idplot.expr,xvarplot.expr),
                     xvarplot.bins,
                     options) 
    print pspec.name
    return mca.getPlots(pspec,cut,makeSummary=True)

if __name__ == "__main__":
    from optparse import OptionParser
    parser = OptionParser(usage="%prog [options] mc.txt cuts.txt plotfile.txt")
    addROCMakerOptions(parser)
    parser.add_option("-o", "--out", dest="out", default=None, help="Output file name. by default equal to plots -'.txt' +'.root'");
    parser.add_option("--rebin", dest="globalRebin", type="int", default="0", help="Rebin all plots by this factor")
    parser.add_option("--xrange", dest="xrange", default=None, nargs=2, type='float', help="X axis range");
    parser.add_option("--yrange", dest="yrange", default=None, nargs=2, type='float', help="X axis range");
    parser.add_option("--ytitle", dest="ytitle", default="Efficiency", type='string', help="Y axis title");
    parser.add_option("--fontsize", dest="fontsize", default=0, type='float', help="Legend font size");
    parser.add_option("--grid", dest="showGrid", action="store_true", default=False, help="Show grid lines")
    parser.add_option("--groupBy",  dest="groupBy",  default="process",  type="string", help="Group by: cut, process")
    parser.add_option("--legend",  dest="legend",  default="TR",  type="string", help="Legend position (BR, TR)")
    (options, args) = parser.parse_args()
    options.globalRebin = 1
    options.allowNegative = True # with the fine bins used in ROCs, one otherwise gets nonsensical results
    mca  = MCAnalysis(args[0],options)
    procs = mca.listSignals()+mca.listBackgrounds()
    cut = CutsFile(args[1],options).allCuts()
    ids   = PlotFile(args[2],options).plots()
    xvars = PlotFile(args[3],options).plots()
    outname  = options.out if options.out else (args[2].replace(".txt","")+".root")
    outfile  = ROOT.TFile(outname,"RECREATE")
    ROOT.gROOT.ProcessLine(".x tdrstyle.cc")
    ROOT.gStyle.SetErrorX(0.5)
    ROOT.gStyle.SetOptStat(0)
    effplots = [ (y,x,makeEff(mca,cut,y,x)) for y in ids for x in xvars ]
    if len(procs)>1 and "cut" in options.groupBy:
        for x in xvars:
            for y,ex,pmap in effplots:
                if ex != x: continue
                effs = []
                myname = outname.replace(".root","_%s_%s.root" % (y.name,x.name))
                for proc in procs:
                    eff = pmap[proc]
                    if not eff: continue
                    eff.SetLineColor(mca.getProcessOption(proc,"FillColor",SAFE_COLOR_LIST[len(effs)]))
                    eff.SetFillColor(mca.getProcessOption(proc,"FillColor",SAFE_COLOR_LIST[len(effs)]))
                    eff.SetMarkerStyle(mca.getProcessOption(proc,"MarkerStyle",33))
                    eff.SetMarkerSize(mca.getProcessOption(proc,"MarkerSize",1.4)*0.8)
                    eff.SetLineWidth(2)
                    effs.append((mca.getProcessOption(proc,"Label",proc),eff))
                if len(effs) == 0: continue
                stackEffs(myname,x,effs,options)
    if "process" in options.groupBy:
        for proc in procs:
            for x in xvars:
                effs = []
                myname = outname.replace(".root","_%s_%s.root" % (proc,x.name))
                for y,ex,pmap in effplots:
                    if ex != x: continue
                    eff = pmap[proc]
                    if not eff: continue
                    eff.SetLineColor(y.getOption("MarkerColor",SAFE_COLOR_LIST[len(effs)]))
                    eff.SetMarkerColor(y.getOption("MarkerColor",SAFE_COLOR_LIST[len(effs)]))
                    eff.SetMarkerStyle(y.getOption("MarkerStyle",33))
                    eff.SetMarkerSize(y.getOption("MarkerSize",1.4)*0.8)
                    eff.SetLineWidth(2)
                    effs.append((y.getOption("Title",y.name),eff))
            if len(effs) == 0: continue
            stackEffs(myname,x,effs,options)
    outfile.Close()


