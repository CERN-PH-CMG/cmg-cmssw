#!/usr/bin/env python
#from mcPlots import *
from CMGTools.TTHAnalysis.plotter.mcPlots import *

if "/bin2Dto1Dlib_cc.so" not in ROOT.gSystem.GetLibraries():
    ROOT.gROOT.ProcessLine(".L %s/src/CMGTools/TTHAnalysis/python/plotter/bin2Dto1Dlib.cc+" % os.environ['CMSSW_BASE']);

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
    doRatio = options.showRatio and len(effs) > 1
    # define aspect ratio
    if doRatio: ROOT.gStyle.SetPaperSize(20.,25.)
    else:       ROOT.gStyle.SetPaperSize(20.,20.)
    # create canvas
    c1 = ROOT.TCanvas(outname+"_canvas", outname, 600, (750 if doRatio else 600))
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
    p1.SetGridy(options.showGrid)
    p1.SetGridx(options.showGrid)
    p1.SetLogx(x.getOption('Logx',False))
    p1.SetLogy(options.logy)
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
    if doRatio:
        p2.cd()
        h0.GetXaxis().SetLabelOffset(999) ## send them away
        h0.GetXaxis().SetTitleOffset(999) ## in outer space
        h0.GetYaxis().SetLabelSize(0.05)
        doEffRatio(x,effs,options)
    c1.Print(outname.replace(".root","")+".png")
    c1.Print(outname.replace(".root","")+".eps")
    c1.Print(outname.replace(".root","")+".pdf")

def doEffRatio(x,effs,options):
    effrels = [ e.ProjectionX(n+"_rel") for (n,e) in effs ]
    unity   = effrels[0]
    rmin, rmax = 1,1
    for ie,eff in enumerate(effrels):
        for b in xrange(1,eff.GetNbinsX()+1):
            scale = effs[0][1].GetBinContent(b)
            if scale == 0:
                eff.SetBinContent(b,0)
                eff.SetBinError(b,0)
                continue
            eff.SetBinContent(b, eff.GetBinContent(b)/scale)
            eff.SetBinError(b, eff.GetBinError(b)/scale)
            if ie == 0:
                eff.SetFillStyle(3013)
                eff.SetFillColor(effs[ie][1].GetLineColor())
                eff.SetMarkerStyle(0)
            else:
                eff.SetLineColor(effs[ie][1].GetLineColor())
                eff.SetLineWidth(effs[ie][1].GetLineWidth())
                eff.SetMarkerColor(effs[ie][1].GetMarkerColor())
                eff.SetMarkerStyle(effs[ie][1].GetMarkerStyle())
            rmax = max(rmax, eff.GetBinContent(b)+2*eff.GetBinError(b))
            rmin = min(rmin, max(0,eff.GetBinContent(b)-2*eff.GetBinError(b)))
    if options.ratioRange != (-1,-1):
        rmin,rmax = options.ratioRange
    unity.Draw("E2");
    unity.GetYaxis().SetRangeUser(rmin,rmax);
    unity.GetXaxis().SetTitleSize(0.14)
    unity.GetYaxis().SetTitleSize(0.14)
    unity.GetXaxis().SetLabelSize(0.11)
    unity.GetYaxis().SetLabelSize(0.11)
    unity.GetYaxis().SetNdivisions(505)
    unity.GetYaxis().SetDecimals(True)
    unity.GetYaxis().SetTitle("X / "+effs[0][0])
    unity.GetYaxis().SetTitleOffset(0.52);
    line = ROOT.TLine(unity.GetXaxis().GetXmin(),1,unity.GetXaxis().GetXmax(),1)
    line.SetLineWidth(3);
    line.SetLineColor(effs[0][1].GetLineColor());
    line.DrawLine(unity.GetXaxis().GetXmin(),1,unity.GetXaxis().GetXmax(),1)
    for ratio in effrels[1:]:
        ratio.Draw("E SAME");

    

