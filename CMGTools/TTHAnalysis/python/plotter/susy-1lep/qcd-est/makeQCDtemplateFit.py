#!/usr/bin/env python

import sys
import os

#tmpArg = sys.argv
#sys.argv = ['-b']
from ROOT import *
#sys.argv = tmpArg

_dhStore = {}
_pdfStore = {}
_varStore = {}
_hStore = {}
_canvStore = {}

def doLegend():

    leg = TLegend(0.63,0.525,0.87,0.875)
    leg.SetBorderSize(1)
    leg.SetTextFont(62)
    leg.SetTextSize(0.03321678)
    leg.SetLineColor(1)
    leg.SetLineStyle(1)
    leg.SetLineWidth(1)
    leg.SetFillColor(0)
    leg.SetFillStyle(1001)

    return leg

def getHistIntError(hist):
    # rebin hist to 1 bin and get the error

    htmp = hist.Clone('tmp')
    htmp.Rebin(htmp.GetNbinsX())

    return htmp.GetBinError(1)

def getVarFromHist(hist, varname):

    minX = hist.GetXaxis().GetXmin();
    maxX = hist.GetXaxis().GetXmax();
    nBins = hist.GetNbinsX();

    # Create observable
    var = RooRealVar(varname, varname, minX, maxX)
    var.setBins(nBins);

    return var

def getPDFfromHist(hist, var):

    hname = hist.GetName()

    # binned dataset
    dh = RooDataHist('dh'+hname,'dh'+hname,RooArgList(var),hist)
    _dhStore[dh.GetName()] = dh # store dh
    # pdf from dh
    pdf = RooHistPdf('pdf'+hname,'pdf'+hname,RooArgSet(var),dh,0)
    _pdfStore[pdf.GetName()] = pdf

    return pdf

def getDataFromHist(hist, var, mcData = True):

    hname = hist.GetName()

    # binned dataset
    dh = RooDataHist('dh'+hname,'dh'+hname,RooArgList(var),hist)
    _dhStore[dh.GetName()] = dh # store dh

    if mcData:
        nevents = hist.Integral()
        # pdf from dh
        pdf = RooHistPdf('pdf'+hname,'pdf'+hname,RooArgSet(var),dh,0)
        _pdfStore[pdf.GetName()] = pdf

        # generate toys
        data = pdf.generateBinned(RooArgSet(var),nevents,RooFit.Name("pseudoData"))

        return data

    else:
        return dh

def setHistRange(hist, maxX = 2.0, minX = 0.5):
    # modifies range by setting bins to zero

    for ibin in range(hist.GetNbinsX()):
        if hist.GetBinCenter(ibin) < minX or hist.GetBinCenter(ibin) > maxX:
            hist.SetBinContent(ibin,0)

def getQCDfromFit(dhData, hQCD, hEWK, var):

    # modify range of QCD
    #setHistRange(hQCD)

    # get PDFs for QCD and EWK
    pdfQCD = getPDFfromHist(hQCD, var)
    pdfEWK = getPDFfromHist(hEWK, var)

    _pdfStore[pdfQCD.GetName()] = pdfQCD
    _pdfStore[pdfEWK.GetName()] = pdfEWK

    # Number of (pseudo)data events for limits
    nevents = dhData.sumEntries()

    # Number of EWK and QCD events as variables
    nEWK = RooRealVar("nEWK","#EWK events",nevents,0,2*nevents)
    nQCD = RooRealVar("nQCD","#QCD events",nevents,0,2*nevents)

    _varStore['nEWK'] = nEWK
    _varStore['nQCD'] = nQCD

    pdfTemplate = RooAddPdf("pdfTemplate","EWK + QCD template",RooArgList(pdfEWK,pdfQCD),RooArgList(nEWK,nQCD));

    pdfTemplate.fitTo(dhData,RooFit.PrintLevel(-1))#,RooFit.SumW2Error(kTRUE))
    #pdfTemplate.chi2FitTo(dhData,RooLinkedList())#RooFit.SumW2Error(kTRUE))

    _pdfStore[pdfTemplate.GetName()] = pdfTemplate

    return nQCD#(nQCD.getValV(),nQCD

