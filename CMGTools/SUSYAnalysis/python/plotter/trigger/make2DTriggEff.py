#!/usr/bin/python

import sys
import os
#sys.argv.append( '-b' )

#import pystyle.CMS_lumi
import CMS_lumi

CMS_lumi.writeExtraText = 1
#CMS_lumi.extraText = "t#bar{t} LO, 25ns"#"Spring15 MC"
CMS_lumi.extraText = "Simulation"#"Spring15 MC"
iPos = 0
if( iPos==0 ): CMS_lumi.relPosX = 0.12

from ROOT import *
from array import array

gStyle.SetOptTitle(0)
gStyle.SetOptStat(0)
gStyle.SetPadTopMargin(0.05)

_canvStore = []
_histStore = {}
_hEffStore = {}

_fitrStore = []

_colorList = [2,8,4,9,7,3,6] + range(10,50)

def varToLabel(var):

    label = var

    if 'pt' in var:
        label = 'p_{T}(lep)'
    elif 'MET' in var:
        label = 'E_{T}^{miss}'
    elif 'T' in var:
        label = var.replace('T','_{T}')

    return label

def getLegend(pos = 'ne'):
    if pos == 'ne':
        leg = TLegend(0.4,0.7,0.9,0.9)
    elif pos == 'log':
        leg = TLegend(0.6,0.8,0.99,0.99)
    elif pos == 'roc':
        leg = TLegend(0.15,0.2,0.7,0.4)
    elif pos == 'fit':
        leg = TLegend(0.15,0.75,0.5,0.9)
    elif pos == '2d':
        leg = TLegend(0.3,0.75,0.85,0.9)

    leg.SetBorderSize(1)
    leg.SetTextFont(62)
    leg.SetTextSize(0.03321678)
    leg.SetLineColor(1)
    leg.SetLineStyle(1)
    leg.SetLineWidth(1)
    leg.SetFillColor(0)
    leg.SetFillStyle(1001)

    return leg

def turnon_func(x, par):

    halfpoint = par[0]
    #slope = max(par[1],0.00001)
    width = max(par[1],1)
    plateau = par[2]

    #offset = par[3]
    #plateau = 1.0
    offset = 0

    pt = TMath.Max(x[0],0.000001)

    arg = 0
    #print pt, halfpoint, width
    #arg = (pt - halfpoint)/(TMath.Sqrt(pt)*slope)
    arg = (pt - halfpoint) / (width * TMath.Sqrt(2))

    fitval = offset + plateau * 0.5 * (1 + TMath.Erf(arg))
    #fitval = offset + plateau * TMath.Erfc(arg)

    return fitval

def textBox():

    pt = TPaveText(.05,.1,.95,.8);

    pt.AddText("A TPaveText can contain severals line of text.");
    pt.AddText("They are added to the pave using the AddText method.");
    pt.AddLine(.0,.5,1.,.5);
    pt.AddText("Even complex TLatex formulas can be added:");
    pt.AddText("F(t) = #sum_{i=-#infty}^{#infty}A(i)cos#[]{#frac{i}{t+i}}");

    return pt


def cutsToString(cutList):

    cutstr = ''

    for i, cut in enumerate(cutList):
        cutstr += cut

        if i != len(cutList)-1: cutstr += ' && '

    return cutstr

