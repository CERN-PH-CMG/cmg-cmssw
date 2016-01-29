import ROOT
from math import *
import re, os
from array import array
from CMGTools.TTHAnalysis.tools.plotDecorations import doSpam

def makeSignalModel(model, w):
    if model == "Z-Voit":
        w.factory("Voigtian::signal(mass,sum::(dm[0,-10,10],MZ[91.1876]), GammaZ[2.495],sigma)")
        return (w.pdf("signal"), ["dm", "sigma"])
    if model == "Z-CB":
        w.factory("BreitWigner::zBW(mass,MZ[91.1876], GammaZ[2.495])")
        w.factory("CBShape::resol(mass,dm[0,-10,10], sigma, alpha[3., 0.5, 8], n[1, 0.1, 100.])")
        w.factory("FCONV::signal(mass,zBW,resol)")
        return (w.pdf("signal"), ["dm", "sigma","alpha", "n"] )
    if model == "Z-DCB":
        ROOT.gSystem.Load("libHiggsAnalysisCombinedLimit")
        w.factory("BreitWigner::zBW(mass,MZ[91.1876], GammaZ[2.495])")
        w.factory("DoubleCB::resol(mass,dm[0,-10,10], sigma, alpha[3., 0.5, 8], n[1, 0.1, 100.], alpha2[3., 0.5, 8], n2[1, 0.1, 100.])")
        w.factory("FCONV::signal(mass,zBW,resol)")
        return (w.pdf("signal"), ["dm", "sigma","alpha", "n","alpha2", "n2"] )
    raise RuntimeError, "missing signal model: "+model

def makeSignalModel1D(model, w):
    w.factory("sigma[1.5,0.3,10]")
    return makeSignalModel(model,w)

def makeSignalModel2D(model, w):
    #w.factory('expr::sigma("@0*exp(@1)",lambda[1,0.5,2.0], logdm)')
    w.factory('prod::sigma(lambda[1,0.5,2.0], massErr)')
    (pdf,params) = makeSignalModel(model,w)
    params[params.index("sigma")] = "lambda"
    return (pdf,params)


def makeBackgroundModel(model, w):
    if model == "Expo":
        w.factory("Exponential::background(mass,slope[-0.1,-1,0.1])")
        return (w.pdf("background"), [])
    if model == "None":
        return (None, [])
    raise RuntimeError, "missing signal model: "+model

def makeSumModel(spdf,bpdf,w):
    if not bpdf: return spdf
    w.factory("SUM::sumModel(f_signal[0.9,0.5,1]*%s, %s)" % (spdf.GetName(), bpdf.GetName()))
    return w.pdf("sumModel")

def printPlot(frame, name, text, options, spam=[]):
    c1 = ROOT.TCanvas("c1","c1")
    xoffs = 0;
    frame.SetMaximum(frame.GetMaximum()*(1.05+0.07*len(spam)))
    if frame.GetMaximum() >= 100*1000:
        c1.SetLeftMargin(0.21); xoffs = 0.155
        frame.Draw();
        frame.GetYaxis().SetTitleOffset(1.7)
    elif frame.GetMaximum() >= 1000:
        c1.SetLeftMargin(0.18); xoffs = 0.02
        frame.Draw()
        frame.GetYaxis().SetTitleOffset(1.45)
    else:
        c1.SetLeftMargin(0.16); # default
        frame.Draw()
        frame.GetYaxis().SetTitleOffset(1.25)
    c1.SetRightMargin(0.05); 
    frame.GetXaxis().SetNdivisions(505);   
    frame.GetXaxis().SetDecimals(1);   
    printCanvas(c1, name, text, options, xoffs=xoffs, spam=spam)

def printCanvas(c1, name, text, options, xoffs=0, spam=[]):
    if   options.lumi > 3.54e+1: lumitext = "%.0f fb^{-1}" % options.lumi
    elif options.lumi > 3.54e+0: lumitext = "%.1f fb^{-1}" % options.lumi
    elif options.lumi > 3.54e-1: lumitext = "%.2f fb^{-1}" % options.lumi
    elif options.lumi > 3.54e-2: lumitext = "%.0f pb^{-1}" % (options.lumi*1000)
    elif options.lumi > 3.54e-3: lumitext = "%.1f pb^{-1}" % (options.lumi*1000)
    else                       : lumitext = "%.2f pb^{-1}" % (options.lumi*1000)

    doSpam("#bf{CMS} #it{%s}" % ("Simulation" if "_ref" in name else "Preliminary"), 0.15+xoffs, 0.955, 0.55+xoffs, 0.998, align=12, textSize=options.textSize)
    doSpam(lumitext+" (13 TeV)", .55+xoffs, .955, .97, .998, align=32, textSize=options.textSize)

    if spam:
        y0 = 0.91 - options.textSize*1.2
        for line in spam:
            niceline = re.sub(r"(\s+)-(\d+)",r"\1#minus\2", line)
            doSpam(niceline, 0.50, y0, 0.90, y0 + options.textSize*1.2, textSize=options.textSize, align=22)
            y0 -= options.textSize * 1.2
    for ext in "png", "pdf":
        c1.Print("%s/%s.%s" % (options.printDir, name, ext))
    log = open("%s/%s.%s"  % (options.printDir, name, "txt"), "w")
    for line in text: log.write(line +"\n")
    log.close()


