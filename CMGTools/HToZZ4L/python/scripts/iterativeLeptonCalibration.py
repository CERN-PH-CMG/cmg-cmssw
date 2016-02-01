import ROOT
from CMGTools.HToZZ4L.scripts.zFitter import *
from array import array

def _processOne2DBin(args):
    (ix,iy,binlabel,hist2D,hist1M,hist1E,options) = args
    res = {}; refres = {}
    name = options.name+"_"+binlabel
    if False:
        c1 = ROOT.TCanvas("c1","c1")
        hist1M.Draw();
        printCanvas(c1, name+"_hmass", [], options)
        hist1E.Draw();
        printCanvas(c1, name+"_hmassErr", [], options)
    if options.mode == "1D":
        (w,data,pdf, params, result) = fit1D(hist1M, options)
        #makePlot1D(w, data, pdf, params, result, name+"_fitPlot1D", options)
        var = result.floatParsFinal().find("dm")
        return ((ix,iy),(var.getVal(),var.getError()))
    elif options.mode == "2D":
        (w,data,pdf, params, result) = fit2D(hist2D, options)
        makePlot2D(w, data, pdf, params, result, name+"_fitPlot2D", options)
        dm = result.floatParsFinal().find("dm")
        lam = result.floatParsFinal().find("lambda")
        return ((ix,iy),(dm.getVal(),dm.getError(),lam.getVal(),lam.getError()))

    raise RuntimeError, "Should not be here"


