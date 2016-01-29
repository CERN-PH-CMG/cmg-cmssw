import ROOT, os
from array import array
from math import sqrt, hypot, log


def hypsub(all,stat):
    return sqrt(max(all*all-stat*stat,0))

def uniformAxis_(expr):
    (bins,xmin,xmax) = map(float,expr.split(","))
    dx = (xmax-xmin)/int(bins)
    return [ (xmin + dx*i) for i in xrange(0,int(bins)+1) ]


def makeSignalModel(w, model, refhpass, refhfail, options):
    if model == None: model = getattr(options, "sigModel", "voigt")
    if model == "voigt":
        w.factory("Voigtian::signal(mass, mean[90,80,100], width[2.495], sigma[3,1,10])")
        return ("signal","signal")
    elif model == "dvoigt":
        w.factory("Voigtian::signal1(mass, mean1[90,80,100], width[2.495], sigma1[2,1,3])")
        w.factory("Voigtian::signal2(mass, mean2[90,80,100], width,        sigma2[4,2,10])")
        w.factory("SUM::signal(vFrac[0.8,0,1]*signal1, signal2)")
        return ("signal","signal")
    elif model == "BWCB":
        w.factory("BreitWigner::zBW(mass,MZ[91.1876], GammaZ[2.495])")
        w.factory("CBShape::resol(mass,dm[0,-4,4], sigma[3,1,10], alpha[3., 0.5, 8], n[1,0.1,25])")
        w.factory("FCONV::signal(mass,zBW,resol)")
        return ("signal","signal")
    elif model == "BWDCB":
        w.factory("BreitWigner::zBW(mass,MZ[91.1876], GammaZ[2.495])")
        w.factory("DoubleCB::resol(mass,dm[0,-4,4], sigma[3,1,10], alpha[3., 0.5, 8], n[1,0.1,25], alpha2[3., 0.5, 8], n2[1,0.1,100])")
        w.factory("FCONV::signal(mass,zBW,resol)")
        return ("signal","signal")
    elif model == "BWDCB2":
        w.factory("BreitWigner::zBW(mass,MZ[91.1876], GammaZ[2.495])")
        w.factory("DoubleCB::resol(mass,dm[0,-4,4], sigma[3,1,10], alpha[3., 0.5, 8], n[1,0.1,25], alpha2[3., 0.5, 8], n2[1,0.1,100])")
        w.factory("DoubleCB::resol2(mass,sum::dm2(dm,dmfail[0,-3,3]), prod::sigma2(sigma,sigmafail[1,0.8,2]), alpha, n, alpha2, n2)")
        w.factory("FCONV::signal(mass,zBW,resol)")
        w.factory("FCONV::signal2(mass,zBW,resol2)")
        return ("signal","signal2")
    elif model in ("MCT","MCTG","MCTCB","MCTDCB"):
        if refhfail.Integral() <= 10 or refhpass.Integral() <= 10:
            return None
        print "Creating %s templates with %.2f pass, %.2f fail" % (model, refhpass.Integral(), refhfail.Integral())
        if refhfail.Integral() < 1000:
            refhfail.Smooth(2)
        elif refhfail.Integral() < 10000:
            refhfail.Smooth()
        tpass = ROOT.RooDataHist("tpass","tpass", ROOT.RooArgList(w.var("mass")), refhpass)
        tfail = ROOT.RooDataHist("tfail","tfail", ROOT.RooArgList(w.var("mass")), refhfail)
        spass = ROOT.RooHistPdf("sigPassT","hpass", ROOT.RooArgSet(w.var("mass")), tpass)
        sfail = ROOT.RooHistPdf("sigFailT","hfail", ROOT.RooArgSet(w.var("mass")), tfail)
        getattr(w,'import')(spass, ROOT.RooCmdArg())
        getattr(w,'import')(sfail, ROOT.RooCmdArg())
        if model == "MCT":
            return ("sigPassT","sigFailT")
        elif model == "MCTG":
           w.factory("Gaussian::resol(mass,dm[0,-4,4], sigma[0.1,0.01,5])")
        elif model == "MCTCB":
            w.factory("CBShape::resol(mass,dm[0,-4,4], sigma[0.1,0.01,10], alpha[3., 0.5, 8], n[1,0.1,25])")
        elif model == "MCTDCB":
            w.factory("DoubleCB::resol(mass,dm[0,-4,4], sigma[0.1,0.01,10], alpha[3., 0.5, 8], n[1,0.1,25], alpha2[3., 0.5, 8], n2[1,0.1,100])")
        w.factory("FCONV::sigPassTS(mass,sigPassT,resol)")
        w.factory("FCONV::sigFailTS(mass,sigFailT,resol)")
        return ("sigPassTS","sigFailTS")


    raise RuntimeError, "Unknown signal model %s" % model

def makeBackgroundModel(w, model, options):
    if model == None: model = getattr(options, "bkgModel", "expo")
    if model == "expo":
        w.factory("Exponential::backgroundPass(mass, lp[0,-5,5])")
        w.factory("Exponential::backgroundFail(mass, lf[0,-5,5])")
        return ("backgroundPass", "backgroundFail") 
    elif model in ("bern1","bern2","bern3","bern4","bern5"):
        order = int(model.replace("bern",""))
        cpass = ",".join(("prod::c2_pass%d(c_pass%d[0.2,-1,1],c_pass%d)"%(i,i,i)) for i in xrange(order))
        cfail = ",".join(("prod::c2_fail%d(c_fail%d[0.2,-1,1],c_fail%d)"%(i,i,i)) for i in xrange(order))
        w.factory("Bernstein::backgroundPass(mass, {%s})" % cpass)
        w.factory("Bernstein::backgroundFail(mass, {%s})" % cfail)
        return ("backgroundPass", "backgroundFail") 
    raise RuntimeError, "Unknown background model %s" % model

