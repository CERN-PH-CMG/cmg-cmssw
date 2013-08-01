import os, sys
from array import array  # to use the "array" function when rebinning histograms
from ROOT import * 
from os import path

gROOT.Reset()
gROOT.SetStyle("Plain")
gStyle.SetOptStat(0)
gStyle.SetOptFit(0)
gStyle.SetTitleOffset(1.5,"Y")
gStyle.SetPadLeftMargin(0.18)
gStyle.SetPadBottomMargin(0.15)
gStyle.SetPadTopMargin(0.08)
gStyle.SetPadRightMargin(0.05)
gStyle.SetMarkerSize(0.5)
gStyle.SetHistLineWidth(1)
gStyle.SetStatFontSize(0.020)
gStyle.SetTitleSize(0.06, "XYZ")
gStyle.SetLabelSize(0.03, "XYZ")
gStyle.SetNdivisions(510, "XYZ")
gStyle.SetLegendBorderSize(0)



nMassBins = 103
massBoundaries = [1., 3., 6., 10., 16., 23., 31., 40., 50., 61., 74., 88., 103., 119., 137., 156., 176., 197., 220., 244., 270., 296., 325.,
  354., 386., 419., 453., 489., 526., 565., 606., 649., 693., 740., 788., 838., 890., 944., 1000., 1058., 1118., 1181., 1246., 1313., 1383., 1455., 1530., 1607., 1687., 1770., 1856., 1945., 2037., 2132., 2231., 2332., 2438., 2546., 2659., 2775., 2895., 3019., 3147., 3279., 3416., 3558., 3704., 3854., 4010., 4171., 4337., 4509., 4686., 4869., 5058., 5253., 5455., 5663., 5877., 6099., 6328., 6564., 6808., 7060., 7320., 7589., 7866., 8152., 8447., 8752., 9067., 9391., 9726., 10072., 10430., 10798., 11179., 11571., 11977., 12395., 12827., 13272., 13732., 14000.]


nb=50
massmin=0
massmax=1500
ymin=0
ymax=1
sety=1
r=18.67393705

f1000=TFile.Open("/afs/cern.ch/work/s/sthebaud/private/rootfiles/temp/dijetWtag_Moriond_QCD1000.root")
tree1000=f1000.Get("jes")

histnormal1000=TH1F("normal1000","; M_{jj}/GeV",nMassBins,array('d',massBoundaries))
tree1000.Project("normal1000","DijetMassCA8pruned","deta<1.3 & Jet1CA8Mass>110 & Jet1CA8Mass<135 & Jet2CA8Mass>110 & Jet2CA8Mass<135 & Jet1CA8Nsub<0.5 & Jet2CA8Nsub<0.5")

histdown1000=TH1F("down1000","; M_{jj}/GeV",nMassBins,array('d',massBoundaries))
tree1000.Project("down1000","DijetMassCA8pruneddw","deta<1.3 & Jet1CA8Massdw>110 & Jet1CA8Massdw<135 & Jet2CA8Massdw>110 & Jet2CA8Massdw<135 & Jet1CA8Nsub<0.5 & Jet2CA8Nsub<0.5")

histup1000=TH1F("up1000","; M_{jj}/GeV",nMassBins,array('d',massBoundaries))
tree1000.Project("up1000","DijetMassCA8prunedup","deta<1.3 & Jet1CA8Massup>110 & Jet1CA8Massup<135 & Jet2CA8Massup>110 & Jet2CA8Massup<135 & Jet1CA8Nsub<0.5 & Jet2CA8Nsub<0.5")



f500=TFile.Open("/afs/cern.ch/work/s/sthebaud/private/rootfiles/temp/dijetWtag_Moriond_QCD500.root")
tree500=f500.Get("jes")

histnormal500=TH1F("normal500","; M_{jj}/GeV",nMassBins,array('d',massBoundaries))
tree500.Project("normal500","DijetMassCA8pruned","deta<1.3 & Jet1CA8Mass>110 & Jet1CA8Mass<135 & Jet2CA8Mass>110 & Jet2CA8Mass<135 & Jet1CA8Nsub<0.5 & Jet2CA8Nsub<0.5")

histdown500=TH1F("down500","; M_{jj}/GeV",nMassBins,array('d',massBoundaries))
tree500.Project("down500","DijetMassCA8pruneddw","deta<1.3 & Jet1CA8Massdw>110 & Jet1CA8Massdw<135 & Jet2CA8Massdw>110 & Jet2CA8Massdw<135 & Jet1CA8Nsub<0.5 & Jet2CA8Nsub<0.5")

histup500=TH1F("up500","; M_{jj}/GeV",nMassBins,array('d',massBoundaries))
tree500.Project("up500","DijetMassCA8prunedup","deta<1.3 & Jet1CA8Massup>110 & Jet1CA8Massup<135 & Jet2CA8Massup>110 & Jet2CA8Massup<135 & Jet1CA8Nsub<0.5 & Jet2CA8Nsub<0.5")


histnormal500.Scale(r)
histdown500.Scale(r)
histup500.Scale(r)

histnormal=TH1F("central","; M_{jj}/GeV",nMassBins,array('d',massBoundaries))
histdown=TH1F("down","; M_{jj}/GeV",nMassBins,array('d',massBoundaries))
histup=TH1F("up","; M_{jj}/GeV",nMassBins,array('d',massBoundaries))

histnormal.Add(histnormal500,histnormal1000,1,1)
histdown.Add(histdown500,histdown1000,1,1)
histup.Add(histup500,histup1000,1,1)

histnormal.SaveAs("histograms/histo_JEScentral.root")
histdown.SaveAs("histograms/histo_JESdown.root")
histup.SaveAs("histograms/histo_JESup.root")


canvasshift=TCanvas("","",0,0,300,300)

histnormal.SetLineWidth(2)
histnormal.SetLineStyle(1)
histnormal.SetLineColor(50)

histdown.SetLineWidth(2)
histdown.SetLineStyle(1)
histdown.SetLineColor(8)

histup.SetLineWidth(2)
histup.SetLineStyle(1)
histup.SetLineColor(9)


histnormal.Draw()
histdown.Draw("same")
histup.Draw("same")

legend=TLegend(0.475, 0.6, 0.9, 0.9, "CMS Preliminary (L=19.6 /fb)")
legend.SetTextSize(0.04)
legend.SetFillStyle(0)
legend.AddEntry(histnormal,"unshifted")
legend.AddEntry(histdown,"JES down")
legend.AddEntry(histup,"JES up")
legend.Draw("same")

canvasshift.SaveAs("histograms/histo_shiftjes.pdf")