def makePlot1D(w, data, pdf, params, result, name, options):
    frame = w.var("mass").frame(ROOT.RooFit.Bins(w.var("mass").getBins()/options.rebin))
    data.plotOn(frame)
    if pdf.GetName() == "sumModel":
        pdf.plotOn(frame, ROOT.RooFit.Components("background"), ROOT.RooFit.LineStyle(2))
    pdf.plotOn(frame)
    text = []
    for param in params:
        var = result.floatParsFinal().find(param)
        text.append("%s: %.3f +- %.3f" % (param, var.getVal(), var.getError()))
    if "dm" in params and "sigma" in params:
        dm = result.floatParsFinal().find("dm")
        sm = result.floatParsFinal().find("sigma")
        spam = [ ("#Delta = %.2f #pm %.2f GeV" % (dm.getVal(), dm.getError())),
                 ("#sigma = %.2f #pm %.2f GeV" % (sm.getVal(), sm.getError())) ]
    else:
        spam = []
    if w.function("resol"):
        foms = getResolutionFOMs(w)
        for k,v in foms.iteritems(): 
            text.append("%s: %.3f" % (k,v))
    printPlot(frame,name,text,options, spam = spam)

class GioProjWData:
    def __init__(self, pdf, xvar, data, rebinFactor=1):
        self.xvar = xvar
        self.pdfOriginal = pdf
        self.dataOriginal = data
        ROOT.RooMsgService.instance().setGlobalKillBelow(ROOT.RooFit.WARNING);
        vars = ROOT.RooArgSet(self.dataOriginal.get());  
        vars.remove(xvar,False,True);
        pars = pdf.getParameters(ROOT.RooArgSet(xvar));
        hist = None;
        if data.ClassName() == "RooDataSet":
            others = ROOT.RooDataSet("","dm",data,vars);
        else:
            others = ROOT.RooDataHist("","dm",vars,data);
        print "Will have to average over %d entries " % others.numEntries();
        for i in xrange(others.numEntries()):
            pars.assignValueOnly(others.get(i))
            h = pdf.createHistogram("htemp",xvar,ROOT.RooFit.Binning(rebinFactor*xvar.getBins()));
            h.SetDirectory(0);
            h.Scale(others.weight()/h.Integral());
            if hist == None: hist = h 
            else:            hist.Add(h); 
        self.hist = hist
        self.rdh  = ROOT.RooDataHist("","",ROOT.RooArgList(xvar),self.hist);
        self.pdf  = ROOT.RooHistPdf("","",ROOT.RooArgSet(xvar),self.rdh,2);
    def plotOn(self, frame, *args):
        return self.pdf.plotOn(frame, *args)

def makePlot2D(w, data, pdf, params, result, name, options):
    frame = w.var("mass").frame(ROOT.RooFit.Bins(w.var("mass").getBins()/options.rebin))
    data.plotOn(frame)
    #if pdf.GetName() == "sumModel":
    #    pdf.plotOn(frame, ROOT.RooFit.Components("background"), ROOT.RooFit.LineStyle(2))
    proj = GioProjWData(pdf, w.var("mass"), data)
    proj.plotOn(frame)
    text = []
    for param in params:
        var = result.floatParsFinal().find(param)
        text.append("%s: %.3f +- %.3f" % (param, var.getVal(), var.getError()))
    if "dm" in params and "lambda" in params:
        dm = result.floatParsFinal().find("dm")
        sm = result.floatParsFinal().find("lambda")
        spam = [ ("#Delta = %.2f #pm %.2f GeV" % (dm.getVal(), dm.getError())),
                 ("#lambda = %.2f #pm %.2f GeV" % (sm.getVal(), sm.getError())) ]
    else:
        spam = []
    printPlot(frame,name,text,options, spam = spam)

def makePlot1DRef(w, data, pdf, pdfref, params, result, refresult, name, options):
    frame = w.var("mass").frame()
    data.plotOn(frame)
    if pdfref.GetName() == "sumModel" and pdf.GetName() == "sumModel":
        pdf.plotOn(frame, ROOT.RooFit.Components("background"), ROOT.RooFit.LineStyle(2))
        pdf.plotOn(frame)
        pdfref.plotOn(frame, ROOT.RooFit.LineColor(ROOT.kGray+0))
        pdfref.plotOn(frame, ROOT.RooFit.Components("signal"), ROOT.RooFit.LineColor(ROOT.kGray+2))
        pdf.plotOn(frame, ROOT.RooFit.Components("signal"), ROOT.RooFit.LineColor(ROOT.kGreen+2))
    text = []
    for param in params:
        var = result.floatParsFinal().find(param)
        ref = refresult.floatParsFinal().find(param)
        text.append("%s: fit %.3f +- %.3f ref %.3f +- %.3f  diff %.3f +- %.3f" % (param,
                        var.getVal(), var.getError(), ref.getVal(), ref.getError(),
                        var.getVal()-ref.getVal(), hypot(var.getError(), ref.getError())))
    if "dm" in params and "sigma" in params:
        fdm =    result.floatParsFinal().find("dm")
        fsm =    result.floatParsFinal().find("sigma")
        rdm = refresult.floatParsFinal().find("dm")
        rsm = refresult.floatParsFinal().find("sigma")
        spam = [ "#Delta - #Delta_{MC} = %.2f #pm %.2f GeV" % (
                        fdm.getVal() - rdm.getVal(), hypot(fdm.getError(), rdm.getError())),
                 "#sigma/#sigma_{MC} = %.2f #pm %.2f" % (
                         fsm.getVal()/rsm.getVal(), 
                        (fsm.getVal()/rsm.getVal())*hypot(fsm.getError()/fsm.getVal(), rsm.getError()/rsm.getVal())) ]
    else:
        spam = []
    printPlot(frame,name,text,options, spam=spam)


