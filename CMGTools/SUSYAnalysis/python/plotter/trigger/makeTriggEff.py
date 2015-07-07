#!/usr/bin/python

import sys
import os
#sys.argv.append( '-b' )

from ROOT import *
from array import array

_canvStore = []
_histStore = {}
_hEffStore = {}

_colorList = [2,3,4,6,7,8,9]


def setColors(histList):

    #    colorList = [3,2,ROOT.kGreen-2,1]
    colorList = [1,2,3,4]

    for ind,hist in enumerate(histList):
        hist.SetLineColor(colorList[ind])
        hist.SetMarkerColor(colorList[ind])

def getHists(tree, var = 'MET', cuts = '', refTrig = ''):

    # variable
    histPrefix = 'h' + var + '_'

    # cut
    #cuts = 'nTightLeps == 1 && nVetoLeps == 0'#HLT_SingleMu'#'MET > 250'
    cuts = 'HLT_SingleMu'

    # plot option
    plotOpt = 'e1'

    refTrig = ''
    #testTrig = ['HLT_SingleMu','HLT_SingleEl','HLT_HT350','HLT_MET170']
    #testTrig = ['HLT_HT900','HLT_HT350','HLT_MET170','HLT_MuHT400MET70']
    testTrig = ['HT900', 'MuHad']

    # prepend HLT name
    testTrig = ['HLT_'+name for name in testTrig]

    # names
    if refTrig != '':
        refName = refTrig.replace('HLT_','')
    else:
        refName = 'Ref'

    rname = histPrefix + refName
    cname = 'canv_Ref' + refName + var
    ctitle = 'Plots for reference:' + refTrig

    if cuts != '':
        ctitle += ' cut: ' + cuts

    htitle = 'Reference' + refTrig#ctitle

    # make canvas
    canv = TCanvas(cname,ctitle,800,800)

    # make hist
    if var == 'MET':
        hRef = TH1F(rname,htitle,100,0,1000)
    elif var == 'HT':
        hRef = TH1F(rname,htitle,100,0,3000)
    else:
        hRef = TH1F(rname,htitle,100,0,1000)

    # make reference plot
    tree.Draw(var + '>>' + hRef.GetName(),cuts,plotOpt)

    _histStore[hRef.GetName()] = hRef

    # loop over test triggers:
    for ind, trig in enumerate(testTrig):

        trigName = trig.replace('HLT_','')
        hname = 'h' + var + '_' + trigName

        hTest = hRef.Clone(hname)
        hTest.SetTitle(trigName)

        hTest.SetLineColor(_colorList[ind])

        # cuts

        if cuts != '':
            tcuts = cuts +  '&&' + trig
        else:
            tcuts = trig

        tree.Draw(var + '>>' + hTest.GetName(),tcuts,plotOpt+'same')

        #hTest.Divide(hRef)

        _histStore[hTest.GetName()] = hTest

    # axis set up
    hRef.SetStats(0)
    hRef.GetXaxis().SetTitle(var)
    #hRef.GetYaxis().SetRangeUser(0,2)
    canv.SetLogy()

    #hRef.SetTitle(ctitle)

    # legend
    leg = canv.BuildLegend()
    leg.SetFillColor(0)

    gPad.Update()

    _canvStore.append(canv)

    return 1

def plotEff(var = 'HT', refName = 'Ref'):

    # variable
    histPrefix = 'h' + var + '_'
    refName = histPrefix + refName

    print refName

    hRef = _histStore[refName]
    hRefEff = hRef.Clone(hRef.GetName()+'Eff')
    # set reference eff to 1
    hRefEff.Divide(hRef)

    cname = 'canv_Eff_Ref' + hRefEff.GetName()
    ctitle = 'Eff for reference:' + hRefEff.GetName()

    # make canvas
    canv = TCanvas(cname,ctitle,800,800)


    # set reference eff to 1
    for bin in range(1,hRefEff.GetNbinsX()+1):
        hRefEff.SetBinContent(bin,1)
        hRefEff.SetBinError(bin,0)

    hRefEff.Draw()
    plotOpt = 'same'

    nameList = []

    for hname in _histStore.keys():

        obj = _histStore[hname]

        hname = str(hname)
        #print hname, obj.ClassName()

        if 'TH1' not in obj.ClassName(): continue

        # filter out hists
        if histPrefix not in hname: continue
        if refName in hname: continue

        nameList.append(hname)

    # loop over test
    for ind,hname in enumerate(nameList):

        hist = _histStore[hname]

        # filter out hists
        #if histPrefix not in hname: continue
        #if 'Ref' in hname: continue

        htitle = hname.replace(histPrefix,'')
        hname = hname.replace('h','hEff')

        print 'Drawing', hname, 'from', hRef.GetName()

        ## Divide
        #hEff = hist.Clone(hname)
        #hEff.Divide(hRef)

        ## TEfficiency
        hEff = TEfficiency(hist,hRef)
        hEff.SetName(hname)
        hEff.SetTitle(htitle)

        # style
        hEff.SetLineColor(_colorList[ind])
        hEff.SetFillColor(0)

        hEff.Draw(plotOpt)
        if 'same' not in plotOpt: plotOpt += 'same'

        _hEffStore[hname] = hEff

    # remove refEff
    #gPad.GetListOfPrimitives().Remove(hRef)

    # legend
    leg = canv.BuildLegend()
    leg.SetFillColor(0)

    # axis set up
    hRefEff.SetStats(0)
    #hRef.GetXaxis().SetTitle(var)
    hRefEff.GetYaxis().SetRangeUser(0,1.5)
    #canv.SetLogy()

    #leg.GetListOfPrimitives().Remove(hRefEff)

    _hEffStore[hRefEff.GetName] = hRefEff
    _canvStore.append(canv)

    gPad.Update()

    return 1

if __name__ == "__main__":

    ## remove '-b' option
    if '-b' in sys.argv:
        sys.argv.remove('-b')

    if len(sys.argv) > 1:
        fileName = sys.argv[1]
        print '#fileName is', fileName
    else:
        print '#No file names given'
        exit(0)

    tfile  = TFile(fileName, "READ")

    if len(sys.argv) > 2:
        outName = sys.argv[2]
    else:
        print '#No out file name is given'
        outName = (os.path.basename(fileName)).replace('.root','_plots.root')
        print '#> Out file name is', outName

    outfile = TFile(outName, "RECREATE")

    if not tfile:
        print "Couldn't open the file"
        exit(0)

    # for friend trees
    tree = tfile.Get('sf/t')

    # for cmg trees
    #tree = tfile.Get('tree')

    print 'Entries in tree:', tree.GetEntries()

    for var in ['HT','MET','ST']:
        getHists(tree,var)
        #print 'Store', _histStore
        plotEff(var)

    #print 'HistStore', _histStore
    #print 'HEffStore', _hEffStore

    ## wait
    answ = raw_input("Enter 'q' to exit: ")

    ## save canvases to file
    for canv in _canvStore:
        pdir = 'plots/'
        canv.SaveAs(pdir+canv.GetName()+'.pdf')
        canv.Write()

    tfile.Close()
    outfile.Close()