def makeEff(mca,cut,idplot,xvarplot,notDoProfile=False):
    import copy
    is2D = (":" in xvarplot.expr.replace("::","--"))
    options = copy.copy(idplot.opts)
    options.update(xvarplot.opts)
    mybins = copy.copy(xvarplot.bins)
    if not notDoProfile:
        if is2D: options['Profile2D']=True
        else:    options['Profile1D']=True
    else:
        if xvarplot.bins[0] == "[":
            mybins += "*[-0.5,0.5,1.5]"
        else:
            mybins += ",2,-0.5,1.5"
    pspec = PlotSpec("%s_vs_%s"  % (idplot.name, xvarplot.name), 
                     "%s:%s" % (idplot.expr,xvarplot.expr),
                     mybins,
                     options) 
    print pspec.name
    print mybins
    print options
    return mca.getPlots(pspec,cut,makeSummary=True)

if __name__ == "__main__":
    from optparse import OptionParser
    parser = OptionParser(usage="%prog [options] mc.txt cuts.txt plotfile.txt")
    addROCMakerOptions(parser)
    parser.add_option("-o", "--out", dest="out", default=None, help="Output file name. by default equal to plots -'.txt' +'.root'");
    parser.add_option("--rebin", dest="globalRebin", type="int", default="0", help="Rebin all plots by this factor")
    parser.add_option("--xrange", dest="xrange", default=None, nargs=2, type='float', help="X axis range");
    parser.add_option("--xcut", dest="xcut", default=None, nargs=2, type='float', help="X axis cut");
    parser.add_option("--yrange", dest="yrange", default=None, nargs=2, type='float', help="Y axis range");
    parser.add_option("--logy", dest="logy", default=False, action='store_true', help="Do y axis in log scale");
    parser.add_option("--ytitle", dest="ytitle", default="Efficiency", type='string', help="Y axis title");
    parser.add_option("--fontsize", dest="fontsize", default=0, type='float', help="Legend font size");
    parser.add_option("--grid", dest="showGrid", action="store_true", default=False, help="Show grid lines")
    parser.add_option("--groupBy",  dest="groupBy",  default="process",  type="string", help="Group by: cut, process")
    parser.add_option("--legend",  dest="legend",  default="TR",  type="string", help="Legend position (BR, TR)")
    parser.add_option("--showRatio", dest="showRatio", action="store_true", default=False, help="Add a data/sim ratio plot at the bottom")
    parser.add_option("--rr", "--ratioRange", dest="ratioRange", type="float", nargs=2, default=(-1,-1), help="Min and max for the ratio")
    parser.add_option("--normEffUncToLumi", dest="normEffUncToLumi", action="store_true", default=False, help="Normalize the dataset to the given lumi for the uncertainties on the calculated efficiency")
    (options, args) = parser.parse_args()
    options.globalRebin = 1
    options.allowNegative = True # with the fine bins used in ROCs, one otherwise gets nonsensical results
    mca  = MCAnalysis(args[0],options)
    procs = mca.listSignals()+mca.listBackgrounds()
    cut = CutsFile(args[1],options).allCuts()
    ids   = PlotFile(args[2],options).plots()
    xvars = PlotFile(args[3],options).plots()
    outname  = options.out if options.out else (args[2].replace(".txt","")+".root")
    if os.path.dirname(outname) != "":
        dirname = os.path.dirname(outname)
        if not os.path.exists(dirname):
            os.system("mkdir -p "+dirname)
            if os.path.exists("/afs/cern.ch"): os.system("cp /afs/cern.ch/user/g/gpetrucc/php/index.php "+os.path.dirname(outname))
    outfile  = ROOT.TFile(outname,"RECREATE")
    ROOT.gROOT.ProcessLine(".x tdrstyle.cc")
    ROOT.gStyle.SetErrorX(0.5)
    ROOT.gStyle.SetOptStat(0)
    effplots = [ (y,x,makeEff(mca,cut,y,x,options.normEffUncToLumi)) for y in ids for x in xvars ]
    for (y,x,pmap) in effplots:
        for proc in procs:
            eff = pmap[proc]
            if not eff: continue
            eff.Print()