def fit1D(hist, options, modelOverride=False):
    w = ROOT.RooWorkspace("w")
    w.factory("mass[%g,%g]" % (hist.GetXaxis().GetXmin(), hist.GetXaxis().GetXmax()))
    w.var("mass").setBins(hist.GetNbinsX())
    w.var("mass").SetTitle(hist.GetXaxis().GetTitle())
    data = ROOT.RooDataHist("data","data", ROOT.RooArgList(w.var("mass")), hist)
    spdf, sparams = makeSignalModel1D(options.signalModel if not modelOverride else modelOverride, w)
    bpdf, bparams = makeBackgroundModel(options.backgroundModel, w) if options.backgroundModel else (None, [])
    pdf = makeSumModel(spdf, bpdf, w)
    result = pdf.fitTo(data, ROOT.RooFit.Save(True))
    return (w,data,pdf,sparams+bparams,result)

def getResolutionFOMs(w,pdf="resol",xvar="mass",xrange=[-10,10]):
    w2 = ROOT.RooWorkspace("w2")
    w2.factory("%s[0,%g,%g]" % (xvar, xrange[0], xrange[1]))
    getattr(w2,'import')(w.pdf(pdf), ROOT.RooFit.RecycleConflictNodes(), ROOT.RooFit.Silence(True))
    w2.var(xvar).setBins(2000)
    hist = w2.pdf(pdf).createHistogram("",w2.var(xvar)); hist.SetDirectory(None)
    imax = hist.GetMaximumBin()
    ymax = hist.GetBinContent(imax)
    ymid = 0.5*ymax
    ysum = ymax; y68 = 0.6827*hist.Integral()
    ihi = ilo = imax
    nbins = hist.GetNbinsX()
    hist.SetBinContent(0,0); hist.SetBinContent(nbins+1,0)
    sigmaEff = -1; fwhm = -1
    while ilo >= 1 or ihi <= nbins:
        ileft, iright = ilo-1, ihi+1
        yleft, yright = map(hist.GetBinContent, (ileft,iright))
        if ilo >= 1 and yleft >= yright:
            ysum += yleft
            ilo   = ileft
        else:
            ysum += yright
            ihi   = iright
        if ysum > y68 and sigmaEff < 0:
            sigmaEff = 0.5*abs(hist.GetXaxis().GetBinLowEdge(ilo) - hist.GetXaxis().GetBinUpEdge(ihi))
        if max(yleft,yright) < ymid and fwhm < 0:
            fwhm = abs(hist.GetXaxis().GetBinLowEdge(ilo) - hist.GetXaxis().GetBinUpEdge(ihi))/(2.*1.177)
    return { 'fwhm23':fwhm, 'sigmaEff':sigmaEff }
    
    
def fit2D(hist, options):
    w = ROOT.RooWorkspace("w")
    w.factory("mass[%g,%g]" % (hist.GetXaxis().GetXmin(), hist.GetXaxis().GetXmax()))
    w.factory("massErr[%g,%g]" % (hist.GetYaxis().GetXmin(), hist.GetYaxis().GetXmax()))
    w.var("mass").setBins(hist.GetNbinsX())
    w.var("massErr").setBins(hist.GetNbinsY())
    w.var("mass").SetTitle(hist.GetXaxis().GetTitle())
    w.var("massErr").SetTitle(hist.GetYaxis().GetTitle())
    data = ROOT.RooDataHist("data","data", ROOT.RooArgList(w.var("mass"),w.var("massErr")), hist)
    spdf, sparams = makeSignalModel2D(options.signalModel, w)
    bpdf, bparams = makeBackgroundModel(options.backgroundModel, w) if options.backgroundModel else (None, [])
    pdf = makeSumModel(spdf, bpdf, w)
    conditional = ROOT.RooFit.ConditionalObservables(ROOT.RooArgSet(w.var("massErr")))
    result = pdf.fitTo(data, ROOT.RooFit.Save(True), conditional)
    return (w,data,pdf,sparams+bparams,result)