def cpeff(hpass,hfail):
    passing = int(hpass.Integral()); total = int(hfail.Integral() + passing)
    cpcalc = ROOT.TEfficiency.ClopperPearson
    if total:
         y0 = passing/float(total)
         ymax = cpcalc(total,passing,0.6827,True )
         ymin = cpcalc(total,passing,0.6827,False)
         return (y0, ymin-y0, ymax-y0)
    else:
        return None

def makeTGraph(effs):
    ret = ROOT.TGraphAsymmErrors(len(effs))
    for i,(x,xl,xh,eff,effl,effh) in enumerate(effs):
        ret.SetPoint(i, x, eff)
        ret.SetPointError(i, -xl,xh, -effl,effh)
    return ret

def chi2(hist, xvar, pdf, norm):
    hpdf = pdf.createHistogram("hpdf", xvar); 
    hpdf.SetDirectory(None)
    if hpdf.Integral():
        hpdf.Scale(norm / hpdf.Integral())
    sum = 0
    for i in xrange(1,hist.GetNbinsX()+1):
        oi = hist.GetBinContent(i)
        ei = hpdf.GetBinContent(i)
        # Poisson NLL would be
        #    - log ( ei^oi exp(-ei) / oi! )
        # Saturated model has ei == oi, so
        #    - log ( oi^oi exp(-oi) / oi! )
        # Poisson / Saturated gives
        #   - oi * (log(ei) - log(oi)) + (ei - oi) 
        if oi > 0.1:
            if ei > 0.1:
                sum += -2 * ( oi * log(ei/oi)  - (oi-ei) )
            else:
                sum += -2 * ( oi * log(0.1/oi)  - (oi-0.1) )
        else:
            sum += -2 * ( - (oi-ei) )
    return sum

def minimize(pdf, data, options, strategy=None):
    nll = pdf.createNLL(data, ROOT.RooFit.Extended(True))
    minim = ROOT.RooMinimizer(nll)
    minim.setPrintLevel(-1)
    minim.setStrategy(strategy if strategy is not None else options.minimizerStrategy);
    minim.setEps(options.minimizerTolerance);
    minim.setOffsetting(1);
    minim.optimizeConst(1);
    minim.setPrintEvalErrors(0);
    minim.setVerbose(False)
    retval = minim.minimize("Minuit2","migrad");
    print "MIGRAD RETURN: ",retval
    return (nll, minim, retval)

def manualMinos(pdf,data,var,options):
    var.setConstant(True)
    nll = pdf.createNLL(data, ROOT.RooFit.Extended(True))
    minim = ROOT.RooMinimizer(nll)
    minim.setPrintLevel(-1)
    minim.setStrategy(1);
    minim.setEps(1);
    minim.setOffsetting(0);
    minim.optimizeConst(1);
    minim.setPrintEvalErrors(0);
    minim.setVerbose(False)
    retval = minim.minimize("Minuit2","migrad");
    x0 = var.getVal(); nll0 = nll.getVal()
    errs = []; log = []
    for bound in var.getMin(), var.getMax():
        x1 = x0; x2 = bound; y2 = 999
        y1 = 0; cautious = 5
        firstLoop = True;
        for it in xrange(50): # at most 50 iterations
            if y2 < 0.5: raise RuntimeError, "Precondition violated"
            if firstLoop:
                log.append( "%2d init: x1 %+.5f x2 %+.5f  dx %.5f    y1 %+7.5f y2 %+7.5f " % (it, x1,x2,x2-x0,y1,y2) )
                print     ( "%2d init: x1 %+.5f x2 %+.5f  dx %.5f    y1 %+7.5f y2 %+7.5f " % (it, x1,x2,x2-x0,y1,y2) )
            while firstLoop:
                overstep = False
                var.setVal(x2); it2 = 0 
                while nll.getVal() - nll0 > cautious and it2 < 10:
                    overstep = True
                    log.append( "%2d firstLoop %2d: x1 %+.5f x2 %+.5f y2 unprof %+7.5f, caution %.0f " % (it,it2, x1,x2,nll.getVal() - nll0,cautious) )
                    print     ( "%2d firstLoop %2d: x1 %+.5f x2 %+.5f y2 unprof %+7.5f, caution %.0f " % (it,it2, x1,x2,nll.getVal() - nll0,cautious) )
                    x2 = 0.5*(x2+x1)
                    var.setVal(x2); 
                    it2 += 1
                if overstep:
                    x2 = x1 + 2*(x2-x1)
                for s in (1,0,2,1):
                    minim.setStrategy(s);
                    if minim.minimize("Minuit2","migrad") != -1:
                        break
                var.setVal(x2) 
                y2 = nll.getVal()-nll0
                log.append( "%2d firstLoop: x2 %+.5f --> y2 prof %+7.5f " % (it, x2, y2) )
                print     ( "%2d firstLoop: x2 %+.5f --> y2 prof %+7.5f " % (it, x2, y2) )
                if y2 < 0.5:
                    if not overstep:
                        log.append( " ==> error bar touches the boundary at x2" )
                        firstLoop = True
                        errs.append(x2-x0)
                        break
                    cautious *= 5
                else: 
                    firstLoop = False
            # ok, now y1 < 0.5, y2 > 0.5 but reasonable
            if firstLoop: break
            xc = 0.5*(x2+x1)
            var.setVal(xc); 
            for s in (1,0,2,1):
                minim.setStrategy(s);
                if minim.minimize("Minuit2","migrad") != -1:
                    break
            log.append( "%2d pre : x1 %+.5f x2 %+.5f  dx1 %+.5f  dx2 %+.5f   y1 %+7.5f y2 %+7.5f " % (it, x1,x2,x1-x0,x2-x0,y1,y2) )
            print     ( "%2d pre : x1 %+.5f x2 %+.5f  dx1 %+.5f  dx2 %+.5f   y1 %+7.5f y2 %+7.5f " % (it, x1,x2,x1-x0,x2-x0,y1,y2) )
            yc = nll.getVal() - nll0
            if yc < 0.5:
                x1,y1 = xc,yc
            else:  
                x2,y2 = xc,yc
                for s in (1,0,2,1):
                    minim.setStrategy(s);
                    if minim.minimize("Minuit2","migrad") != -1:
                        break
            log.append( "%2d post: x1 %+.5f x2 %+.5f  dx1 %+.5f  dx2 %+.5f   y1 %+7.5f y2 %+7.5f " % (it, x1,x2,x1-x0,x2-x0,y1,y2) )
            print     ( "%2d post: x1 %+.5f x2 %+.5f  dx1 %+.5f  dx2 %+.5f   y1 %+7.5f y2 %+7.5f " % (it, x1,x2,x1-x0,x2-x0,y1,y2) )
            if abs(x1-x2)<1e-5: 
                log.append( " --- completed: dx = %.5f\n" % (x2-x0) )
                print     ( " --- completed: dx = %.5f\n" % (x2-x0) )
                errs.append(x2-x0)
                break
    var.setConstant(False)
    if len(errs) == 2:
        var.setAsymError(min(errs),max(errs))
        return [ min(errs), max(errs) ], log
    else:
        return None, log