def get2DHistsFromTree(tree, tvar = ('MET','HT'), refTrig = '', cuts = '', testTrig = '', maxEntries = -1, lumi = -1):

    # decipher var tuple
    (var1,var2) = tvar
    var = var1+'vs'+var2

    # maximum number of entries to process
    if maxEntries == -1:
        maxEntries = tree.GetEntries()

    # histogram name prefix
    histPrefix = 'h' + var + '_'

    # plot option
    plotOpt = 'colz'

    # histogram list
    histList = []

    # prepend HLT name
    testTrig = ['HLT_'+name.replace('HLT_','') for name in testTrig]

    # names
    if refTrig != '':
        refName = refTrig.replace('HLT_','')
    else:
        refName = 'PreSel'

    ## name replacement
    refName = refName.replace('SingleMu','IsoMu27')
    refName = refName.replace('SingleEl','El32')
    refName = refName.replace('HTMET','HT350MET120')

    rname = histPrefix + refName

    cname = var + '_' + refName
    ctitle = 'Plots for reference:' + refName

    if cuts != '':
        ctitle += ' cut: ' + cuts

    if refTrig != '':
        cuts += ' && HLT_' + refTrig.replace('HLT_','')
        htitle = refTrig.replace('HLT_','')#'Ref: ' + refTrig
    else:
        htitle = 'Preselection'

    print 'Going to draw', ctitle

    # make canvas
    canv = TCanvas(cname,ctitle,800,800)

    # make hist
    nbins = 50

    varBinSize = False
    pt_bins = range(0,30,2) + range(30,70,5) + range(70,150,10) + range (150,250,25) + range(250,350,50)
    met_bins = range(0,200,10) + range(200,400,50) + range(400,700,100)
    ht_bins = range(0,200,10) + range(200,400,50) + range(400,1000,100) + range(1000,1750,250)

    if var1 == 'MET' and var2 == 'HT':
        hRef = TH2F(rname,htitle,nbins,0,1500,nbins,0,600)
        hRef = TH2F(rname,htitle,len(ht_bins)-1,array('f',ht_bins),len(met_bins)-1,array('f',met_bins))
    elif var1 == 'HT' and var2 == 'MET':
        hRef = TH2F(rname,htitle,len(met_bins)-1,array('f',met_bins),len(ht_bins)-1,array('f',ht_bins))
    elif 'pt' in var2 and var1 == 'MET':
        hRef = TH2F(rname,htitle,len(pt_bins)-1,array('f',pt_bins),len(met_bins)-1,array('f',met_bins))
    elif 'pt' in var2 and var1 == 'HT':
        hRef = TH2F(rname,htitle,len(pt_bins)-1,array('f',pt_bins),len(ht_bins)-1,array('f',ht_bins))
    else:
        hRef = TH2F(rname,htitle,nbins,0,1500,nbins,0,600)

    # make reference plot
    # lumi scale. -1 means use MC counts
    doLumi = True if lumi != -1 else False

    varstr = var1 + ':' + var2

    if not doLumi:
        tree.Draw(varstr + '>>' + hRef.GetName(),cuts,plotOpt, maxEntries)
        print 'Drawing', hRef.GetName(), 'with cuts', cuts
        hRef.GetYaxis().SetTitle('MC counts')
    else:
        hRef.GetYaxis().SetTitle('Events')
        hRef.Sumw2()

        wt = 600*lumi/float(maxEntries)
        print 'Weight for %2.2f lumi and maxEntries %10.f is %f' %(lumi, maxEntries,wt)
        weight = str(wt) + ' * Xsec'
        if cuts != '': wcuts = weight + '*(' + cuts + ')'
        else: wcuts = weight
        print 'Drawing', hRef.GetName(), 'with cuts', wcuts

        tree.Draw(varstr + '>>' + hRef.GetName(),wcuts,plotOpt, maxEntries)
        hRef.SetMaximum(hRef.GetMaximum() * 2)

    hRef.GetZaxis().SetRangeUser(0,1)
    hRef.SetLineColor(1)
    # axis set up
    hRef.SetStats(0)
    hRef.GetXaxis().SetTitle(varToLabel(var2))
    hRef.GetYaxis().SetTitle(varToLabel(var1))
    hRef.GetYaxis().SetTitleOffset(1.2)
    #canv.SetLogy()

    gPad.Update()

    _histStore[hRef.GetName()] = hRef
    histList.append(hRef)

    # loop over test triggers:
    for ind, trig in enumerate(testTrig):

        # for OR and AND test triggers
        if '||' in trig:
            tnames = trig.replace('HLT_','').split('||')
            trigName = tnames[0]
            trig = '(' + 'HLT_'+tnames[0].replace('HLT_','')

            for name in tnames[1:]:
                trigName += 'OR' + name
                trig += '||' + 'HLT_'+name.replace('HLT_','')
            trig += ')'

            print trigName
            print trig
        else:
            trigName = trig.replace('HLT_','')

        trigName = trigName.replace('SingleMu','IsoMu27')
        trigName = trigName.replace('SingleEl','El32')
        trigName = trigName.replace('HTMET','HT350MET120')

        hname = 'h' + var + '_' + trigName

        hTest = hRef.Clone(hname)
        hTest.SetTitle(trigName)

        hTest.SetLineColor(_colorList[ind])

        # cuts

        if cuts != '':
            tcuts = cuts + ' && ' + trig
        else:
            tcuts = trig

        print 'Drawing', hTest.GetName(), 'with cuts', tcuts

        # lumi scale
        if not doLumi:
            tree.Draw(varstr + '>>' + hTest.GetName(),tcuts,plotOpt+'same', maxEntries)
        else:
            #hTest.Sumw2()
            if tcuts != '': wtcuts = weight + '*(' + tcuts + ')'
            else: wtcuts = weight
            tree.Draw(varstr + '>>' + hTest.GetName(),wtcuts,plotOpt+'same', maxEntries)

        gPad.Update()

        #hTest.Divide(hRef)
        #hTest.Draw("colz")

        gPad.Update()

        _histStore[hTest.GetName()] = hTest
        histList.append(hTest)

    '''
    # if var bin sizes
    if varBinSize:

        # add /bin in Y axis label
        hRef.GetYaxis().SetTitle(hRef.GetYaxis().GetTitle() + '/bin')

        for hist in histList:
            for bin in range(1,hist.GetNbinsX()+1):
                binC = hist.GetBinContent(bin)
                binW = hist.GetBinWidth(bin)

                binV = binC/binW
                #print binC, binW, binV

                hist.SetBinContent(bin, binV)

    #hRef.SetTitle(ctitle)
    '''

    # legend
    leg = canv.BuildLegend()
    leg.SetFillColor(0)
    #leg.SetHeader(ctitle.replace('&&','\n'));

    ## CMS LUMI
    if lumi != -1:
        CMS_lumi.lumi_13TeV = str(lumi) + ' fb^{-1}'
    else:
        CMS_lumi.lumi_13TeV = 'MC'
    CMS_lumi.CMS_lumi(canv, 4, iPos)

    gPad.Update()

    #_canvStore.append(canv)

    return histList