def makeCut(cut):
    if cut == "Zee-BB":
        return "abs(z_l1_pdgId[0]) == 11 && max(abs(z_l1_eta[0]),abs(z_l2_eta[0])) < 1.479"
    elif cut == "Zee-NonBB":
        return "abs(z_l1_pdgId[0]) == 11 && max(abs(z_l1_eta[0]),abs(z_l2_eta[0])) > 1.479"
    elif cut == "Zee-BE":
        return "abs(z_l1_pdgId[0]) == 11 && max(abs(z_l1_eta[0]),abs(z_l2_eta[0])) > 1.479 && min(abs(z_l1_eta[0]),abs(z_l2_eta[0])) < 1.479"
    elif cut == "Zee-EE":
        return "abs(z_l1_pdgId[0]) == 11 && min(abs(z_l1_eta[0]),abs(z_l2_eta[0])) > 1.479"
    elif cut == "Zee":
        return "abs(z_l1_pdgId[0]) == 11"
    elif cut == "Zee-dm132":
        return "abs(z_l1_pdgId[0]) == 11 && 1.3 < 0.5*z_mass[0]*TMath::Hypot(z_l1_ptErr[0]/z_l1_pt[0], z_l2_ptErr[0]/z_l2_pt[0]) && 0.5*z_mass[0]*TMath::Hypot(z_l1_ptErr[0]/z_l1_pt[0], z_l2_ptErr[0]/z_l2_pt[0]) < 2.00"
    elif cut == "Zmm-BB":
        return "abs(z_l1_pdgId[0]) == 13 && max(abs(z_l1_eta[0]),abs(z_l2_eta[0])) < 1.2"
    elif cut == "Zmm-NonBB":
        return "abs(z_l1_pdgId[0]) == 13 && max(abs(z_l1_eta[0]),abs(z_l2_eta[0])) > 1.2"
    elif cut == "Zmm-BE":
        return "abs(z_l1_pdgId[0]) == 13 && max(abs(z_l1_eta[0]),abs(z_l2_eta[0])) > 1.2 && min(abs(z_l1_eta[0]),abs(z_l2_eta[0])) < 1.2"
    elif cut == "Zmm-EE":
        return "abs(z_l1_pdgId[0]) == 13 && min(abs(z_l1_eta[0]),abs(z_l2_eta[0])) > 1.2"
    elif cut == "Zmm":
        return "abs(z_l1_pdgId[0]) == 13"
    elif cut == "Zmm-B0E1":
        return "abs(z_l1_pdgId[0]) == 13 && max(abs(z_l1_eta[0]),abs(z_l2_eta[0])) > 1.2 && max(abs(z_l1_eta[0]),abs(z_l2_eta[0])) < 1.5 && min(abs(z_l1_eta[0]),abs(z_l2_eta[0])) < 1.0"
    elif cut == "Zmm-B0E2":
        return "abs(z_l1_pdgId[0]) == 13 && max(abs(z_l1_eta[0]),abs(z_l2_eta[0])) > 1.5 && max(abs(z_l1_eta[0]),abs(z_l2_eta[0])) < 2.0 && min(abs(z_l1_eta[0]),abs(z_l2_eta[0])) < 1.0"
    elif cut == "Zmm-B0E3":
        return "abs(z_l1_pdgId[0]) == 13 && max(abs(z_l1_eta[0]),abs(z_l2_eta[0])) > 2.0 && max(abs(z_l1_eta[0]),abs(z_l2_eta[0])) < 2.4 && min(abs(z_l1_eta[0]),abs(z_l2_eta[0])) < 1.0"
    else:
        return cut

def makeHist1D(tree, options, weightedMC = False):
    print "Filling tree for cut %s" % options.cut
    finalCut =  makeCut(options.cut)
    if weightedMC: finalCut = "(%s) * (genWeight/abs(genWeight)) " % finalCut
    tree.Draw("%s>>hist(%s)" % (options.xvar[0], options.xvar[1]), finalCut, "goff", options.maxEntries)
    print "Done, fitting."
    hist = ROOT.gROOT.FindObject("hist")
    if weightedMC:
        for b in xrange(hist.GetNbinsX()+2): 
            hist.SetBinContent(b, max(0, hist.GetBinContent(b)))
    hist.GetXaxis().SetTitle(options.xtitle)
    return hist

def makeHist2D(tree, options, weightedMC = False, 
               dmvar = "0.5*z_mass[0]*TMath::Hypot(z_l1_ptErr[0]/z_l1_pt[0], z_l2_ptErr[0]/z_l2_pt[0])",
               dmbins = 50, dmrange=[0.3,5]):
    print "Filling tree for cut %s" % options.cut
    finalCut =  makeCut(options.cut)
    if weightedMC: finalCut = "(%s) * (genWeight/abs(genWeight)) " % finalCut
    tree.Draw("%s:%s>>hist(%s,%d,%g,%g)" % (dmvar, options.xvar[0], options.xvar[1], dmbins,dmrange[0],dmrange[1]), finalCut, "goff", options.maxEntries)
    hist = ROOT.gROOT.FindObject("hist")
    if weightedMC:
        for bx in xrange(hist.GetNbinsX()+2): 
          for by in xrange(hist.GetNbinsY()+2): 
            hist.SetBinContent(bx,by, max(0, hist.GetBinContent(bx,by)))
    return hist

def makeHist1D_DMSlices(tree, options, weightedMC = False, 
        dmvar = "0.5*z_mass[0]*TMath::Hypot(z_l1_ptErr[0]/z_l1_pt[0], z_l2_ptErr[0]/z_l2_pt[0])",
        dmbins = 25, dmrange=[0.5,3.5]):
    hist = makeHist2D(tree,options,weightedMC=weightedMC, dmvar=dmvar, dmbins=dmbins, dmrange=dmrange)
    print "Splitting."
    ret = []; xbins, xmin, xmax = map(float, options.xvar[1].split(",")) 
    for b in xrange(1,dmbins+1):
        hist1D = ROOT.TH1D("hist%d" % b,"hist",int(xbins),xmin,xmax)
        hist1D.SetDirectory(None)
        for bx in xrange(1,hist.GetNbinsX()+1):
            hist1D.SetBinContent(bx, hist.GetBinContent(bx,b))
        hist1D.dm = [ hist.GetYaxis().GetBinLowEdge(b), hist.GetYaxis().GetBinUpEdge(b) ]
        if options.xcut and (hist1D.dm[1] < options.xcut[0] or hist1D.dm[0] > options.xcut[1]):
            continue
        ret.append(hist1D)
    return ret