def fitAndPass(args):
    (x,y) = args
    return [ x, fitEfficiency(*y) ]

def fitEfficiency(name, hpass, hfail, refpass, reffail, options):
    eff0 = fitOneEfficiency(name, hpass, hfail, options.sigModel, options.bkgModel, refpass, reffail, options)
    if not eff0: return None
    sigModels = [ options.sigModel ] + options.altSigModel
    bkgModels = [ options.bkgModel ] + options.altBkgModel
    modelPairs = [ ( smod, bmod ) for smod in sigModels for bmod in bkgModels ]
    modelPairs.remove( (options.sigModel, options.bkgModel) )
    alls = {}
    if len(modelPairs) > 0:
        effL,effH = eff0[0]+eff0[1], eff0[0]+eff0[2]
        for smod, bmod in modelPairs:
            if options.request and options.request != name+"_sig_%s_bkg_%s" % (smod, bmod):
                continue
            eff1 = fitOneEfficiency(name+"_sig_%s_bkg_%s" % (smod, bmod), hpass, hfail, smod, bmod, refpass, reffail, options)
            if eff1:
                effL = min(effL, eff1[0]+eff1[1])
                effH = max(effH, eff1[0]+eff1[2])
                alls[(smod,bmod)] = eff1[:]
        eff0 += ( effL-eff0[0], effH-eff0[0], alls )
    return eff0