def plot2DEff(histList,  tvar = ('MET','HT')):

    ## histList: [hReference, hTest1, hTest2,...]

    # decipher var tuple
    (var1,var2) = tvar
    var = var1+'vs'+var2

    # hist prefix
    histPrefix = 'h' + var + '_'

    # reference hist should be first
    hRef = histList[0]

    # loop over test
    #for ind,hname in enumerate(nameList):
    for ind,hist in enumerate(histList[1:]):

        hname = hist.GetName()
        cname = hname.replace('h'+var,var) + '_Eff_Ref'+ hRef.GetName().replace('h'+var,'')

        ctitle = 'Eff for reference:' + hRef.GetName()

        # make canvas
        canv = TCanvas(cname,ctitle,800,800)
        # style

        # legend
        leg = getLegend('2d')
        leg.SetHeader('Reference: ' + hRef.GetName().replace('h'+var+'_',''))

        htitle = hname.replace(histPrefix,'')
        hname = hname.replace('h','hEff')

        print 'Drawing', hname, 'from', hRef.GetName()

        ## Divide
        hEff = hist.Clone(hname)
        hEff.Divide(hRef)
        hEff.GetZaxis().SetRangeUser(0,1)

        plotOpt = 'colz'
        hEff.Draw(plotOpt)

        leg.AddEntry(hEff,hEff.GetTitle(),'f')
        leg.Draw()
        SetOwnership( leg, 0 )

        # LT LINE
        if 'MET' in hname and 'Lep' in hname:
            cutline = TLine(0,250,250,0)
            cutline.SetLineWidth(5)
            cutline.SetLineStyle(4)
            cutline.Draw()
            SetOwnership(cutline,0)

        ## CMS lumi
        CMS_lumi.CMS_lumi(canv, 4, iPos)

        gPad.Update()

        _hEffStore[hname] = hEff
        _canvStore.append(canv)

    return 1