def processOneDMSlice(args):
    i,hist,dm,refhist,myparams,options = args
    res = {}; refres = {}
    name = options.name+"_dm_%.2f_%.2f" % (dm[0], dm[1])
    (w,data,pdf, params, result) = fit1D(hist, options)
    makePlot1D(w, data, pdf, params, result, name, options)
    for x in myparams:
        var = result.floatParsFinal().find(x)
        res[x] = ( 0.5*(dm[0] + dm[1]), 0.5*(dm[1] - dm[0]), 
                   var.getVal(), var.getError() )
    if w.function("resol"):
        foms = getResolutionFOMs(w)
        for k,v in foms.iteritems(): res[k] = v
    if refhist:
        (wref,dataref, pdfref, params, refresult) = fit1D(refhist, options)
        makePlot1D(wref, dataref, pdfref, params, refresult, name+"_ref", options)
        makePlot1DRef(w, data, pdf, pdfref, params, result, refresult, name+"_comp", options)
        for x in myparams:
            var = refresult.floatParsFinal().find(x)
            refres[x] = ( 0.5*(dm[0] + dm[1]), 0.5*(dm[1] - dm[0]), 
                          var.getVal(), var.getError() )
        if w.function("resol"):
            foms = getResolutionFOMs(wref)
            for k,v in foms.iteritems(): refres[k] = v
    return (i,res,refres)

def processOnePtEtaBin(args):
    bin,hist,refhist,myparams,options = args
    res = {}; refres = {}
    name = hist.GetName().replace("hist_",options.name+"_")
    (w,data,pdf, params, result) = fit1D(hist, options)
    makePlot1D(w, data, pdf, params, result, name, options)
    for x in myparams:
        var = result.floatParsFinal().find(x)
        res[x] = ( var.getVal(), var.getError() )
    if refhist:
        (wref,dataref, pdfref, params, refresult) = fit1D(refhist, options)
        makePlot1D(wref, dataref, pdfref, params, refresult, name+"_ref", options)
        makePlot1DRef(w, data, pdf, pdfref, params, result, refresult, name+"_comp", options)
        for x in myparams:
            var = refresult.floatParsFinal().find(x)
            refres[x] = ( var.getVal(), var.getError() )
    return (bin,res,refres)

def makeHist3D_(tree, y, z, options, weightedMC = False):
    print "Filling tree for cut %s" % options.cut
    finalCut =  makeCut(options.cut)
    if weightedMC: finalCut = "(%s) * (genWeight/abs(genWeight)) " % finalCut
    yexpr, yedges = y
    zexpr, zedges = z
    xbins, xmin, xmax = map(float, options.xvar[1].split(","))
    xedges = [ xmin + i * (xmax-xmin)/int(xbins) for i in xrange(0,int(xbins)+1) ]
    hist = ROOT.TH3D("hist","hist", len(xedges)-1,array('f',xedges),  len(yedges)-1,array('f',yedges),  len(zedges)-1,array('f',zedges))
    nent = tree.Draw("%s:%s:%s>>hist" % (zexpr,yexpr,options.xvar[0]), finalCut, "goff", options.maxEntries)
    hist = ROOT.gROOT.FindObject("hist")
    if weightedMC:
        for bx in xrange(hist.GetNbinsX()+2): 
          for by in xrange(hist.GetNbinsY()+2): 
             for bz in xrange(hist.GetNbinsZ()+2): 
                hist.SetBinContent(bx,by,bz, max(0, hist.GetBinContent(bx,by,bz)))
    return hist

def makeHistsMPtEta(tree, ptedges, etaedges, options,  weightedMC = False):
    hl1 = makeHist3D_(tree, ("z_l1_pt[0]", ptedges), ("abs(z_l1_eta[0])", etaedges), options, weightedMC=weightedMC)
    hl1 = hl1.Clone("hist_l1");
    hl2 = makeHist3D_(tree, ("z_l2_pt[0]", ptedges), ("abs(z_l2_eta[0])", etaedges), options, weightedMC=weightedMC)
    hl2 = hl2.Clone("hist_l2");
    hl1.Scale(0.5);
    hl2.Scale(0.5);
    hl1.Add(hl2)
    xbins, xmin, xmax = map(float, options.xvar[1].split(","))
    ret = []
    for ipt in xrange(1,len(ptedges)):
        for ieta in xrange(1,len(etaedges)):
            name = "hist_pt_%.1f_%.1f_eta_%.3f_%.3f" % (ptedges[ipt-1],ptedges[ipt],etaedges[ieta-1],etaedges[ieta])
            hist1D = ROOT.TH1D(name,name, int(xbins),xmin,xmax)
            hist1D.SetDirectory(None)
            for bx in xrange(1,hist1D.GetNbinsX()+1):
                hist1D.SetBinContent(bx, hl1.GetBinContent(bx,ipt,ieta))
            hist1D.bin = (ipt,ieta)
            ret.append(hist1D)
    h2d = hl1.Project3D("yz")
    return (h2d,ret)
            
def styleScatterData(gdata,gmc=None):
        gdata.SetMarkerStyle(ROOT.kFullCircle)
        gdata.SetMarkerColor(ROOT.kGreen+2)
        gdata.SetLineColor(ROOT.kGreen+2)
def styleScatterMC(gmc):
        gmc.SetMarkerStyle(ROOT.kOpenCircle)
        gmc.SetMarkerColor(ROOT.kGray+3)
        gmc.SetLineColor(ROOT.kGray+3)
 
