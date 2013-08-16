# Creates and saves 7 ROOT files containing the dijet mass distribution histograms of the data, QCD MC and signals sample files. The events in the histograms satisfy the "select" condition, and the custom binning is applied.


import os, sys
from array import array  # to use the "array" function when rebinning histograms
from ROOT import * 
from os import path

select="deta < 1.3 & categories<1 & categories >-1"
r=18.67393705
path="/afs/cern.ch/work/s/sthebaud/private/rootfiles/"


nMassBins = 103
massBoundaries = [1., 3., 6., 10., 16., 23., 31., 40., 50., 61., 74., 88., 103., 119., 137., 156., 176., 197., 220., 244., 270., 296., 325.,
  354., 386., 419., 453., 489., 526., 565., 606., 649., 693., 740., 788., 838., 890., 944., 1000., 1058., 1118., 1181., 1246., 1313., 1383., 1455., 1530., 1607., 1687., 1770., 1856., 1945., 2037., 2132., 2231., 2332., 2438., 2546., 2659., 2775., 2895., 3019., 3147., 3279., 3416., 3558., 3704., 3854., 4010., 4171., 4337., 4509., 4686., 4869., 5058., 5253., 5455., 5663., 5877., 6099., 6328., 6564., 6808., 7060., 7320., 7589., 7866., 8152., 8447., 8752., 9067., 9391., 9726., 10072., 10430., 10798., 11179., 11571., 11977., 12395., 12827., 13272., 13732., 14000.]



fsignal1000=TFile.Open(path+"dijetWtag_Moriond_HHPy61000.root")
treesignal1000=fsignal1000.Get("dijetWtag")
histsignal1000=TH1F("dijetWtag_Moriond_HHPy61000.root","; M_{jj}/GeV",nMassBins,array('d',massBoundaries))
treesignal1000.Project("dijetWtag_Moriond_HHPy61000.root","DijetMassCA8",select)
histsignal1000.SaveAs("histograms/histo_dijetmass_rebin_signal1000.root")


fsignal1500=TFile.Open(path+"dijetWtag_Moriond_HHPy61500.root")
treesignal1500=fsignal1500.Get("dijetWtag")
histsignal1500=TH1F("dijetWtag_Moriond_HHPy61500.root","; M_{jj}/GeV",nMassBins,array('d',massBoundaries))
treesignal1500.Project("dijetWtag_Moriond_HHPy61500.root","DijetMassCA8",select)
histsignal1500.SaveAs("histograms/histo_dijetmass_rebin_signal1500.root")


fsignal2000=TFile.Open(path+"dijetWtag_Moriond_HHPy62000.root")
treesignal2000=fsignal2000.Get("dijetWtag")
histsignal2000=TH1F("dijetWtag_Moriond_HHPy62000.root","; M_{jj}/GeV",nMassBins,array('d',massBoundaries))
treesignal2000.Project("dijetWtag_Moriond_HHPy62000.root","DijetMassCA8",select)
histsignal2000.SaveAs("histograms/histo_dijetmass_rebin_signal2000.root")


fsignal2500=TFile.Open(path+"dijetWtag_Moriond_HHPy62500.root")
treesignal2500=fsignal2500.Get("dijetWtag")
histsignal2500=TH1F("dijetWtag_Moriond_HHPy62500.root","; M_{jj}/GeV",nMassBins,array('d',massBoundaries))
treesignal2500.Project("dijetWtag_Moriond_HHPy62500.root","DijetMassCA8",select)
histsignal2500.SaveAs("histograms/histo_dijetmass_rebin_signal2500.root")


fsignal3000=TFile.Open(path+"dijetWtag_Moriond_HHPy63000.root")
treesignal3000=fsignal3000.Get("dijetWtag")
histsignal3000=TH1F("dijetWtag_Moriond_HHPy63000.root","; M_{jj}/GeV",nMassBins,array('d',massBoundaries))
treesignal3000.Project("dijetWtag_Moriond_HHPy63000.root","DijetMassCA8",select)
histsignal3000.SaveAs("histograms/histo_dijetmass_rebin_signal3000.root")


fdata=TFile.Open(path+"dijetWtag_Moriond_Run2012.root")
treedata=fdata.Get("dijetWtag")
histdata=TH1F("dijetWtag_Moriond_Run2012.root","; M_{jj}/GeV",nMassBins,array('d',massBoundaries))
treedata.Project("dijetWtag_Moriond_Run2012.root","DijetMassCA8",select)
histdata.SaveAs("histograms/histo_dijetmass_rebin_data.root")


fqcd500=TFile.Open(path+"dijetWtag_Moriond_QCD500.root")
treeqcd500=fqcd500.Get("dijetWtag")
histqcd500=TH1F("dijetWtag_Moriond_QCD500.root","; M_{jj}/GeV",nMassBins,array('d',massBoundaries))
treeqcd500.Project("dijetWtag_Moriond_QCD500.root","DijetMassCA8",select)
histqcd500.Scale(r)
histqcd500.SaveAs("histograms/histo_dijetmass_rebin_qcd500.root")

fqcd1000=TFile.Open(path+"dijetWtag_Moriond_QCD1000.root")
treeqcd1000=fqcd1000.Get("dijetWtag")
histqcd1000=TH1F("dijetWtag_Moriond_QCD1000.root","; M_{jj}/GeV",nMassBins,array('d',massBoundaries))
treeqcd1000.Project("dijetWtag_Moriond_QCD1000.root","DijetMassCA8",select)
histqcd1000.SaveAs("histograms/histo_dijetmass_rebin_qcd1000.root")

histqcd=histqcd500
histqcd.Add(histqcd500,histqcd1000,1,1)

histqcd.SaveAs("histograms/histo_dijetmass_rebin_qcd.root")

