#!/usr/bin/python

import sys
import os
#sys.argv.append( '-b' )

from ROOT import *
from array import array

_canvStore = []
_histStore = []

_colorList = [2,3,4,6,7,8,9]


def setColors(histList):

    #    colorList = [3,2,ROOT.kGreen-2,1]
    colorList = [1,2,3,4]

    for ind,hist in enumerate(histList):
        hist.SetLineColor(colorList[ind])
        hist.SetMarkerColor(colorList[ind])

def custHists():
    ## loop over all saved hists
    for hist in _histListCR+_histListSR:

        hname = hist.GetName()
        ## common settings
        hist.SetStats(0)
        hist.SetFillColor(0)
        hist.SetLineWidth(2)
        hist.SetMarkerStyle(0)

        ## rebin
        hist.Rebin(2)

        htitle = ''
        ## NJ bins
        if 'NJ45'in hname:
            htitle = 'Nj #in [4,5] '
        elif 'NJ6i'in hname:
            htitle = 'Nj #geq 6 '
        elif 'NJ68'in hname:
            htitle = 'Nj #in [6,8] '
        elif 'NJ9i'in hname:
            htitle = 'Nj #geq 9 '

        ## HT bins
        if 'HT500750'in hname:
            htitle += '500 < HT < 750'
        elif 'HT7501250'in hname:
            htitle += '750 < HT < 1250'
        elif 'HT1250'in hname:
            htitle += 'HT > 1250'
        elif 'HT500'in hname:
            htitle += 'HT > 500'

        hist.SetTitle(htitle)

        print hname, htitle

        setColors(_histListSR)
        setColors(_histListCR)

    return 1

def getHists(tree, var = 'MET'):

    # variable
    # cut
    cuts = 'HLT_SingleMu'#'MET > 250'
    # plot option
    plotOpt = 'e1'

    refTrig = ''#HLT_SingleMu'
    #testTrig = ['HLT_SingleMu','HLT_SingleEl','HLT_HT350','HLT_MET170']
    testTrig = ['HLT_HT900','HLT_HT350','HLT_MET170','HLT_MuHT400MET70']

    # names
    if refTrig != '':
        refName = refTrig.replace('HLT_','')
    else:
        refName = 'Reference'

    refName += var

    rname = 'h' + refName
    cname = 'canv_Ref' + refName + var
    ctitle = 'Plots for reference:' + refTrig

    if cuts != '':
        ctitle += ' cut: ' + cuts

    htitle = ctitle

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

    _histStore.append(hRef)

    # loop over test triggers:
    for ind, trig in enumerate(testTrig):

        trigName = trig.replace('HLT_','')
        hname = 'h' + trigName

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

        _histStore.append(hTest)

    # axis set up
    hRef.SetStats(0)
    hRef.GetXaxis().SetTitle(var)
    #hRef.GetYaxis().SetRangeUser(0,2)
    canv.SetLogy()

    # legend
    leg = canv.BuildLegend()
    leg.SetFillColor(0)

    gPad.Update()

    _canvStore.append(canv)

    return 1

def plotEff():

    hRef = _histStore[0]
    hRefEff = hRef.Clone(hRef.GetName()+'Eff')
    # set reference eff to 1
    hRefEff.Divide(hRef)

    cname = 'canv_Eff_Ref' + hRefEff.GetName()
    ctitle = 'Eff for reference:' + hRefEff.GetName()

    # make canvas
    canv = TCanvas(cname,ctitle,800,800)

    hRefEff.Draw()
    plotOpt = 'same'

    # loop over test
    for ind,hist in enumerate(_histStore[1:]):
        hname = (hist.GetName()).replace('h','hEff')

        #hEff = hist.Clone(hname)
        #hEff.Divide(hRef)
        hEff = TEfficiency(hist,hRef)
        hEff.SetName(hname)

        # style
        hEff.SetLineColor(_colorList[ind])
        hEff.SetFillColor(0)

        print 'Drawing', hname, plotOpt
        hEff.Draw(plotOpt)
        if 'same' not in plotOpt: plotOpt += 'same'

        _histStore.append(hEff)

    # legend
    leg = canv.BuildLegend()
    leg.SetFillColor(0)

    # axis set up
    hRefEff.SetStats(0)
    #hRef.GetXaxis().SetTitle(var)
    hRefEff.GetYaxis().SetRangeUser(0,1.5)
    #canv.SetLogy()


    _histStore.append(hRef)
    _canvStore.append(canv)

    gPad.Update()

    return 1

if __name__ == "__main__":

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

    tree = tfile.Get('sf/t')

    print 'Entries in tree:', tree.GetEntries()

    getHists(tree,'HT')
    plotEff()

    print _histStore

    ## wait
    answ = raw_input("Enter 'q' to exit: ")


    ## save canvases to file
    for canv in _canvStore:
        pdir = 'plots/'
        canv.SaveAs(pdir+canv.GetName()+'.pdf')
        canv.Write()

    tfile.Close()
    outfile.Close()