def addZFitterOptions(parser):
    parser.add_option("-n", "--name",   dest="name", default='plot', help="name");
    parser.add_option("-r", "--refmc",   dest="refmc", default=None, help="refmc");
    parser.add_option("-m", "--mode",   dest="mode", default='1D', help="mode");
    parser.add_option("-s", "--signalModel",   dest="signalModel", default='Z-Voit', help="Signal model");
    parser.add_option("-b", "--backgroundModel",   dest="backgroundModel", default='Expo', help="Background model");
    parser.add_option("-t", "--tree",    dest="tree", default='tree', help="Tree name");
    parser.add_option("-c", "--cut",     dest="cut", type="string", default="z_mass[0] > 0", help="cut")
    parser.add_option("--xcut",     dest="xcut", type="float", nargs=2, default=None, help="x axis cut")
    parser.add_option("-x", "--x-var",   dest="xvar", type="string", default=("z_mass[0]","80,70,110"), nargs=2, help="X var and bin")
    parser.add_option("--xtitle",   dest="xtitle", type="string", default="mass (GeV)", help="X title")
    parser.add_option("--textSize",   dest="textSize", type="float", default=0.04, help="Text size")
    parser.add_option("-l","--lumi",   dest="lumi", type="float", default=1.54, help="Text size")
    parser.add_option("--pdir", "--print-dir", dest="printDir", type="string", default="plots", help="print out plots in this directory");
    parser.add_option("--max-entries",     dest="maxEntries", default=1000000000, type="int", help="Max entries to process in each tree") 
    parser.add_option("-j", "--jobs",    dest="jobs",      type="int",    default=0, help="Use N threads");
    parser.add_option("--rebin",    dest="rebin",      type="int",    default=1, help="Rebin mass plots by N");