def getHistsFromFile(tfile, binname = 'incl', mcData = True):

    # get full BKG, QCD and EWK histos
    QCDseleName = 'Lp_sel_'+binname+'_QCD'
    QCDantiName = 'Lp_anti_'+binname+'_QCD'
    #QCDantiName = 'Lp_anti_incl_QCD'

    hQCDsele = tfile.Get(QCDseleName).Clone('QCDsel_'+binname)
    hQCDanti = tfile.Get(QCDantiName).Clone('QCDanti_'+binname)

    _hStore[hQCDsele.GetName()] = hQCDsele
    _hStore[hQCDanti.GetName()] = hQCDanti

    EWKselName = 'Lp_sel_'+binname+'_background'
    hEWKsele = tfile.Get(EWKselName).Clone('EWKsel_'+binname)

    _hStore[hEWKsele.GetName()] = hEWKsele

    if mcData:
        # Create ~DATA~ hist from selected QCD and EWK
        hData = hEWKsele.Clone('DataSel_'+binname)
        hData.Add(hQCDsele)
    else:
        # take data histogram
        hData = tfile.Get('Lp_sel_'+binname+'_data').Clone('DataSel_'+binname)

    _hStore[hData.GetName()] = hData

    # return data hist, EWK and QCD templates
    return (hData,hEWKsele,hQCDsele,hQCDanti)

def plotHists(binname = 'incl', inclTemplate = False, addHists = True):

    # frame
    frame = _varStore['Lp'].frame(RooFit.Title('Lp distributions and fit in bin '+binname))

    _dhStore['data_'+binname].plotOn(frame,RooFit.Name('data'),RooFit.DataError(RooAbsData.SumW2) )#,RooLinkedList())

    # plot full template fit
    _pdfStore['pdfTemplate'].plotOn(frame,RooFit.LineColor(4),RooFit.Name('FullFit'))
    # plot only QCD component
    if not inclTemplate:
        argset = RooArgSet(_pdfStore['pdfQCDanti_'+binname]) # hack to keep arguments alive
    else:
        import re
        incName = re.sub('ST[0-9]_','',binname)
        argset = RooArgSet(_pdfStore['pdfQCDanti_'+incName]) # hack to keep arguments alive
    _pdfStore['pdfTemplate'].plotOn(frame,RooFit.Components(argset),RooFit.LineColor(2),RooFit.LineStyle(2),RooFit.Name('QCDfit'))
    # plot only EWK
    argset2 = RooArgSet(_pdfStore['pdfEWKsel_'+binname]) # hack to keep arguments alive
    _pdfStore['pdfTemplate'].plotOn(frame,RooFit.Components(argset2),RooFit.LineColor(3),RooFit.LineStyle(2),RooFit.Name('EWKfit'))

    # PLOT
    canv = TCanvas("canv"+binname,"canvas for bin "+binname,800,600)
    frame.Draw()

    if addHists:
        _hStore['EWKsel_'+binname].SetFillStyle(3001)
        _hStore['QCDsel_'+binname].SetFillStyle(3002)

        stack = THStack('hs','hstack')
        stack.Add(_hStore['QCDsel_'+binname])
        stack.Add(_hStore['EWKsel_'+binname])
        stack.Draw("histsame")
        #_hStore['QCDsel_'+binname].Draw("histsame")

        SetOwnership( stack, 0 )
        #SetOwnership( _hStore['QCDsel_'+binname], 0 )
        #SetOwnership( _hStore['EWKsel_'+binname], 0 )
        #hData.Draw('histsame')

    frame.Draw("same")

    # LEGEND
    leg = doLegend()
    leg.AddEntry(frame.findObject('data'),'Pseudo Data','lp')

    if addHists:
        leg.AddEntry(_hStore['EWKsel_'+binname],'EWK selected','f')
        leg.AddEntry(_hStore['QCDsel_'+binname],'QCD selected','f')

    leg.AddEntry(frame.findObject('FullFit'),'Full fit','l')
    leg.AddEntry(frame.findObject('QCDfit'),'QCD fit','l')
    leg.AddEntry(frame.findObject('EWKfit'),'EWK fit','l')

    leg.Draw()

    SetOwnership( leg, 0 )

    #for prim in  canv.GetListOfPrimitives(): print prim

    gPad.Update()

    if '-b' not in sys.argv:
        # wait for input
        answ = ['c']
        while 'c' not in answ:
            answ.append(raw_input("Enter 'c' to continue: "))

    _canvStore[canv.GetName()] = canv

    return canv