def fitOneEfficiency(name, hpass, hfail, sigModel, bkgModel, refpass, reffail, options):
    w = ROOT.RooWorkspace("w")
    w.factory("mass[%g,%g]" % (hpass.GetXaxis().GetXmin(), hpass.GetXaxis().GetXmax()))
    w.var("mass").setBins(hpass.GetNbinsX())
    w.factory("passing[yes=1,no=0]")
    # make combined dataset, relying on combine
    dpass = ROOT.RooDataHist("dpass","dpass", ROOT.RooArgList(w.var("mass")), hpass)
    dfail = ROOT.RooDataHist("dfail","dfail", ROOT.RooArgList(w.var("mass")), hfail)
    dfactory = ROOT.CombDataSetFactory(ROOT.RooArgSet(w.var("mass")), w.cat("passing"))
    dfactory.addSetBin("yes", dpass)
    dfactory.addSetBin("no",  dfail)
    data = dfactory.done("data","data")
    nsig = hpass.Integral(); nbkg = hfail.Integral(); nall = nsig+nbkg
    # make PDF
    signals = makeSignalModel(w, sigModel, refpass, reffail, options)
    if not signals: return None
    backgrounds = makeBackgroundModel(w, bkgModel, options)
    if not backgrounds: return None
    w.factory('expr::sigPass("@0*  @1",   Nsig[%g,0,%g], efficiency[0.9,0,1])' % (nsig,nall))
    w.factory('expr::sigFail("@0*(1-@1)", Nsig         , efficiency         )')
    w.factory('expr::bkgPass("@0*  @1",   Nbkg[%g,0,%g], effbkg[0.5,0,1])' % (nbkg,nall))
    w.factory('expr::bkgFail("@0*(1-@1)", Nbkg         , effbkg         )')
    w.factory('SUM::pdfPass(sigPass*{sigPdfPass}, bkgPass*{bkgPdfPass})'.format(sigPdfPass = signals[0], bkgPdfPass = backgrounds[0]))
    w.factory('SUM::pdfFail(sigFail*{sigPdfFail}, bkgFail*{bkgPdfFail})'.format(sigPdfFail = signals[1], bkgPdfFail = backgrounds[1]))
    w.factory('SIMUL::pdf(passing, yes=pdfPass, no=pdfFail)')   

    getattr(w,'import')(dpass, ROOT.RooCmdArg())
    getattr(w,'import')(dfail, ROOT.RooCmdArg())
    # make FIT

    #result = w.pdf("pdf").fitTo(data, ROOT.RooFit.Save(True), ROOT.RooFit.Minos(ROOT.RooArgSet(w.var("efficiency"))))
    nll, minim, retval = minimize(w.pdf("pdf"), data, options, strategy=0)
    nll, minim, retval = minimize(w.pdf("pdf"), data, options, strategy=None)
    if w.var("efficiency").getVal() > 0.9:
        w.var("efficiency").setMin(0.85)
        retval = minim.minimize("Minuit2","migrad");
    result = minim.save()
    effval = w.var("efficiency").getVal()
    efferr, minoslog = manualMinos(w.pdf("pdf"), data, w.var("efficiency"), options)
    #poi = ROOT.RooArgSet(w.var("efficiency"))
    #ret = minim.minos(poi)
    print "MANUAL MINOS: ",efferr,"\n\t","\n\t".join(minoslog)

    # plot
    c1 = ROOT.TCanvas("c1","c1"); c1.SetCanvasSize(900,500);
    c1.Divide(2,1)
    c1.cd(1)
    fpass = w.var("mass").frame()
    dpass.plotOn(fpass)
    w.pdf("pdfPass").plotOn(fpass, ROOT.RooFit.Components(backgrounds[0]),ROOT.RooFit.LineColor(ROOT.kGreen+3))
    w.pdf("pdfPass").plotOn(fpass, ROOT.RooFit.LineColor(ROOT.kGreen+1))
    fpass.Draw()
    c1.cd(2)
    ffail = w.var("mass").frame()
    dfail.plotOn(ffail)
    w.pdf("pdfFail").plotOn(ffail, ROOT.RooFit.Components(backgrounds[1]),ROOT.RooFit.LineColor(ROOT.kRed+3))
    w.pdf("pdfFail").plotOn(ffail, ROOT.RooFit.LineColor(ROOT.kRed+0))
    ffail.Draw()
    for ext in "pdf","png":
        c1.Print("%s/%s.%s" % (options.printDirBins, name, ext))
    log = open("%s/%s.txt" % (options.printDirBins, name), "w")
    fpf = result.floatParsFinal()
    for i in range(fpf.getSize()):
        par = fpf.at(i)
        log.write("%-20s : " % par.GetName())
        if par.hasAsymError():
            log.write("%8.4f  %+8.4f / %+8.4f" % (par.getVal(), par.getAsymErrorLo(), par.getAsymErrorHi()))
            plo, phi = par.getVal() - par.getAsymErrorLo(), par.getVal() + par.getAsymErrorHi()
        else:
            if par.GetName() in ("Nsig", "Nbkg"):
                log.write("%8.1f  %+8.1f           " % (par.getVal(), par.getError()))
            else:
                log.write("%8.4f  %+8.4f           " % (par.getVal(), par.getError()))
            plo, phi = par.getVal() - par.getError(), par.getVal() + par.getError()
        pmax, pmin = par.getMax(), par.getMin()
        log.write("   [ %8.4f , %8.4f ]" % (pmin, pmax))
        if phi > 0.95*pmax + 0.05*pmin: log.write("   <-- close to max")
        if plo < 0.05*pmax + 0.95*pmin: log.write("   <-- close to min")
        log.write("\n")

    log.write("MANUAL MINOS: %s\n\t%s\n"  % (efferr,"\n\t".join(minoslog)) )

    # goodness of fit
    chi2pass = chi2(hpass, w.var("mass"), w.pdf("pdfPass"), w.function("sigPass").getVal() + w.function("bkgPass").getVal())
    chi2fail = chi2(hfail, w.var("mass"), w.pdf("pdfFail"), w.function("sigFail").getVal() + w.function("bkgFail").getVal())
    log.write("\nChi2: pass %.2f, fail %.2f, total %.2f, ndof: %d - %d = %d \n" % (
        chi2pass, chi2fail, chi2pass + chi2fail, 
        hpass.GetNbinsX()*2, fpf.getSize(),  hpass.GetNbinsX()*2 - fpf.getSize()+1))
    log.close()


    tfout = ROOT.TFile.Open("%s/%s.%s" % (options.printDirBins, name, "root"), "RECREATE")
    w.SetName("w_"+name)
    tfout.WriteTObject(w, "w_"+name)
    tfout.Close()

    # report result
    if efferr: 
        return effval, efferr[0], efferr[1]
    else:
        return None

