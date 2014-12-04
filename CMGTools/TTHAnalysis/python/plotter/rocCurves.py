#!/usr/bin/env python
#from mcPlots import *
from CMGTools.TTHAnalysis.plotter.mcPlots import *


def hist2ROC1d(hsig,hbg):
    bins = hsig.GetNbinsX()+2
    si = [ hsig.GetBinContent(i) for i in xrange(bins) ]
    bi = [  hbg.GetBinContent(i) for i in xrange(bins) ]
    if hsig.GetMean() > hbg.GetMean():
        si.reverse(); bi.reverse()
    sums,sumb = sum(si), sum(bi)
    for i in xrange(1,bins): 
        si[i] += si[i-1]
        bi[i] += bi[i-1]
    fullsi, fullbi = si[:], bi[:]
    si, bi = [], [];
    for i in xrange(1,bins):
        if fullsi[i] != fullsi[i-1] or fullbi[i] != fullbi[i-1]:
            si.append(fullsi[i])
            bi.append(fullbi[i])
    if len(si) == 2: # just one WP + dummy (100%,100%)
        si = [si[0]]; bi = [ bi[0] ]
    bins = len(si)
    ret = ROOT.TGraph(bins)
    for i in xrange(bins):
        ret.SetPoint(i, bi[i]/sumb, si[i]/sums)
    ret.dim=1
    return ret

def hist2ROC2d(hsig,hbg):
    binsx = hsig.GetNbinsX()
    binsy = hsig.GetNbinsY()
    si = [ [ hsig.GetBinContent(ix,iy) for ix in xrange(1,binsx+1) ] for iy in xrange(1,binsy+1) ]
    bi = [ [  hbg.GetBinContent(ix,iy) for ix in xrange(1,binsx+1) ] for iy in xrange(1,binsy+1) ]
    sumsi = [ sum(s) for s in si ]
    sums,sumb = sum([sum(s) for s in si]), sum([sum(b) for b in bi])
    if hsig.GetMean(1) > hbg.GetMean(1): 
        si.reverse(); bi.reverse()
    for ix in xrange(0,binsx):
        if hsig.GetMean(2) > hbg.GetMean(2): 
            si[ix].reverse()
            bi[ix].reverse()
        for iy in xrange(1,binsx): 
            si[ix][iy] += si[ix][iy-1]
            bi[ix][iy] += bi[ix][iy-1]
    for ix in xrange(1,binsx):
        for iy in xrange(1,binsx): 
            si[ix][iy] += si[ix-1][iy]
            bi[ix][iy] += bi[ix-1][iy]
    bins = binsx*binsy
    ret = ROOT.TGraph(bins)
    i = 0
    for ix in xrange(binsx): 
        for iy in xrange(binsx): 
            ret.SetPoint(i, bi[ix][iy]/sumb, si[ix][iy]/sums)
            i += 1
    ret.dim=2
    return ret

def makeROC(plotmap,mca):
    sig, bkg = None, None
    if 'signal'     in plotmap: sig = plotmap['signal']
    if 'background' in plotmap: bkg  = plotmap['background']
    if sig.ClassName() == "TH1F":
        ret = hist2ROC1d(sig,bkg)
    elif sig.ClassName() == "TH2F":
        ret = hist2ROC2d(sig,bkg)
    else: raise RuntimeError, "Can't make a ROC from a "+sig.ClassName()
    ret.GetXaxis().SetTitle("Eff Background")
    ret.GetYaxis().SetTitle("Eff Signal")
    return ret

def addROCMakerOptions(parser):
    addMCAnalysisOptions(parser)
    parser.add_option("--select-plot", "--sP", dest="plotselect", action="append", default=[], help="Select only these plots out of the full file")
    parser.add_option("--exclude-plot", "--xP", dest="plotexclude", action="append", default=[], help="Exclude these plots from the full file")

def doLegend(rocs,textSize=0.035):
        (x1,y1,x2,y2) = (.6, .30 + textSize*max(len(rocs)-3,0), .93, .18)
        leg = ROOT.TLegend(x1,y1,x2,y2)
        leg.SetFillColor(0)
        leg.SetShadowColor(0)
        leg.SetTextFont(42)
        leg.SetTextSize(textSize)
        for key,val in rocs:
            leg.AddEntry(val, key, val.style)
        leg.Draw()
        ## assign it to a global variable so it's not deleted
        global legend_;
        legend_ = leg 
        return leg

if __name__ == "__main__":
    from optparse import OptionParser
    parser = OptionParser(usage="%prog [options] mc.txt cuts.txt plotfile.txt")
    addROCMakerOptions(parser)
    parser.add_option("-o", "--out", dest="out", default=None, help="Output file name. by default equal to plots -'.txt' +'.root'");
    parser.add_option("--xrange", dest="xrange", default=None, nargs=2, type='float', help="X axis range");
    parser.add_option("--yrange", dest="yrange", default=None, nargs=2, type='float', help="X axis range");
    parser.add_option("--xtitle", dest="xtitle", default="Eff Background", type='string', help="X axis title");
    parser.add_option("--ytitle", dest="ytitle", default="Eff Signal", type='string', help="Y axis title");
    parser.add_option("--fontsize", dest="fontsize", default=0, type='float', help="Legend font size");
    (options, args) = parser.parse_args()
    options.globalRebin = 1
    mca  = MCAnalysis(args[0],options)
    cut = CutsFile(args[1],options).allCuts()
    plots = PlotFile(args[2],options)
    outname  = options.out if options.out else (args[2].replace(".txt","")+".root")
    outfile  = ROOT.TFile(outname,"RECREATE")
    rocs = []
    ROOT.gROOT.ProcessLine(".x tdrstyle.cc")
    c1 = ROOT.TCanvas("roc_canvas","roc_canvas")
    allrocs = ROOT.TMultiGraph("all","all")
    for i,plot in enumerate(plots.plots()):
        pmap = mca.getPlots(plot,cut,makeSummary=True)
        roc = makeROC(pmap,mca)
        if roc.GetN() > 1 and roc.dim == 1 and not plot.getOption("Discrete",False):
            roc.SetLineColor(plot.getOption("LineColor",i+1))
            roc.SetMarkerColor(plot.getOption("LineColor",i+1))
            roc.SetLineWidth(2)
            roc.SetMarkerStyle(0)
            roc.style = "L"
        else:
            #print roc.GetX()[0],roc.GetY()[0],plot.name
            roc.SetMarkerColor(plot.getOption("MarkerColor",i+1))
            roc.SetMarkerStyle(plot.getOption("MarkerStyle",20 if roc.dim == 1 else 7))
            roc.SetMarkerSize(plot.getOption("MarkerSize",1.0))
            roc.style = "P"
        allrocs.Add(roc,roc.style)
        roc.SetName(plot.name)
        rocs.append((plot.getOption("Title",plot.name),roc))
        outfile.WriteTObject(roc)
    allrocs.Draw("APL");
    allrocs.GetXaxis().SetTitle(options.xtitle)
    allrocs.GetYaxis().SetTitle(options.ytitle)
    if options.xrange:
        allrocs.GetXaxis().SetRangeUser(options.xrange[0], options.xrange[1])
    if options.yrange:
        allrocs.GetYaxis().SetRangeUser(options.yrange[0], options.yrange[1])
    allrocs.Draw()
    leg = doLegend(rocs)
    if options.fontsize: leg.SetTextSize(options.fontsize)
    c1.Print(outname.replace(".root","")+".png")
    outfile.WriteTObject(c1,"roc_canvas")
    outfile.Close()