def getQCDratio(tfile,binname = 'incl', doPlot = False, doClosure = False, inclTemplate = False):

    print 80*'#'
    print 'Going to calculate F-ratio in bin', binname
    print 80*'#'

    mcData = True # take data from file or generate toys

    if mcData:
        print 'Data is taken from toys!'

    # get full BKG, QCD and EWK histos
    (hData,hEWKsele,hQCDsele,hQCDanti) = getHistsFromFile(tfile, binname,  mcData)

    # Print some info
    print 10*'-'
    print 'Number of events in anti-selected bin'
    print 'QCD: ', hQCDanti.Integral(), '+/-', getHistIntError(hQCDanti)
    print 10*'-'
    print 'Number of events in selected bin'
    print 'Data:', hData.Integral()
    print 'EWK: ', hEWKsele.Integral()
    print 'QCD: ', hQCDsele.Integral(), '+/-', getHistIntError(hQCDsele)
    print 10*'-'

    # Create Lp var from hist
    lp = getVarFromHist(hData, "Lp")

    _varStore['Lp'] = lp

    # Deal with data (pseudo or real)
    data = getDataFromHist(hData,lp, mcData)

    _dhStore['data_'+binname] = data

    # take anti from ST-inclusive QCD:
    if inclTemplate:
        import re
        incName = re.sub('ST[0-9]_','',binname)
        print 'Using template', incName, 'instead of', binname
        hQCDanti = _hStore['QCDanti_'+incName]

    # Get QCD prediction in the selected region
    nQCD = getQCDfromFit(data,hQCDanti,hEWKsele,lp)

    nQCDsel = nQCD.getValV()
    nQCDselErr = nQCD.getError()
    print 'Fit result:'
    print 'QCD: ', nQCDsel, '+/-', nQCDselErr


    # get correct QCD anti
    if inclTemplate:
        hQCDanti = _hStore['QCDanti_'+binname]


    if not doClosure:
        #determine F ratio as selected(fit)/anti-selected(data/mc)
        fRatio = nQCDsel/hQCDanti.Integral()

        nQCDanti =  hQCDanti.Integral()
        nQCDantiErr = getHistIntError(hQCDanti)

        # calculate error
        fRatioErr = fRatio*sqrt(nQCDselErr/nQCDsel*nQCDselErr/nQCDsel + nQCDantiErr/nQCDanti*nQCDantiErr/nQCDanti)
    else:
        print '#!CLOSURE: F-ratio is QCD selected(fit)/selected(data/mc)'
        #determine F ratio as selected(fit)/selected(data/mc)
        fRatio = nQCDsel/hQCDsele.Integral()

        nQCDsele =  hQCDsele.Integral()
        nQCDseleErr = getHistIntError(hQCDsele)

        # calculate error
        fRatioErr = fRatio*sqrt(nQCDselErr/nQCDsel*nQCDselErr/nQCDsel + nQCDseleErr/nQCDsele*nQCDseleErr/nQCDsele)

    print 'F_ratio =', fRatio, '+/-', fRatioErr

    if doPlot:
        print 10*'-'
        canv = plotHists(binname, inclTemplate)

    return (fRatio,fRatioErr)

