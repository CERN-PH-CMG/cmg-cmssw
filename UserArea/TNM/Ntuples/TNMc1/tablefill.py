#Print the AN table of the bin content of the data dijet mass histogam for all categories (no tagging), category 0 (double tag) & category 3 (single tag) 

import os, sys
from array import array  # to use the "array" function when rebinning histograms
from ROOT import * 
from os import path

select="deta < 1.3 & categories<1 & categories >-1"
x=0.3
r=18.67393705

nMassBins = 39
massBoundaries = [890., 944., 1000., 1058., 1118., 1181., 1246., 1313., 1383., 1455., 1530., 1607., 1687., 1770., 1856., 1945., 2037., 2132., 2231., 2332., 2438., 2546., 2659., 2775., 2895., 3019., 3147., 3279., 3416., 3558., 3704., 3854., 4010., 4171., 4337., 4509., 4686., 4869., 5058., 5253.]




fdata=TFile.Open("/afs/cern.ch/work/s/sthebaud/private/rootfiles/dijetWtag_Moriond_Run2012.root")
treedata=fdata.Get("dijetWtag")

histalldata=TH1F("alldata","; M_{jj}/GeV",nMassBins,array('d',massBoundaries))
treedata.Project("alldata","DijetMassCA8","deta < 1.3 & categories<7 & categories >-2")

hist0data=TH1F("0data","; M_{jj}/GeV",nMassBins,array('d',massBoundaries))
treedata.Project("0data","DijetMassCA8","deta < 1.3 & categories<1 & categories >-1")

hist2data=TH1F("2data","; M_{jj}/GeV",nMassBins,array('d',massBoundaries))
treedata.Project("2data","DijetMassCA8","deta < 1.3 & categories<4 & categories >2")

for i in range(39):
    print("{0} & {1} & {2} & {3} \\\\".format(massBoundaries[i],histalldata.GetBinContent(i+1),hist0data.GetBinContent(i+1),hist2data.GetBinContent(i+1)))