def makeHistos2D(tree,numcut,dencut,x,mass,options,post="",reftree=None):
    if options.request and not options.request.startswith(options.name): return None
    if options.request and post != "" and not options.request.endswith(post): return None
    xvar, xbins = x
    massvar, massbins = mass
    if massbins[0] == "[": massvals = eval(massbins)
    else:                  massvals = uniformAxis_(massbins)
    if xbins[0] == "[": xvals = eval(xbins)
    else:               xvals = uniformAxis_(xbins)
    xbins = len(xvals)-1; massbins = len(massvals)-1
    weight = options.mcWeight if tree == reftree else "1"
    ROOT.gROOT.cd()
    den2d = ROOT.TH2D("hden","hden",xbins,array('f',xvals),massbins,array('f',massvals))
    num2d = ROOT.TH2D("hnum","hnum",xbins,array('f',xvals),massbins,array('f',massvals))
    print "Reading denominator"
    tree.Draw("(%s):(%s)>>hden" % (massvar,xvar), "(%s) * (%s)" % (dencut,weight), "goff", options.maxEntries)
    print "Reading numerator"
    tree.Draw("(%s):(%s)>>hnum" % (massvar,xvar), "((%s) && (%s)) * (%s)" % (numcut,dencut,weight), "goff", options.maxEntries)
    if reftree:
        rden2d = ROOT.TH2D("rhden","rhden",xbins,array('f',xvals),massbins,array('f',massvals))
        rnum2d = ROOT.TH2D("rhnum","rhnum",xbins,array('f',xvals),massbins,array('f',massvals))
        print "Reading mc denominator"
        reftree.Draw("(%s):(%s)>>rhden" % (massvar,xvar), "(!(%s) && (%s) && (%s)) * (%s)" % (numcut,dencut,options.mcCut,options.mcWeight), "goff", options.maxEntries)
        print "Reading mc numerator"
        reftree.Draw("(%s):(%s)>>rhnum" % (massvar,xvar), "( (%s) && (%s) && (%s)) * (%s)" % (numcut,dencut,options.mcCut,options.mcWeight), "goff", options.maxEntries)
        if options.mcMass != massvar:
            rtden2d = ROOT.TH2D("rthden","rthden",xbins,array('f',xvals),massbins,array('f',massvals))
            rtnum2d = ROOT.TH2D("rthnum","rthnum",xbins,array('f',xvals),massbins,array('f',massvals))
            print "Reading mc truth denominator"
            reftree.Draw("(%s):(%s)>>rthden" % (options.mcMass,xvar), "(!(%s) && (%s) && (%s)) * (%s)" % (numcut,dencut,options.mcCut,options.mcWeight), "goff", options.maxEntries)
            print "Reading mc truth numerator"
            reftree.Draw("(%s):(%s)>>rthnum" % (options.mcMass,xvar), "( (%s) && (%s) && (%s)) * (%s)" % (numcut,dencut,options.mcCut,options.mcWeight), "goff", options.maxEntries)
    print "Ready for fits"
    effs = [ ]; seffs = []; ceffs = []; teffs = []; alls = {}
    tofit = []
    for i in xrange(1,xbins+1):
        print "Processing %s bin %d" % (xvar,i)
        num1d = ROOT.TH1D("num","num",massbins,array('f',massvals)); num1d.SetDirectory(None)
        den1d = ROOT.TH1D("den","den",massbins,array('f',massvals)); den1d.SetDirectory(None)
        rnum1d = None; rden1d = None
        rtnum1d = None; rtden1d = None
        if reftree:
            rnum1d = ROOT.TH1D("rnum","rnum",massbins,array('f',massvals)); rnum1d.SetDirectory(None)
            rden1d = ROOT.TH1D("rden","rden",massbins,array('f',massvals)); rden1d.SetDirectory(None)
            if options.mcMass != massvar:
                rtnum1d = ROOT.TH1D("rtnum","rtnum",massbins,array('f',massvals)); rtnum1d.SetDirectory(None)
                rtden1d = ROOT.TH1D("rtden","rtden",massbins,array('f',massvals)); rtden1d.SetDirectory(None)
                rnum1d.truth = rtnum1d
                rden1d.truth = rtden1d
        for j in xrange(1,massbins+1):
            num1d.SetBinContent(j,num2d.GetBinContent(i,j))
            den1d.SetBinContent(j,den2d.GetBinContent(i,j))
            if rnum1d:
                rnum1d.SetBinContent(j,rnum2d.GetBinContent(i,j))
                rden1d.SetBinContent(j,rden2d.GetBinContent(i,j))
            if rtnum1d:
                rtnum1d.SetBinContent(j,rtnum2d.GetBinContent(i,j))
                rtden1d.SetBinContent(j,rtden2d.GetBinContent(i,j))
        den1d.Add(num1d,-1.0)
        if options.request and "bin" in options.request and options.request != options.name+"_bin%d%s"%(i,post):
            continue
        x  = num2d.GetXaxis().GetBinCenter(i)
        xl = num2d.GetXaxis().GetBinLowEdge(i)-x
        xh = num2d.GetXaxis().GetBinUpEdge(i)-x
        tofit.append( ( (x,xl,xh), (options.name+"_bin%d%s"%(i,post), num1d, den1d, rnum1d, rden1d, options) ) )
        #eff = fitEfficiency(options.name+"_bin%d%s"%(i,post), num1d, den1d, rnum1d, rden1d, options)
        ceff = cpeff(num1d,den1d)
        ceffs.append( (x,xl,xh, ceff[0],ceff[1],ceff[2]) )
        if rnum1d and tree == reftree:
            teff = cpeff(rnum1d,rden1d)
            teffs.append( (x,xl,xh, teff[0],teff[1],teff[2]) )
    if options.jobs > 0:
        from multiprocessing import Pool
        fits = Pool(options.jobs).map(fitAndPass, tofit)
    else:
        fits = map(fitAndPass, tofit)
    for ((x,xl,xh),eff) in fits:
        if eff:
            effv, effl, effh = eff[:3]
            effs.append( (x,xl,xh, effv,effl,effh) )
            if len(eff) >= 5:
                seffs.append( (x,xl,xh, effv,eff[3],eff[4]) )
                for k,v in effs[5].iteritems():
                    if k not in alls: alls[k] = []
                    alls[k].append( (x,xl,xh, v[0],v[1],v[2]) )
    ret = makeTGraph(effs)
    if len(seffs): ret.syst = makeTGraph(seffs)
    if len(alls): ret.alls = dict([ (k,makeTGraph(v)) for (k,v) in alls.iteritems() ])
    if len(ceffs): ret.counting = makeTGraph(ceffs)
    if len(teffs): ret.truth = makeTGraph(teffs)
    return ret