#            PrintHisto(eff)
            if options.xcut:
                ax = eff.GetXaxis()
                for b in xrange(1,eff.GetNbinsX()+1):
                    if ax.GetBinCenter(b) < options.xcut[0] or ax.GetBinCenter(b) > options.xcut[1]:
                        eff.SetBinContent(b,0)
                        eff.SetBinError(b,0)

            if options.normEffUncToLumi:
                assert (("TH3" in eff.ClassName()) or ("TH2" in eff.ClassName()))
                is1d = "TH3" not in eff.ClassName()
                eff.Print()
                print 'is1d is ',is1d
                binsfail = []
                binspass = []
                if is1d:
                    binsfail = [eff.GetBin(i1,1) for i1 in range(1,eff.GetNbinsX()+1)]
                    binspass = [eff.GetBin(i1,2) for i1 in range(1,eff.GetNbinsX()+1)]
                else:
                    binsfail = [eff.GetBin(i1,i2,1) for i1 in range(1,eff.GetNbinsX()+1) for i2 in range(1,eff.GetNbinsY()+1)]
                    binspass = [eff.GetBin(i1,i2,2) for i1 in range(1,eff.GetNbinsX()+1) for i2 in range(1,eff.GetNbinsY()+1)]

                for bin in binspass+binsfail:
                    eff.SetBinError(bin,sqrt(eff.GetBinContent(bin)))

                outfile.WriteTObject(eff)

                effratio = eff.ProjectionX("_px") if is1d else eff.Project3D("yx")
                effratio.Reset()

                eff.Print()
                effratio.Print()

                for b1 in xrange(len(binsfail)):
                    passing = eff.GetBinContent(binspass[b1])
                    failing = eff.GetBinContent(binsfail[b1])
                    bx = ROOT.Long(0)
                    by = ROOT.Long(0)
                    bz = ROOT.Long(0)
                    eff.GetBinXYZ(binspass[b1],bx,by,bz)
                    if is1d:
                        ratiobin = effratio.FindBin(eff.GetXaxis().GetBinCenter(bx))
                    else:
                        ratiobin = effratio.FindBin(eff.GetXaxis().GetBinCenter(bx),eff.GetYaxis().GetBinCenter(by))
                    effratio.SetBinContent(ratiobin,passing/(passing+failing))
                    effratio.SetBinError(ratiobin,sqrt(passing*failing*((passing+failing)**(-3))))
#                    print 'bx',ROOT.Long(bx)
#                    print 'num',passing
#                    print 'den',passing+failing
#                    print 'ratio',effratio.GetBinContent(ratiobin)
#                    print 'err',effratio.GetBinError(ratiobin)
#                    print 'relerr',effratio.GetBinError(ratiobin)/effratio.GetBinContent(ratiobin)
                eff = effratio
                pmap[proc] = effratio
#            else:
#                for bx in xrange(eff.GetNbinsX()):
#                    print bx
#                    print eff.GetBinContent(bx+1)
#                    print eff.GetBinError(bx+1)
            eff.SetName("_".join([y.name,x.name,proc]))
            outfile.WriteTObject(eff)
    if len(procs)>=1 and "cut" in options.groupBy:
        for x in xvars:
            for y,ex,pmap in effplots:
                if ex != x: continue
                effs = []
                myname = outname.replace(".root","_%s_%s.root" % (y.name,x.name))
                for proc in procs:
                    eff = pmap[proc]
                    if not eff: continue
                    eff.SetLineColor(mca.getProcessOption(proc,"FillColor",SAFE_COLOR_LIST[len(effs)]))
                    eff.SetMarkerColor(mca.getProcessOption(proc,"FillColor",SAFE_COLOR_LIST[len(effs)]))
                    eff.SetMarkerStyle(mca.getProcessOption(proc,"MarkerStyle",20))
                    eff.SetMarkerSize(mca.getProcessOption(proc,"MarkerSize",1.6)*0.8)
                    eff.SetLineWidth(4)
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
                    eff.SetLineWidth(4)
                    effs.append((y.getOption("Title",y.name),eff))
            if len(effs) == 0: continue
            stackEffs(myname,x,effs,options)
    outfile.Close()