class ILC2D(object):
    def __init__(self, tree, xedges, yedges, options, weightedMC = False):
        self.xedges = xedges;
        self.yedges = yedges;
        self.xbins  = (len(xedges)-1)
        self.ybins  = (len(yedges)-1)
        self.nbins  = (len(xedges)-1)*(len(yedges)-1)
        ROOT.initBinning2D(len(xedges)-1, array('f', xedges), len(yedges)-1, array('f', yedges));
        self.template = ROOT.TH2F("template","template",len(xedges)-1, array('f', xedges), len(yedges)-1, array('f', yedges));
        self.options = options
        self.tree = tree
        self.weightedMC = weightedMC
    def testBinning2D(self,binVar="bin2D(z_l1_pt[0],z_l1_eta[0])",massvar="z_mass[0]",  mrange=[70,110]):
        c1 = ROOT.TCanvas("c1");
        finalCut =  "(%s) && (%g < %s && %s < %g)" % (makeCut(options.cut), mrange[0], massvar,massvar, mrange[1])
        c1.SetLogy(True)
        self.tree.Draw("%s>>hbin(%d,%g,%g)" % (binVar, self.nbins, -0.5, self.nbins-0.5), finalCut);
        printCanvas(c1, self.options.name+"_binning", [], options)
        c1.SetLogy(False)
        c1.SetLogz(True)
        c1.SetRightMargin(0.25)
        ROOT.gROOT.cd()
        h2d = ROOT.TH2D("h2d","h2d",len(self.xedges)-1, array('f', self.xedges), len(self.yedges)-1, array('f', self.yedges));
        self.tree.Draw("z_l1_eta[0]:z_l1_pt[0]>>h2d",finalCut,"COLZ TEXT");
        printCanvas(c1, self.options.name+"_binning2D", [], options)
    def halfIteration2D(self, ilep, iterno,
                    binVar="bin2D(z_l{lep}_pt[0],z_l{lep}_eta[0])", 
                    massvar="massCorr2D(z_l1_pt[0],z_l1_ptErr[0],z_l1_eta[0],z_l1_phi[0],z_l1_mass[0],z_l2_pt[0],z_l2_ptErr[0],z_l2_eta[0],z_l2_phi[0])",
                   dmvar="massErrCorr2D(z_l1_pt[0],z_l1_ptErr[0],z_l1_eta[0],z_l1_phi[0],z_l1_mass[0],z_l2_pt[0],z_l2_ptErr[0],z_l2_eta[0],z_l2_phi[0])", 
                      massbins=160, mrange=[70,110],
                      dmbins=20, dmrange=[0.3,4]):
        binVar = binVar.format(lep=ilep+1)
        print "Filling tree for cut %s, binVar %s, bins %d" % (options.cut, binVar, self.nbins)
        finalCut =  makeCut(options.cut)
        if self.weightedMC: finalCut = "(%s) * (genWeight/abs(genWeight)) " % finalCut
        expr = "%s:%s:%s" % (binVar, dmvar, massvar)
        binning = "%d,%g,%g,%d,%g,%g,%d,%g,%g" % ( massbins, mrange[0], mrange[1], 
                                                   dmbins, dmrange[0], dmrange[1], 
                                                   self.nbins, -0.5, self.nbins-0.5);
        tree.Draw("%s>>hist(%s)" % (expr, binning), finalCut, "goff", options.maxEntries)
        print "Splitting."
        hist3D = ROOT.gROOT.FindObject("hist"); hist3D.SetDirectory(None)
        allhists = []
        if ilep == 0:
            print "Monitoring"
            mass1D = ROOT.TH1D("h1d_all","h1d_all",massbins, mrange[0], mrange[1])
            mass2D = ROOT.TH2D("h2d_all","h2d_all",massbins, mrange[0], mrange[1], dmbins, dmrange[0], dmrange[1]);
        for ix in xrange(self.xbins):
          for iy in xrange(self.ybins):
            binlabel = "iter_%02d_lep%d_%.1f-%.1f_%+.2f_%+.2f" % (iterno, ilep+1, self.xedges[ix],self.xedges[ix+1], self.yedges[iy], self.yedges[iy+1])
            binidx = ix + (iy)*self.xbins;
            hist2D = ROOT.TH2D("h2d_"+binlabel,"",massbins, mrange[0], mrange[1], dmbins, dmrange[0], dmrange[1]);
            hist2D.SetDirectory(None);
            for bx in xrange(1,massbins+1):
              for by in xrange(1,dmbins+1):
                hist2D.SetBinContent(bx,by,max(0,hist3D.GetBinContent(bx,by,binidx+1)))
            hist1M = hist2D.ProjectionX("h1d_"+binlabel+"_mass",    1, dmbins+1);   hist1M.SetDirectory(None);
            hist1E = hist2D.ProjectionY("h1d_"+binlabel+"_massErr", 1, massbins+1); hist1E.SetDirectory(None);
            allhists.append((ix,iy,binlabel,hist2D,hist1M,hist1E,options))
            if ilep == 0: 
                mass1D.Add(hist1M)
                mass2D.Add(hist2D)
        if ilep == 0:
            (w,data,pdf, params, result) = fit1D(mass1D, self.options, modelOverride="Z-DCB")
            makePlot1D(w, data, pdf, params, result, self.options.name+"_iter%d_control1D" % iterno, self.options)
            if options.mode == "2D":
                (w,data,pdf, params, result) = fit2D(mass2D, self.options)
                makePlot2D(w, data, pdf, params, result, self.options.name+"_iter%d_control2D" % iterno, self.options)
        print "Fitting"
        if options.jobs > 0:
            from multiprocessing import Pool
            fits = dict(Pool(options.jobs).map(_processOne2DBin, allhists))
        else:
            fits = dict(map(_processOne2DBin, allhists))
        return fits
    def iteration2D(self,iterno):
        fits = [ self.halfIteration2D(i,iterno) for i in (0,1) ]
        mergedResults = {}
        mergedCorrs = {}
        fitPlot = self.template.Clone("fit_iter%d"  % iterno)
        lamPlot = self.template.Clone("lam_iter%d"  % iterno)
        corPlot = self.template.Clone("corr_iter%d" % iterno)
        lcorPlot = self.template.Clone("lcorr_iter%d" % iterno)
        for ix in xrange(self.xbins):
          for iy in xrange(self.ybins):
            oks = [ f[(ix,iy)] for f in fits if (ix,iy) in f ]
            if not oks: continue
            if options.mode == "1D":
                sumw2 = sum(1.0/err**2 for (val,err) in oks)
                avg   = sum(val/err**2 for (val,err) in oks)/sumw2
                mergedResults[(ix,iy)] = avg
                mergedCorrs[(ix,iy)] = ( 1.0-(options.damping * avg/91.18), 1.0 )
                fitPlot.SetBinContent(ix+1,iy+1,avg);
                fitPlot.SetBinError(ix+1,iy+1,1/sqrt(sumw2));
                corPlot.SetBinContent(ix+1,iy+1,ROOT.getPtCorr2D(ix,iy)-1)
            elif options.mode == "2D":
                msumw2 = sum(1.0/merr**2  for (mval,merr,lval,lerr) in oks)
                mavg   = sum(mval/merr**2 for (mval,merr,lval,lerr) in oks)/msumw2
                lsumw2 = sum(1.0/lerr**2  for (mval,merr,lval,lerr) in oks)
                lavg   = sum(lval/lerr**2 for (mval,merr,lval,lerr) in oks)/lsumw2
                mergedResults[(ix,iy)] = (mavg,lavg)
                mergedCorrs[(ix,iy)] = (1.0-(options.damping * mavg/91.18), 1.0+options.damping*(lavg-1.0))
                fitPlot.SetBinContent(ix+1,iy+1,mavg);
                fitPlot.SetBinError(ix+1,iy+1,1/sqrt(msumw2));
                lamPlot.SetBinContent(ix+1,iy+1,lavg);
                lamPlot.SetBinError(ix+1,iy+1,1/sqrt(lsumw2));
        if len(mergedCorrs) == 0:
            return False
        maxCorrPt, maxCorrPtErr = [ max(abs(c[i]-1.0) for c in mergedCorrs) for i in (0,1) ]
        for ix in xrange(self.xbins):
          for iy in xrange(self.ybins):
                if (ix,iy) in mergedCorrs: 
                    corrPt, corrPtErr = mergedCorrs[(ix,iy)]
                    if abs(corrPt-1.0) < options.threshold*maxCorrPt: corrPt = 1.0
                    if abs(corrPtErr-1.0) < options.threshold*maxCorrPtErr: corrPtErr = 1.0
                    ROOT.updateCorr2D(ix,iy, corrPt, corrPtErr)
                corPlot.SetBinContent(ix+1,iy+1,ROOT.getPtCorr2D(ix,iy)-1)
                lcorPlot.SetBinContent(ix+1,iy+1,ROOT.getPtErrCorr2D(ix,iy))
        self.savePlot2D(fitPlot, "fit_iter%d"  % iterno, zrange=[-2.500,+2.500])
        self.savePlot2D(corPlot, "cor_iter%d"  % iterno, zrange=[-0.025,+0.025], opt="COLZ0 TEXT")
        if options.mode == '2D':
            self.savePlot2D(lamPlot, "lam_iter%d"  % iterno,   zrange=[0.5,1.5])
            self.savePlot2D(lcorPlot, "lcor_iter%d"  % iterno, zrange=[0.5,1.5], opt="COLZ0 TEXT")
        self.history.append( mergedResults )
    def savePlot2D(self,hist,name,opt="COLZ0 TEXTE",zrange=None):
        c1 = ROOT.TCanvas("c1");
        c1.SetRightMargin(0.2)
        ROOT.gStyle.SetPaintTextFormat(".3f")
        hist.SetContour(100)
        c1.SetLogx(True)
        hist.Draw(opt)
        if zrange: hist.GetZaxis().SetRangeUser(zrange[0], zrange[1])
        text = []
        text.append("%-6s  %-6s  %-6s  %-6s   %-7s  %-7s" % ( "xmin", "xmax", "ymin", "ymax", "val", "err"))
        text.append("%-6s  %-6s  %-6s  %-6s   %-7s  %-7s" % ( "----", "----", "----", "----", "---", "---"))
        ax = hist.GetXaxis(); ay = hist.GetYaxis()
        for bx in xrange(1,hist.GetNbinsX()+1):
            for by in xrange(1,hist.GetNbinsY()+1):
                text.append("%6.1f  %6.1f  %+6.3f  %+6.3f   %7.4f  %6.4f" % (
                    ax.GetBinLowEdge(bx), ax.GetBinUpEdge(bx),
                    ay.GetBinLowEdge(by), ay.GetBinUpEdge(by),
                    hist.GetBinContent(bx,by), hist.GetBinError(bx,by)))
        printCanvas(c1, self.options.name+"_"+name, text, self.options)
    def suite(self,suitename):
        if self.options.cut == "Zee":
            name = self.options.name
            for B in "BB NonBB BE EE".split():
                self.options.cut = "Zee-"+B
                self.options.name = name + "_"+suitename+"_"+B
                hist =   makeHist1D(self.tree, self.options)
                (w,data,pdf, params, result) = fit1D(hist, self.options)
                makePlot1D(w, data, pdf, params, result, options.name, options)
            self.options.name = name
            self.options.cut = "Zee"
        #else:
        #    raise RuntimeError, "Unknown suite for "+self.options.cut
    def go(self):
        self.options.xvar = ("massCorr2D(z_l1_pt[0],z_l1_ptErr[0],z_l1_eta[0],z_l1_phi[0],z_l1_mass[0],z_l2_pt[0],z_l2_ptErr[0],z_l2_eta[0],z_l2_phi[0])", self.options.xvar[1])
        #self.testBinning2D()
        self.suite("starting")
        self.history = []
        for i in range(1,options.iters+1):
            self.iteration2D(i)
        allkeys = [ (ix,iy) for ix in xrange(self.xbins) for iy in xrange(self.ybins) ]
        for key in allkeys:
            print "%2d,%2d: " % key,
            for h in self.history:
                if self.options.mode == "1D":
                    print "%-8s" % ( "%+5.3f" % h[key] if key in h else " -- "),
                elif self.options.mode == "2D":
                    print "%-12s" % ( "%+5.3f/%5.3f" % h[key] if key in h else " --/-- "),
            print
        self.suite("final")