if __name__ == "__main__":
    from optparse import OptionParser
    parser = OptionParser(usage="%prog [options] tree [tree2] ")
    addZFitterOptions(parser)
    (options, args) = parser.parse_args()
    if len(args) == 0:
        print "You must specify at least one tree to fit"
    ROOT.gROOT.SetBatch(True)
    ROOT.gROOT.ProcessLine(".x ~/cpp/tdrstyle.cc")
    if not os.path.exists(options.printDir):
        os.system("mkdir -p "+options.printDir)
        if os.path.exists("/afs/cern.ch"): os.system("cp /afs/cern.ch/user/g/gpetrucc/php/index.php "+options.printDir)
    tree = ROOT.TChain(options.tree)
    for fname in args: tree.Add(fname)
    if options.refmc:
        reffile = ROOT.TFile.Open(options.refmc)
        reftree = reffile.Get(options.tree)
    if options.mode == "1D":
        hist =   makeHist1D(tree, options)
        (w,data,pdf, params, result) = fit1D(hist, options)
        makePlot1D(w, data, pdf, params, result, options.name, options)
        if options.refmc:
            refhist = makeHist1D(reftree, options, weightedMC=True)
            (wref,dataref, pdfref, params, refresult) = fit1D(refhist, options)
            makePlot1D(wref, dataref, pdfref, params, refresult, options.name+"_ref", options)
            makePlot1DRef(w, data, pdf, pdfref, params, result, refresult, options.name+"_comp", options)
    elif options.mode == "2D":
        hist =   makeHist2D(tree, options)
        (w,data,pdf, params, result) = fit2D(hist, options)
        makePlot2D(w, data, pdf, params, result, options.name, options)
        if options.refmc:
            refhist = makeHist2D(reftree, options, weightedMC=True)
            (wref,dataref, pdfref, params, refresult) = fit2D(refhist, options)
            makePlot2D(wref, dataref, pdfref, params, refresult, options.name+"_ref", options)
        #    makePlot1DRef(w, data, pdf, pdfref, params, result, refresult, options.name+"_comp", options)
    elif options.mode == "1D_DMSlices":
        hists =   makeHist1D_DMSlices(tree, options)
        if options.refmc:
            refhists = makeHist1D_DMSlices(reftree, options, weightedMC=True)
        else: 
            refhists = [None for h in hists]
        myparams = [ "dm", "sigma" ]
        tasks = [ (i,hist,hist.dm,refhist,myparams,options) for i,(hist, refhist) in enumerate(zip(hists,refhists)) ]
        if options.jobs > 0:
            from multiprocessing import Pool
            fits = Pool(options.jobs).map(processOneDMSlice, tasks)
        else:
            fits = map(processOneDMSlice, tasks)
        # strip clearly bad fits
        fits = [ (i,res,refres) for (i,res,refres) in fits if abs(res["dm"][2]) < 3 ]
        if len(refres):
            fits = [ (i,res,refres) for (i,res,refres) in fits if abs(refres["dm"][2]) < 3 ]
        gdata = dict([ (x,ROOT.TGraphErrors(len(fits))) for x in myparams ])
        gref  = dict([ (x,ROOT.TGraphErrors(len(fits))) for x in myparams ])
        for (i, res, refres) in fits:
           for x in myparams:
                gdata[x].SetPoint(i,      res[x][0], res[x][2] )
                gdata[x].SetPointError(i, res[x][1], res[x][3] )
           if len(refres):
               for x in myparams:
                    gref[x].SetPoint(i,      refres[x][0], refres[x][2] )
                    gref[x].SetPointError(i, refres[x][1], refres[x][3] )
        c1 = ROOT.TCanvas("c1","c1")
        xmax = max(hist.dm[1] for hist in hists) 
        framedm = ROOT.TH2D("frame",";#sigma(pred);#sigma(fit)",100,0,xmax*1.3,100,0,xmax*1.3)
        framedm.Draw(); ROOT.gStyle.SetOptStat(0)
        line = ROOT.TLine(0,0,xmax*1.3,xmax*1.3)
        line.SetLineStyle(2)
        line.SetLineWidth(2)
        line.DrawLine(0,0,xmax*1.3,xmax*1.3)
        line.SetLineColor(ROOT.kRed+1)
        line.DrawLine(0,0,xmax*1.3/1.2,xmax*1.3    )
        line.DrawLine(0,0,xmax*1.3    ,xmax*1.3/1.2)
        line.SetLineWidth(1)
        line.SetLineColor(ROOT.kOrange+7)
        line.DrawLine(0,0,xmax*1.3/1.1,xmax*1.3    )
        line.DrawLine(0,0,xmax*1.3    ,xmax*1.3/1.1)
        if refhists[0] != None:
            styleScatterMC(gref["sigma"])
            gref["sigma"].Draw("P SAME")
        styleScatterData(gdata["sigma"])
        gdata["sigma"].Draw("P SAME")
        printCanvas(c1, options.name+"_summary", [], options)
        if "sigmaEff" in fits[0][1]:
            framedm.GetYaxis().SetTitle("#sigma_{eff}(fit)")
            for (i, res, refres) in fits:
                gdata["sigma"].SetPoint(i,      res["sigma"][0], res["sigmaEff"] )
                gdata["sigma"].SetPointError(i, res["sigma"][1], res["sigmaEff"]*res["sigma"][3]/res["sigma"][2] )
                if refhists[0] != None:
                    gref["sigma"].SetPoint(i,      refres["sigma"][0], refres["sigmaEff"] )
                    gref["sigma"].SetPointError(i, refres["sigma"][1], refres["sigmaEff"]*refres["sigma"][3]/refres["sigma"][2] )
            framedm.Draw(); ROOT.gStyle.SetOptStat(0)
            line = ROOT.TLine(0,0,xmax*1.3,xmax*1.3)
            line.SetLineStyle(2)
            line.SetLineWidth(2)
            line.DrawLine(0,0,xmax*1.3,xmax*1.3)
            line.SetLineColor(ROOT.kRed+1)
            line.DrawLine(0,0,xmax*1.3/1.2,xmax*1.3    )
            line.DrawLine(0,0,xmax*1.3    ,xmax*1.3/1.2)
            line.SetLineWidth(1)
            line.SetLineColor(ROOT.kOrange+7)
            line.DrawLine(0,0,xmax*1.3/1.1,xmax*1.3    )
            line.DrawLine(0,0,xmax*1.3    ,xmax*1.3/1.1)
            if refhists[0] != None:
                gref["sigma"].Draw("P SAME")
            gdata["sigma"].Draw("P SAME")
            printCanvas(c1, options.name+"_summary_eff", [], options)
    elif options.mode == "1D_PtEtaSlices":
        ptbins = [10,20,35,45,60,90]; 
        etabins = [0, 1.5, 2.5] if "Zee" in options.cut else [0, 1.2, 2.4]
        #ptbins = [5,35,120]; etabins = [0, 1.5, 2.5]
        frame2D, hists = makeHistsMPtEta(tree, ptbins, etabins, options)
        if options.refmc:
            _, refhists = makeHistsMPtEta(reftree, ptbins, etabins, options, weightedMC=True)
        else: 
            refhists = [None for h in hists]
        myparams = [ "dm", "sigma" ]
        tasks = [ (hist.bin,hist,refhist,myparams,options) for (hist, refhist) in zip(hists,refhists) ]
        if options.jobs > 0:
            from multiprocessing import Pool
            fits = Pool(options.jobs).map(processOnePtEtaBin, tasks)
        else:
            fits = map(processOnePtEtaBin, tasks)
        # strip clearly bad fits
        fits = [ (i,res,refres) for (i,res,refres) in fits if abs(res["dm"][0]) < 3 ]
        if options.refmc:
            fits = [ (i,res,refres) for (i,res,refres) in fits if abs(refres["dm"][0]) < 3 ]
        # get bin labels from hist names
        binLabel = {}; binIndex = {}
        for i,hist in enumerate(hists):
            ptmin,ptmax,etamin,etamax = ptbins[hist.bin[0]-1],ptbins[hist.bin[0]],etabins[hist.bin[1]-1],etabins[hist.bin[1]]
            binIndex[hist.bin] = i
            binLabel[hist.bin] = "#splitline{%g < p_{T} < %g}{%g < |#eta| < %g}" % (ptmin,ptmax,etamin,etamax)
        frame2D.GetXaxis().SetNdivisions(505)
        frame2D.SetMarkerSize(2.0)
        # make graphs
        gdata = dict([ (x,ROOT.TGraphErrors(len(fits))) for x in myparams ])
        gref  = dict([ (x,ROOT.TGraphErrors(len(fits))) for x in myparams ])
        gdiff = dict([ (x,ROOT.TGraphErrors(len(fits))) for x in myparams ])
        # make 2Ds
        hdata = dict([ (x,frame2D.Clone(options.name+"_"+x        )) for x in myparams ])
        href  = dict([ (x,frame2D.Clone(options.name+"_"+x+"_ref" )) for x in myparams ])
        hdiff = dict([ (x,frame2D.Clone(options.name+"_"+x+"_diff")) for x in myparams ])
        text = dict([ (x,[]) for x in myparams ])
        for i,(bin, res, refres) in enumerate(fits):
           header = hists[binIndex[bin]].GetName().replace("hist_","").replace("_"," ")
           for x in myparams:
                gdata[x].SetPoint(     i, res[x][0], binIndex[bin]+0.6 )
                gdata[x].SetPointError(i, res[x][1],  0                )
                hdata[x].SetBinContent(bin[1],bin[0], res[x][0])
                hdata[x].SetBinError(  bin[1],bin[0], res[x][1])
           if options.refmc:
               for x in myparams:
                    gref[x].SetPoint(     i, refres[x][0], binIndex[bin]+0.4 )
                    gref[x].SetPointError(i, refres[x][1],  0                )
                    href[x].SetBinContent(bin[1],bin[0], refres[x][0] )
                    href[x].SetBinError(  bin[1],bin[0], refres[x][1] )
                    diff = res[x][0] - refres[x][0] if x != "sigma" else res[x][0]/refres[x][0]
                    err  = hypot(res[x][1],refres[x][1]) if x != "sigma" else diff*hypot(res[x][1]/res[x][0],refres[x][1]/refres[x][0])
                    gdiff[x].SetPoint(i,      diff, binIndex[bin]+0.5)
                    gdiff[x].SetPointError(i, err , 0.0)
                    hdiff[x].SetBinContent(bin[1],bin[0], diff )
                    hdiff[x].SetBinError(  bin[1],bin[0], err  )
                    text[x].append("%s    %+5.3f +- %5.3f     %+5.3f +- %5.3f     %+5.3f +- %5.3f" % ( 
                                        header, res[x][0], res[x][1], refres[x][0], refres[x][1], diff, err))
           else:
              for x in myparams:
                   text[x].append("%s    %+5.3f +- %5.3f " % (header, res[x][0], res[x][1]))
        c1 = ROOT.TCanvas("c1","c1")
        c1.SetRightMargin(0.2)
        ROOT.gStyle.SetOptStat(0)
        ROOT.gStyle.SetPalette(1);
        ROOT.gStyle.SetPaintTextFormat(".2f")
        def normalizeZ(h,pivot=0):
            swing = max(abs(h.GetMaximum()-pivot), abs(h.GetMinimum()-pivot))
            h.GetZaxis().SetRangeUser(pivot-1.5*swing,pivot+1.5*swing)
        for x in myparams:
            if x == "dm": normalizeZ(hdata[x])
            hdata[x].SetContour(100)
            hdata[x].Draw("COLZ0 TEXTE")
            printCanvas(c1, hdata[x].GetName(), text[x], options)
            if options.refmc:
                if x == "dm": normalizeZ(href[x])
                href[x].SetContour(100)
                href[x].Draw("COLZ0 TEXTE")
                printCanvas(c1, href[x].GetName(), text[x], options)
                normalizeZ(hdiff[x], pivot = 1 if x == "sigma" else 0)
                hdiff[x].SetContour(100)
                hdiff[x].Draw("COLZ0 TEXTE")
                printCanvas(c1, hdiff[x].GetName(), text[x], options)
        c1 = ROOT.TCanvas("c1","c1")
        c1.SetLeftMargin(0.35)
        for x in myparams:
            xmax = max(g[x].GetX()[i] + 1.3*g[x].GetErrorX(i) for g in (gdata,gref) for i in xrange(len(fits)))
            xmin = min(g[x].GetX()[i] - 1.3*g[x].GetErrorX(i) for g in (gdata,gref) for i in xrange(len(fits)))
            dx = 0.1*(xmax-xmin)
            frame = ROOT.TH2D("frame","", 100, xmin-dx, xmax+dx, len(hists), 0., len(hists))
            frame.GetXaxis().SetTitle({'dm':"#Delta m  (GeV)",'sigma':'#sigma(m)  (GeV)'}[x]);
            frame.GetXaxis().SetNdivisions(505)
            for bin,label in binLabel.iteritems():
                frame.GetYaxis().SetBinLabel(binIndex[bin]+1, label)
            frame.Draw()
            if options.refmc:
                styleScatterMC(gref[x])
                gref[x].Draw("PZ SAME")
            styleScatterData(gdata[x])
            gdata[x].Draw("PZ SAME")
            printCanvas(c1, options.name+"_"+x+"_summary", text[x], options)
            if options.refmc:
                xmax = max(g[x].GetX()[i] + 1.3*g[x].GetErrorX(i) for g in (gdiff,) for i in xrange(len(fits)))
                xmin = min(g[x].GetX()[i] - 1.3*g[x].GetErrorX(i) for g in (gdiff,) for i in xrange(len(fits)))
                dx = 0.1*(xmax-xmin)
                frame = ROOT.TH2D("frame","", 100, xmin-dx, xmax+dx, len(hists), 0., len(hists))
                frame.GetXaxis().SetTitle({'dm':"#Delta - #Delta_{MC}  (GeV)",'sigma':'#sigma/#sigma_{MC}'}[x]);
                frame.GetXaxis().SetNdivisions(505)
                for bin,label in binLabel.iteritems():
                    frame.GetYaxis().SetBinLabel(binIndex[bin]+1, label)
                frame.Draw()
                styleScatterData(gdiff[x])
                gdiff[x].Draw("PZ SAME")
                printCanvas(c1, options.name+"_"+x+"_diff_summary", text[x], options)

 
        
        