def plotFratios(resList, isClosure = False):

    nbins = len(resList)
    hist = TH1F('hRatios','F-Ratios',nbins,0,nbins)

    for i,(bin,val,err) in enumerate(resList):
        #print bin, val
        #binName = bin[:bin.find("_NJ")]
        binName = bin
        hist.GetXaxis().SetBinLabel(i+1,binName)
        hist.SetBinContent(i+1,val)
        hist.SetBinError(i+1,err)

    _hStore[hist.GetName()] = hist
    hist.SetStats(0)

    canv=TCanvas(hist.GetName(),hist.GetTitle(),800,600)

    # style
    hist.SetMarkerStyle(20)
    hist.Draw('E1p')

    if not isClosure:
        hist.GetYaxis().SetTitle("F_{sel-to-anti} QCD")
        #hist.GetYaxis().SetRangeUser(0.,0.8)
    else:
        hist.GetYaxis().SetTitle("F_{fit-to-mc} QCD_{selected}")
        #hist.GetYaxis().SetRangeUser(0.4,1.3)

        # unity line
        line = TLine(0,1,hist.GetNbinsX(),1)
        line.SetLineStyle(2)
        line.Draw('same')
        SetOwnership(line,0)

    gPad.Update()

    _canvStore[canv.GetName()] = canv

    return hist

if __name__ == "__main__":

    # disable RooFit info
    RooMsgService.instance().setGlobalKillBelow(RooFit.WARNING)

    # OPTIONS
    doClosure = False
    doPlots = True
    inclTemplate = True

    infileName = "../lp_only_plots.root"

    if len(sys.argv) > 1:
        infileName = sys.argv[1]
        print 'Infile is', infileName
    else:
        print 'No file name is given'
        exit(0)

    tfile = TFile(infileName,"READ")

    # select bin
    binNames = ['incl']
    #binNames += ['HT500toInf','HT500to1000','HT750to1000','HT500to750']
    #binNames = ['incl','NB1','NB2','NB3']
    #binNames = ['NJ45']
    #binNames += ['NJ45','ST0_NJ45','ST1_NJ45','ST2_NJ45','ST3_NJ45','ST4_NJ45']
    #binNames += ['NJ68','ST0_NJ68','ST1_NJ68','ST2_NJ68','ST3_NJ68','ST4_NJ68']
    #binNames += ['NJ6inf','ST0_NJ6inf','ST1_NJ6inf','ST2_NJ6inf','ST3_NJ6inf','ST4_NJ6inf']
    binNames += ['NJ45','NJ68','ST0_NJ45','ST0_NJ68','ST1_NJ45','ST1_NJ68','ST2_NJ45','ST2_NJ68','ST3_NJ45','ST3_NJ68','ST4_NJ45','ST4_NJ68']

    resList = []

    for binName in binNames:
        (fRat,err) = getQCDratio(tfile,binName, doPlots, doClosure, inclTemplate)
        resList.append((binName,fRat,err))

    print 80*'='

    # Plot results in one histo
    hRatio = plotFratios(resList,doClosure)

    print 'Finished fitting. Saving Canvases...'

    # save plots to root file
    outfile = TFile('plots/'+os.path.basename(infileName).replace('.root','_plots.root'),'RECREATE')
    print 'Saving plots to file', outfile.GetName()

    for canvKey in _canvStore:
        canv = _canvStore[canvKey]
        canv.SaveAs('plots/'+canv.GetName()+'.png')
        canv.Write()

    # save ratio hist
    hRatio.Write()

    print 40*'/\\'
    print 'Compact results'
    for (bin,fRat,err) in resList:
        #(fRat, err) = ratioDict[bin]
        print 'Bin\t %s has F ratio\t %f +/- %f (%f %% error)' %(bin, fRat, err, 100*err/fRat)
        #print '%s\t%.3f\t%.3f' % (bin, fRat, err)
    print 40*'\\/'

    if '-b' not in sys.argv:
        # wait for input
        answ = []
        while 'q' not in answ:
            answ.append(raw_input("Enter 'q' to continue: "))

    # close
    tfile.Close()
    outfile.Close()