def addILCOptions(parser):
    parser.add_option("--cm", "--calibration-mode",   dest="cmode", default='2D', help="mode");
    #parser.add_option("--ybins", dest="ybins", type="string", default="[-2.5,-1.5,1.5,2.5]", help="y bins")
    parser.add_option("--xbins", dest="xbins", type="string", default="[20,100]", help="x bins")
    #parser.add_option("--xbins", dest="xbins", type="string", default="[10,30,65,100]", help="x bins")
    #parser.add_option("--xbins", dest="xbins", type="string", default="[5,15,25,35,50,70,100]", help="x bins")
    #parser.add_option("--ybins", dest="ybins", type="string", default="[-2.5,-2.0,-1.5,-1.0,1.0,1.5,2.0,2.5]", help="y bins")
    parser.add_option("--ybins", dest="ybins", type="string", default="[-2.5,-2.1,-1.8,-1.479,-1.1,-0.5,0.5,1.1,1.479,1.8,2.1,2.5]", help="y bins")
    parser.add_option("--damping",   dest="damping", type="float", default=0.3, help="Damping factor")
    parser.add_option("--threshold",   dest="threshold", type="float", default=0.0, help="Threshold: don't do changes below this fraction of the max")
    parser.add_option("-i", "--iters",    dest="iters",      type="int",    default=10, help="Number of iterations of the algorithm");
    

if __name__ == "__main__":
    from optparse import OptionParser
    parser = OptionParser(usage="%prog [options] tree [tree2] ")
    addZFitterOptions(parser)
    addILCOptions(parser)
    (options, args) = parser.parse_args()
    if len(args) == 0:
        print "You must specify at least one tree to fit"
    ROOT.gROOT.SetBatch(True)
    ROOT.gROOT.ProcessLine(".x ~/cpp/tdrstyle.cc")
    ROOT.gROOT.ProcessLine(".L iterativeLeptonCalibration.cc+")
    ROOT.gStyle.SetOptStat(0)
    ROOT.gStyle.SetPalette(54);
    tree = ROOT.TChain(options.tree)
    for fname in args: tree.Add(fname)
    if options.refmc:
        reffile = ROOT.TFile.Open(options.refmc)
        reftree = reffile.Get(options.tree)
    if options.cmode == "2D":
        ilc = ILC2D(tree, eval(options.xbins), eval(options.ybins), options)
        ilc_ref = ILC2D(reftree, eval(options.xbins), eval(options.ybins), options, weightedMC=True) if options.refmc else None
    ilc.go()        