def make2DEffPlots(tree, lumi = -1, maxEntries = -1, varList = [], refTrig = '', testTrig = [], cuts = ''):

    # lumi dir
    if lumi != -1:
        lumiDir = 'Lumi'+str(lumi)+'fb/'
    else:
        lumiDir = 'LumiMC/'

    # make suffix from testTrigNames
    suffix = 'test'
    for trig in testTrig:
        suffix +=  '_' + trig.replace('||','OR')
    suffix = ''

    for var in varList:
        #for ref in refTrig[:1]:

        histList = get2DHistsFromTree(tree,var,refTrig, cuts, testTrig, maxEntries, lumi)
        plot2DEff(histList, var)
        saveCanvases(lumiDir,suffix)

    return 1

def saveCanvases(pdir = '', extraName = ''):

    ## save canvases to file
    extList = ['.png','.pdf']

    pdir = 'plots/2d/' + pdir
    prefix = ''
    suffix = '_' + extraName

    ## wait
    if not _batchMode:
        answ = raw_input("'Enter' to proceed (or 'q' to exit): ")
        if 'q' in answ: exit(0)

    cdir = os.path.dirname(pdir + prefix)
    print 'Canvas dir is', cdir

    if not os.path.exists(cdir):
        os.makedirs(cdir)

    cdir += '/'

    # make output file
    outName = cdir + 'plots_'+ extraName +'.root'
    ofile = TFile(outName,'RECREATE')

    for canv in _canvStore:

        if 'Eff' not in canv.GetName(): continue

        for ext in extList:
            cname = canv.GetName().replace('LepGood1_pt','LepPt')
            cname = cdir + cname+ suffix + ext
            canv.SaveAs(cname)
        canv.Write()

    # empty stores for further use
    del _canvStore[:]
    _histStore.clear()
    _hEffStore.clear()
    del _fitrStore[:]

    ofile.Close()

    return 1


