import ROOT
from math import *
import re
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
    elif cut == "Zmm-BB":
        return "abs(z_l1_pdgId[0]) == 13 && max(abs(z_l1_eta[0]),abs(z_l2_eta[0])) < 1.2"
    elif cut == "Zmm-NonBB":
        return "abs(z_l1_pdgId[0]) == 13 && max(abs(z_l1_eta[0]),abs(z_l2_eta[0])) > 1.2"
    elif cut == "Zmm":
        return "abs(z_l1_pdgId[0]) == 13"
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
          for by in xrange(hist.GetNbinsX()+2): 
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
    if refhist:
        (wref,dataref, pdfref, params, refresult) = fit1D(refhist, options)
        makePlot1D(wref, dataref, pdfref, params, refresult, name+"_ref", options)
        makePlot1DRef(w, data, pdf, pdfref, params, result, refresult, name+"_comp", options)
        for x in myparams:
            var = refresult.floatParsFinal().find(x)
            refres[x] = ( 0.5*(dm[0] + dm[1]), 0.5*(dm[1] - dm[0]), 
                          var.getVal(), var.getError() )
    return (i,res,refres)

def addZFitterOptions(parser):
    parser.add_option("-n", "--name",   dest="name", default='plot', help="name");
    parser.add_option("-r", "--refmc",   dest="refmc", default=None, help="refmc");
    parser.add_option("-m", "--mode",   dest="mode", default='1D', help="mode");
    parser.add_option("-s", "--signalModel",   dest="signalModel", default='Z-Voit', help="Signal model");
    parser.add_option("-b", "--backgroundModel",   dest="backgroundModel", default='Expo', help="Background model");
    parser.add_option("-t", "--tree",    dest="tree", default='tree', help="Tree name");
    parser.add_option("-c", "--cut",     dest="cut", type="string", default="z_mass[0] > 0", help="cut")
    parser.add_option("-x", "--x-var",   dest="xvar", type="string", default=("z_mass[0]","80,70,110"), nargs=2, help="X var and bin")
    parser.add_option("--xtitle",   dest="xtitle", type="string", default="mass (GeV)", help="X title")
    parser.add_option("--textSize",   dest="textSize", type="float", default=0.04, help="Text size")
    parser.add_option("--lumi",   dest="lumi", type="float", default=0.209, help="Text size")
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
        gdata = dict([ (x,ROOT.TGraphErrors(len(hists))) for x in myparams ])
        gref  = dict([ (x,ROOT.TGraphErrors(len(hists))) for x in myparams ])
        tasks = [ (i,hist,hist.dm,refhist,myparams,options) for i,(hist, refhist) in enumerate(zip(hists,refhists)) ]
        if options.jobs > 0:
            from multiprocessing import Pool
            fits = Pool(options.jobs).map(processOneDMSlice, tasks)
        else:
            fits = map(processOneDMSlice, tasks)
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
        line.Draw()
        line.DrawLine(0,0,xmax*1.3/1.2,xmax*1.3    )
        line.DrawLine(0,0,xmax*1.3    ,xmax*1.3/1.2)
        if refhists[0] != None:
            gref["sigma"].SetMarkerStyle(ROOT.kOpenCircle)
            gref["sigma"].SetMarkerColor(ROOT.kGray+3)
            gref["sigma"].SetLineColor(ROOT.kGray+3)
            gref["sigma"].Draw("P SAME")
        gdata["sigma"].SetMarkerStyle(ROOT.kFullCircle)
        gdata["sigma"].Draw("P SAME")
        gdata["sigma"].SetMarkerColor(ROOT.kGreen+2)
        gdata["sigma"].SetLineColor(ROOT.kGreen+2)
        printCanvas(c1, options.name+"_summary", [], options)
        