def makeRatio(numeff,deneff):
    numsyst = getattr(numeff, 'syst', numeff)
    densyst = getattr(deneff, 'syst', deneff)
    xn = numeff.GetX()
    yn = numeff.GetY()
    xd = deneff.GetX()
    yd = deneff.GetY()
    yns = numsyst.GetY()
    yds = densyst.GetY()
    ratio = ROOT.TGraphAsymmErrors(numeff.GetN())
    unity = ROOT.TGraphAsymmErrors(numeff.GetN())
    ratiosyst = ROOT.TGraphAsymmErrors(numeff.GetN())
    unitysyst = ROOT.TGraphAsymmErrors(numeff.GetN())
    text  = []
    for i in xrange(numeff.GetN()):
        found = False
        for i2 in xrange(deneff.GetN()):
            if abs(xn[i]-xd[i2]) < 1e-4: 
                found = True; break
        if yd[i2] <= 0 or not found:
            unity.SetPoint(i , xn[i ], -99)
            ratio.SetPoint(i , xn[i ], -99)
            unity.SetPointError(i , numeff.GetErrorXlow(i ), numeff.GetErrorXhigh(i ), 0,0)
            ratio.SetPointError(i , deneff.GetErrorXlow(i2), deneff.GetErrorXhigh(i2), 0,0)
            unitysyst.SetPoint(i , xn[i ], -99)
            ratiosyst.SetPoint(i , xn[i ], -99)
            unitysyst.SetPointError(i , numeff.GetErrorXlow(i ), numeff.GetErrorXhigh(i ), 0,0)
            ratiosyst.SetPointError(i , deneff.GetErrorXlow(i2), deneff.GetErrorXhigh(i2), 0,0)
        else:
            ratio.SetPoint(i , xn[i ], yn[i ]/yd[i2])
            unity.SetPoint(i , xn[i ], 1.0)
            ratio.SetPointError(i , numeff.GetErrorXlow(i ), numeff.GetErrorXhigh(i ), numeff.GetErrorYlow(i )/yd[i2], numeff.GetErrorYhigh(i )/yd[i2])
            unity.SetPointError(i , deneff.GetErrorXlow(i2), deneff.GetErrorXhigh(i2), deneff.GetErrorYlow(i2)/yd[i2], deneff.GetErrorYhigh(i2)/yd[i2])
            ratiosyst.SetPoint(i , xn[i ], yn[i ]/yd[i2])
            unitysyst.SetPoint(i , xn[i ], 1.0)
            ratiosyst.SetPointError(i , numeff.GetErrorXlow(i ), numeff.GetErrorXhigh(i ), numsyst.GetErrorYlow(i )/yd[i2], numsyst.GetErrorYhigh(i )/yd[i2])
            unitysyst.SetPointError(i , deneff.GetErrorXlow(i2), deneff.GetErrorXhigh(i2), densyst.GetErrorYlow(i2)/yd[i2], densyst.GetErrorYhigh(i2)/yd[i2])
            if numsyst != numeff or densyst != deneff:
                text.append( "at bin %3d: x = %8.2f, num = %.4f -%.4f/+%.4f -%.4f/+%.4f   den = %.4f -%.4f/+%.4f -%.4f/+%.4f  ratio = %.4f -%.4f/+%.4f -%.4f/+%.4f = %.4f -%.4f/+%.4f " % (i +1, xn[i ],
                                    yn[i ], numeff.GetErrorYlow(i ), numeff.GetErrorYhigh(i ), 
                                           hypsub(numsyst.GetErrorYlow(i ),numeff.GetErrorYlow(i )), hypsub(numsyst.GetErrorYhigh(i ),numeff.GetErrorYhigh(i )),
                                    yd[i2], deneff.GetErrorYlow(i2), deneff.GetErrorYhigh(i2),
                                           hypsub(densyst.GetErrorYlow(i2),deneff.GetErrorYlow(i2)), hypsub(densyst.GetErrorYhigh(i2),deneff.GetErrorYhigh(i2)),
                                    yn[i ]/yd[i2], 
                                    hypot(numeff.GetErrorYlow(i ), deneff.GetErrorYhigh(i2))/yd[i2], 
                                    hypot(numeff.GetErrorYhigh(i ), deneff.GetErrorYlow(i2))/yd[i2],
                                    hypot(hypsub(numsyst.GetErrorYlow(i ), numeff.GetErrorYlow(i )), hypsub(densyst.GetErrorYhigh(i2),deneff.GetErrorYhigh(i2)))/yd[i2], 
                                    hypot(hypsub(numsyst.GetErrorYhigh(i ),numeff.GetErrorYhigh(i )),hypsub(densyst.GetErrorYlow(i2), deneff.GetErrorYlow(i2)))/yd[i2],
                                    yn[i ]/yd[i2], 
                                    hypot(numsyst.GetErrorYlow(i ), densyst.GetErrorYhigh(i2))/yd[i2], 
                                    hypot(numsyst.GetErrorYhigh(i ), densyst.GetErrorYlow(i2))/yd[i2] ))
            else:
                text.append( "at bin %3d: x = %8.2f, num = %.4f -%.4f/+%.4f  den = %.4f -%.4f/+%.4f  ratio = %.4f -%.4f/+%.4f " % (i +1, xn[i ],
                                    yn[i ], numeff.GetErrorYlow(i ), numeff.GetErrorYhigh(i ),
                                    yd[i2], deneff.GetErrorYlow(i2), deneff.GetErrorYhigh(i2),
                                    yn[i ]/yd[i2], 
                                    hypot(numeff.GetErrorYlow(i ), deneff.GetErrorYhigh(i2))/yd[i2], 
                                    hypot(numeff.GetErrorYhigh(i ), deneff.GetErrorYlow(i2))/yd[i2] ))
    if numsyst != numeff: ratio.syst = ratiosyst
    if densyst != deneff: unity.syst = unitysyst
    return [ ratio, unity ], text

def makeRatios(effs):
    if len(effs) != 2: raise RuntimeError, "Not supported yet"
    return makeRatio(effs[0],effs[1])

def makeFrame(options):
    if ROOT.gROOT.FindObject("frame") != None: ROOT.gROOT.FindObject("frame").Delete()
    xvars,xbins = options.xvar
    if xbins[0] == "[":
        from array import array
        xvals = eval(xbins)
        hist1d = ROOT.TH1D("frame","frame",len(xvals)-1,array('f',xvals))
    else:
        xn,xmin,xmax=xbins.split(",")
        hist1d = ROOT.TH1D("frame","frame",int(xn),float(xmin),float(xmax))
    styleCommon(hist1d)
    return hist1d

def styleCommon(graph,options):
    graph.GetYaxis().SetRangeUser(options.yrange[0], options.yrange[1])
    graph.GetYaxis().SetDecimals(True)
    if options.xtitle: graph.GetXaxis().SetTitle(options.xtitle)
    if options.ytitle: graph.GetYaxis().SetTitle(options.ytitle)