if __name__ == "__main__":

    ## remove '-b' option
    _batchMode = False

    if '-b' in sys.argv:
        sys.argv.remove('-b')
        _batchMode = True

    if len(sys.argv) > 1:
        fileName = sys.argv[1]
        print '#fileName is', fileName
    else:
        print '#No file names given'
        exit(0)

    tfile  = TFile(fileName, "READ")

    '''
    if len(sys.argv) > 2:
        outName = sys.argv[2]
    else:
        print '#No out file name is given'
        outName = (os.path.basename(fileName)).replace('.root','_plots.root')
        print '#> Out file name is', outName

    outfile = TFile(outName, "RECREATE")
    '''

    if not tfile:
        print "Couldn't open the file"
        exit(0)

    ## Get tree from file
    # for friend trees
    tree = tfile.Get('sf/t')
    # for cmg trees
    #tree = tfile.Get('tree')

    nentries = tree.GetEntries()
    print 'Entries in tree:', nentries

    ## SETTINGS
    # max entries to process
    maxEntries = -1#100000
    # do efficiency fit
    doFit = True#False
    # luminosity scaling, -1 takes MC counts
    lumi = -1

    '''
    ## DEFINE plots
    # variable list
    #varList = ['HT']#,'MET','ST']
    #varList = ['HT','LepGood1_pt']#,'LepGood1_eta']
    varList = ['LepGood1_pt']

    # TEST triggers
    #testTrig = ['SingleMu','SingleEl','HT350','MET170']
    #testTrig = ['HT350','HT900','HTMET','MET170']#,'MuHT400MET70']
    #testTrig = ['HT350','HT600','HT900']
    #testTrig = ['MuHT400MET70']
    #testTrig = ['HT900', 'MuHad']
    #testTrig = ['HLT_SingleMu', 'HLT_MuNoIso', 'HLT_MuHad', 'HLT_MuHT600', 'HLT_MuHT400MET70','HLT_MuMET120', 'HLT_MuHT400B']
    #testTrig = ['HLT_SingleEl', 'HLT_ElNoIso', 'HLT_ElHad', 'HLT_EleHT600','HLT_EleHT400MET70','HLT_EleHT200', 'HLT_EleHT400B']
    #testTrig = ['HLT_SingleEl','HLT_ElNoIso','HLT_EleHT600']
    #testTrig = ['HLT_SingleMu','HLT_MuNoIso','HLT_MuHT600']
    '''

    # muon
    cuts = 'nTightMu >= 1 && LepGood1_pt > 25'
    refTrig = 'SingleMu'
    testTrig = ['HTMET','MuHT400MET70']
    var = ('MET','HT')
    varList = [var]
    #make2DEffPlots(tree, lumi, maxEntries, varList, refTrig, testTrig, cuts)

    # electron
    cuts = 'nTightEl >= 1 && LepGood1_pt > 25'
    refTrig = 'SingleEl'
    testTrig = ['HTMET','EleHT400MET70']
    var = ('MET','HT')
    varList = [var]
    make2DEffPlots(tree, lumi, maxEntries, varList, refTrig, testTrig, cuts)


    ## LepPt vs MET
    var = ('MET','LepGood1_pt')
    varList = [var]

    ## Muons
    cuts = 'nTightMu >= 1 && LepGood1_pt > 5 && HT > 500'
    refTrig = ''
    testTrig = ['Mu50NoIso||MuHT400MET70','Mu50NoIso','MuHT400MET70']
    #make2DEffPlots(tree, lumi, maxEntries, varList, refTrig, testTrig, cuts)

    ## Electrons
    cuts = 'nTightEl >= 1 && LepGood1_pt > 5 && HT > 500'
    refTrig = 'HT350'
    testTrig = ['ElNoIso||EleHT400MET70','ElNoIso','EleHT400MET70']

    #make2DEffPlots(tree, lumi, maxEntries, varList, refTrig, testTrig, cuts)

    '''
    #############
    # LT: Muon
    #############

    varList = ['LT']
    cuts = 'nTightMu == 1 && LepGood1_pt > 25 && HT > 500'
    refTrig = ''
    #testTrig = ['Mu50NoIso','MuHT400MET70','Mu50NoIso||MuHT400MET70']
    testTrig = ['Mu50NoIso||MuHT400MET70']
    makeEffPlots(tree, lumi, maxEntries, doFit, varList, refTrig, testTrig, cuts)

    #############
    # LT: Electron
    #############

    varList = ['LT']
    cuts = 'nTightEl == 1 && LepGood1_pt > 25 && HT > 500'
    refTrig = ''
    #testTrig = ['ElNoIso','EleHT400MET70','ElNoIso||EleHT400MET70']
    testTrig = ['ElNoIso||EleHT400MET70']
    makeEffPlots(tree, lumi, maxEntries, doFit, varList, refTrig, testTrig, cuts)

    #############
    # Lepton legs
    #############

    ## muons
    varList = ['LepGood1_pt']
    cuts = 'nTightMu == 1 && LepGood1_pt > 5 && HT > 500 && MET > 200'
    refTrig = ''
    testTrig = ['Mu50NoIso','MuHT400MET70']
    makeEffPlots(tree, lumi, maxEntries, doFit, varList, refTrig, testTrig, cuts)

    varList = ['LepGood1_pt']
    cuts = 'nTightMu == 1 && LepGood1_pt > 5 && HT > 500 && MET > 200'
    refTrig = 'HTMET'
    testTrig = ['Mu50NoIso','MuHT400MET70']
    makeEffPlots(tree, lumi, maxEntries, doFit, varList, refTrig, testTrig, cuts)

    ## electrons
    varList = ['LepGood1_pt']
    cuts = 'nTightEl == 1 && LepGood1_pt > 5 && HT > 500 && MET > 200'
    refTrig = ''
    testTrig = ['ElNoIso','EleHT400MET70']
    makeEffPlots(tree, lumi, maxEntries, doFit, varList, refTrig, testTrig, cuts)

    varList = ['LepGood1_pt']
    cuts = 'nTightEl == 1 && LepGood1_pt > 5 && HT > 500 && MET > 200'
    refTrig = 'HTMET'
    testTrig = ['ElNoIso','EleHT400MET70']
    makeEffPlots(tree, lumi, maxEntries, doFit, varList, refTrig, testTrig, cuts)

    ###############
    # Hadronic legs: Muon
    ###############

    varList = ['HT']
    cuts = 'nTightMu == 1 && LepGood1_pt > 55 && MET > 200'
    refTrig = ''
    testTrig = ['SingleMu','Mu50NoIso', 'MuHT400MET70']
    makeEffPlots(tree, lumi, maxEntries, False, varList, refTrig, testTrig, cuts)

    varList = ['HT']
    cuts = 'nTightMu == 1 && LepGood1_pt > 25 && MET > 200'
    refTrig = ''
    testTrig = ['MuHT400MET70']#,'MuHT600']
    makeEffPlots(tree, lumi, maxEntries, doFit, varList, refTrig, testTrig, cuts)

    varList = ['MET']
    cuts = 'nTightMu == 1 && LepGood1_pt > 25 && HT  > 500'
    refTrig = ''
    testTrig = ['MuHT400MET70']#,'MuHT600']
    makeEffPlots(tree, lumi, maxEntries, doFit, varList, refTrig, testTrig, cuts)

    varList = ['HT']
    cuts = 'nTightMu == 1 && LepGood1_pt > 25 && MET > 200'
    refTrig = 'SingleMu'
    #testTrig = ['SingleMu','Mu50NoIso','HLT_MuHT400MET70']
    testTrig = ['MuHT400MET70']#,'MuHT600']
    makeEffPlots(tree, lumi, maxEntries, doFit, varList, refTrig, testTrig, cuts)

    varList = ['MET']
    cuts = 'nTightMu == 1 && LepGood1_pt > 25 && HT  > 500'
    refTrig = 'SingleMu'
    testTrig = ['MuHT400MET70']#,'MuHT600']
    makeEffPlots(tree, lumi, maxEntries, doFit, varList, refTrig, testTrig, cuts)

    ###############
    # Hadronic legs: Electron
    ###############

    varList = ['HT']
    cuts = 'nTightEl == 1 && LepGood1_pt > 120 && MET > 200'
    refTrig = ''
    testTrig = ['SingleEl','ElNoIso', 'EleHT400MET70']
    makeEffPlots(tree, lumi, maxEntries, False, varList, refTrig, testTrig, cuts)

    varList = ['HT']
    cuts = 'nTightEl == 1 && LepGood1_pt > 25 && MET > 200'
    refTrig = ''
    testTrig = ['EleHT400MET70']
    makeEffPlots(tree, lumi, maxEntries, doFit, varList, refTrig, testTrig, cuts)

    varList = ['MET']
    cuts = 'nTightEl == 1 && LepGood1_pt > 25 && HT  > 500'
    refTrig = ''
    testTrig = ['EleHT400MET70']
    makeEffPlots(tree, lumi, maxEntries, doFit, varList, refTrig, testTrig, cuts)

    varList = ['HT']
    cuts = 'nTightEl == 1 && LepGood1_pt > 25 && MET > 200'
    refTrig = 'SingleEl'
    testTrig = ['EleHT400MET70']
    makeEffPlots(tree, lumi, maxEntries, doFit, varList, refTrig, testTrig, cuts)

    varList = ['MET']
    cuts = 'nTightEl == 1 && LepGood1_pt > 25 && HT  > 500'
    refTrig = 'SingleEl'
    testTrig = ['EleHT400MET70']
    makeEffPlots(tree, lumi, maxEntries, doFit, varList, refTrig, testTrig, cuts)
    '''

    ## wait
    if not _batchMode:
        answ = raw_input("'Enter' to exit: ")

    tfile.Close()
    #outfile.Close()
