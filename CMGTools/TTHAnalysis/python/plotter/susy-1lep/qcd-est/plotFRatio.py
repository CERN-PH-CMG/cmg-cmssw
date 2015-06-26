#!/usr/bin/python

import sys
import os
#sys.argv.append( '-b' )

from ROOT import *
from array import array

def loopFile(tfile):

    histList = []
    selSigList = []
    antiSigList = []

    histDict = {}

    for key in tfile.GetListOfKeys():
        hist = key.ReadObj()

        if 'TH1' in str(type(hist)):
            if 'signal' in hist.GetName():

                newName = hist.GetName().replace('_signal','')
                newName = newName.replace('Lp_','')

                if 'NJ9' in newName: continue

                if '_sel_' in hist.GetName():
                    selSigList.append(hist.Clone(newName))
                elif '_anti_' in hist.GetName():
                    antiSigList.append(hist.Clone(newName))

    tupleList = []

    for idx, hist in enumerate(antiSigList):

        binName = selSigList[idx].GetName().replace('sel_','')

        nSelect = selSigList[idx].Integral()
        nAnti = antiSigList[idx].Integral()

        if nSelect == 0 or nAnti == 0:
            nSelectErr = 0
            nAntiErr = 0
            ratio = 0
        else:
            nSelectErr = 1/sqrt(nSelect)
            nAntiErr = 1/sqrt(nAnti)
            ratio = nSelect / nAnti

        ratErr = ratio*sqrt(nSelectErr*nSelectErr+nAntiErr*nAntiErr)

        #histDict[binName] = ratio
        tupleList.append((binName,ratio,ratErr))

        if idx == 0:
            print "BinName\t nSelect\t nAnti\t Ratio\t RatioErr"
        print binName, '\t', nSelect, '\t', nAnti, '\t', ratio, ratErr

    #return histDict
    return tupleList

def plotList(tupleList):

    nbins = len(tupleList)
    hist = TH1F('hist','hist',nbins,0,nbins)

    for i,(bin,val,err) in enumerate(tupleList):
        #print bin, val
        hist.GetXaxis().SetBinLabel(i+1,bin)
        hist.SetBinContent(i+1,val)
        hist.SetBinError(i+1,err)

    return hist

if __name__ == "__main__":

    if len(sys.argv) > 1:
        infile = sys.argv[1]
        print 'Infile is', infile
    else:
        print 'No file name is given'

    tfile  = TFile(infile, "READ")

    if len(sys.argv) > 2:
        outName = sys.argv[2]
    else:
        print 'No out file name is given'
        outName = os.path.basename(infile)
        print 'Out file name is', outName

    outfile = TFile(outName, "RECREATE")

    if not tfile:
        print "Couldn't open the file"
        exit(0)

    pdict = loopFile(tfile)
    hist = plotList(pdict)
    hist.SetStats(0)

    canv=TCanvas(hist.GetName(),hist.GetTitle(),800,600)
    hist.Draw('e')
    hist.GetYaxis().SetTitle("F_{sel-to-anti} QCD")

    x = input("Enter a number to  exit: ")

    hist.Write()

    tfile.Close()
    outfile.Close()