def styleAsData(graph,options):
    styleCommon(graph,options)
    graph.SetMarkerStyle(20)
    graph.SetLineWidth(2)
    graph.SetLineColor(ROOT.kBlack)
    graph.SetMarkerColor(ROOT.kBlack)
    gsyst = getattr(graph, 'syst', None)
    if gsyst:
        styleCommon(gsyst,options)
        gsyst.SetMarkerStyle(0)
        gsyst.SetLineWidth(4)
        gsyst.SetLineColor(ROOT.kRed+0)
        gsyst.SetMarkerColor(ROOT.kRed+0)

def styleAsRef(graph,options):
    styleCommon(graph,options)
    graph.SetMarkerStyle(20)
    graph.SetFillColor(ROOT.kAzure+10)
    gsyst = getattr(graph, 'syst', None)
    if gsyst:
        styleCommon(gsyst,options)
        gsyst.SetMarkerStyle(20)
        gsyst.SetFillColor(ROOT.kViolet+6)

def stackEfficiencies(base,ref,options):
    styleAsData(base,options)
    styleAsRef(ref,options)
    ref.Draw("AE2");
    if hasattr(ref,"syst"):
        ref.syst.Draw("E2 SAME");
        ref.Draw("E2 SAME");
    if hasattr(base,"syst"):
        base.syst.Draw("PZ SAME");
    base.Draw("PZ SAME");

def plotRatios(effs,ratios,options):
    for e in effs:
        e.GetXaxis().SetLabelOffset(999) ## send them away
        e.GetXaxis().SetTitleOffset(999) ## in outer space
        e.GetYaxis().SetLabelSize(0.05)
    ratio, unity = ratios
    allratios = ratios
    ratiosyst = getattr(ratio, 'syst', None)
    unitysyst = getattr(unity, 'syst', None)
    styleAsData(ratio,options)
    styleAsRef(unity,options)
    if ratiosyst: 
        allratios += [ratiosyst]
        ymax = max(ratiosyst.GetY()[i]+ratiosyst.GetErrorYhigh(i) for i in xrange(ratiosyst.GetN()))
        ymin = min(ratiosyst.GetY()[i]-ratiosyst.GetErrorYlow(i)  for i in xrange(ratiosyst.GetN()))
    else:
        ymax = max(ratio.GetY()[i]+ratio.GetErrorYhigh(i) for i in xrange(ratio.GetN()))
        ymin = min(ratio.GetY()[i]-ratio.GetErrorYlow(i)  for i in xrange(ratio.GetN()))
    if unitysyst: allratios += [unitysyst]
    for r in allratios:
        #r.GetYaxis().SetRangeUser(options.rrange[0],options.rrange[1]);
        r.GetYaxis().SetRangeUser(1+1.3*(min(0.99,ymin)-1),1+1.3*(max(1.01,ymax)-1));
        r.GetXaxis().SetTitleSize(0.14)
        r.GetYaxis().SetTitleSize(0.14)
        r.GetXaxis().SetLabelSize(0.11)
        r.GetYaxis().SetLabelSize(0.11)
        r.GetYaxis().SetNdivisions(505)
        r.GetYaxis().SetTitle("ratio")
        r.GetYaxis().SetTitleOffset(0.52);
    unity.Draw("AE2")
    if unitysyst:
        unitysyst.Draw("E2 SAME")
    line = ROOT.TLine(unity.GetXaxis().GetXmin(),1,unity.GetXaxis().GetXmax(),1)
    line.SetLineWidth(2);
    line.SetLineColor(ROOT.kGray+2);
    line.SetLineStyle(2);
    unity.Draw("E2 SAME")
    line.DrawLine(unity.GetXaxis().GetXmin(),1,unity.GetXaxis().GetXmax(),1)
    if ratiosyst:
        ratiosyst.Draw("PZ SAME")
    ratio.Draw("PZ SAME")
       

def plotEffs(name,effs,options):
    c1 = ROOT.TCanvas("c1", "c1", 600, (750 if options.doRatio else 600))
    c1.Draw()
    p1, p2 = c1, None # high and low panes
    # set borders, if necessary create subpads
    if len(effs) > 1 and options.doRatio:
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
    if len(effs) == 2:
        stackEfficiencies(effs[0],effs[1],options)
    else:
        styleAsData(effs[0],options)
        effs[0].Draw("AP")
    text = []
    if len(effs) > 1 and options.doRatio:
        ratios, text = makeRatios(effs)
        p2.cd()
        plotRatios(effs,ratios,options)
    for ext in "pdf","png":
        c1.Print("%s/%s.%s" % (options.printDir, name, ext))
    handle = open("%s/%s.%s"  % (options.printDir, name, "txt"), "w")
    for line in text: handle.write(line+"\n")
    fout = ROOT.TFile.Open("%s/%s.%s"  % (options.printDir, name, "root"), "RECREATE")
    fout.WriteTObject(effs[0], name)
    if getattr(effs[0],'syst',None): fout.WriteTObject(effs[0].syst, name+"_syst")
    if getattr(effs[0],'counting',None): fout.WriteTObject(effs[0].counting, name+"_counting")
    if len(effs) > 1:
        fout.WriteTObject(effs[1], name+"_ref")
        if getattr(effs[1],'syst',None): fout.WriteTObject(effs[1].syst, name+"_ref_syst")
        if getattr(effs[1],'counting',None): fout.WriteTObject(effs[1].counting, name+"_ref_counting")
        if getattr(effs[1],'truth',None): fout.WriteTObject(effs[1].truth, name+"_ref_truth")
        if options.doRatio:
            if getattr(ratios[0],'syst',None): fout.WriteTObject(ratios[0].syst, name+"_syst")
            fout.WriteTObject(ratios[0], name+"_ratio")
    if getattr(effs[0],"alls",{}):
        for k,v in effs[0].alls.iteritems():
            fout.WriteTObject(v, name+"_sig_%s_bkg_%s" % k)
        for k,v in effs[1].alls.iteritems():
            fout.WriteTObject(v, name+"_ref_sig_%s_bkg_%s" % k)
            if k in effs[0].alls:
                vratios, vtext = (effs[0].alls[k], v)
                fout.WriteTObject(vratios[0], name+"_ratio_sig_%s_bkg_%s" % k)
    fout.Close()
    

def addTnPEfficiencyOptions(parser):
    parser.add_option("-t", "--tree",    dest="tree", default='tree', help="Pattern for tree name");
    parser.add_option("-s", "--signalModel",   dest="sigModel", default='voigt', help="Signal model");
    parser.add_option("-b", "--backgroundModel",   dest="bkgModel", default='expo', help="Background model");
    parser.add_option("--salt", "--altSignalModel",   dest="altSigModel", default=[], action="append", help="Signal model");
    parser.add_option("--balt", "--altBackgroundModel",   dest="altBkgModel", default=[], action="append", help="Background model");
    parser.add_option("-N", "--name",    dest="name", type="string", help="name", default="eff")
    parser.add_option("-n", "--num",     dest="num", type="string", default="1", help="numerator")
    parser.add_option("-d", "--den",     dest="den", type="string", default="1", help="denominator")
    parser.add_option("-x", "--x-var",     dest="xvar", type="string", default=("TnP_l2_pt","[5,10,15,20,25,30,35,40,50,60,80]"), nargs=2, help="X var and bin")
    parser.add_option("-m", "--mass-var",  dest="mvar", type="string", default=("TnP_mass","80,72,115"), nargs=2, help="Mass var and binning")
    parser.add_option("-M", "--max-entries",    dest="maxEntries", default=99999999, type="long")
    parser.add_option("-r", "--refmc",   dest="refmc", default=None, help="refmc");
    parser.add_option("--mcm", "--mc-mass",  dest="mcMass", type="string", default="TnP_mass*sqrt(TnP_l1_mcPt*TnP_l2_mcPt/TnP_l1_pt/TnP_l2_pt)", help="mc Mass")
    parser.add_option("--mcc", "--mc-cut",  dest="mcCut", type="string", default="TnP_l1_mcMatchId != 0 && TnP_l2_mcMatchId != 0", help="mc Mass")
    parser.add_option("-W", "--mcw", "--mc-weight",  dest="mcWeight", type="string", default="1", help="mc Mass")
    parser.add_option("--minimizer-strategy",  dest="minimizerStrategy", default=2, type="int")
    parser.add_option("--minimizer-tolerance", dest="minimizerTolerance", default=0.1, type="float")
    parser.add_option("--xtitle",   dest="xtitle", type="string", default=None, help="X title")
    parser.add_option("--ytitle",   dest="ytitle", type="string", default="Efficiency", help="Y title")
    parser.add_option("--mtitle",   dest="mtitle", type="string", default="Mass (GeV)", help="M title")
    parser.add_option("--pdir", "--print-dir", dest="printDir", type="string", default="plots", help="print out plots in this directory");
    parser.add_option("--yrange", dest="yrange", type="float", nargs=2, default=(0,1.025));
    parser.add_option("--rrange", dest="rrange", type="float", nargs=2, default=(0.95,1.05));
    parser.add_option("--doRatio", dest="doRatio", action="store_true", default=False, help="Add a ratio plot at the bottom")
    parser.add_option("--request", "--refit", dest="request", type="string", default=None, help="make a single fit");
    parser.add_option("-j", "--jobs",    dest="jobs",      type="int",    default=0, help="Use N threads");

if __name__ == "__main__":
    from optparse import OptionParser
    parser = OptionParser(usage="%prog [options] tree reftree")
    addTnPEfficiencyOptions(parser)
    (options, args) = parser.parse_args()
    if len(args) == 0:
        print "You must specify at least one tree to fit"
    ROOT.gROOT.SetBatch(True)
    ROOT.gROOT.ProcessLine(".x ~/cpp/tdrstyle.cc")
    ROOT.gSystem.Load("libHiggsAnalysisCombinedLimit")
    if "/functions_cc.so" not in ROOT.gSystem.GetLibraries(): 
        ROOT.gROOT.ProcessLine(".L %s/src/CMGTools/TTHAnalysis/python/plotter/functions.cc+" % os.environ['CMSSW_BASE']);
    ROOT.RooMsgService.instance().setGlobalKillBelow(ROOT.RooFit.ERROR)
    if not os.path.exists(options.printDir):
        os.system("mkdir -p %s" % options.printDir)
        os.system("cp /afs/cern.ch/user/g/gpetrucc/php/index.php  %s/" % options.printDir)
    options.printDirBins = options.printDir +("/%s_bins"%options.name)
    if not os.path.exists(options.printDirBins):
        os.system("mkdir -p %s" % options.printDirBins)
        os.system("cp /afs/cern.ch/user/g/gpetrucc/php/index.php  %s/" % options.printDirBins)
    tree = ROOT.TChain(options.tree)
    for fname in args: tree.Add(fname)
    trees = [ (tree, "") ]
    if options.refmc:
        reffile = ROOT.TFile.Open(options.refmc)
        reftree = reffile.Get(options.tree)
        trees += [ (reftree, "_ref") ]
    else:
        reftree = None

    effs =  [ makeHistos2D(t,options.num,options.den,options.xvar,options.mvar,options,post=l,reftree=reftree) for (t,l) in trees ]
    if options.request and "bin" in options.request: exit()
    plotEffs(options.name,effs,options)
    if getattr(effs[0],'counting',None):
        plotEffs(options.name+"_counting",[e.counting for e in effs],options)
    if reftree:
        plotEffs(options.name+"_mcclosure",[effs[1], effs[1].truth],options)

