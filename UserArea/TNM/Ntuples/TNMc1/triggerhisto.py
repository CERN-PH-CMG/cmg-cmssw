#Creates and saves as PDF the trigger efficiency graph for all categories (no tagging) and category 0 (double tag).
#The efficiency is : (FAT750 OR HT750 OR PFHT650 OR PF(NOPU)HT650) over HT550 

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


nb=50
massmin=0
massmax=1500
ymin=0
ymax=1
sety=1

f=TFile.Open("/afs/cern.ch/work/s/sthebaud/private/rootfiles/temp/dijetWtag_Moriond_Run2012D.root")
tree=f.Get("triggers")

histallmonitor=TH1F("allmonitor","; M_{jj}/GeV",nb,massmin,massmax)
tree.Project("allmonitor","DijetMassCA8","categories<7 & categories >-2 & ht550==1")
histallmonitor.Sumw2()

hist0monitor=TH1F("0monitor","; M_{jj}/GeV",nb,massmin,massmax)
tree.Project("0monitor","DijetMassCA8","categories<1 & categories >-1 & ht550==1")
hist0monitor.Sumw2()

histalltrig=TH1F("alltrig","; M_{jj}/GeV",nb,massmin,massmax)
tree.Project("alltrig","DijetMassCA8","categories<7 & categories >-2 & ht550==1 & trig==1")
histalltrig.Sumw2()

hist0trig=TH1F("0trig","; M_{jj}/GeV",nb,massmin,massmax)
tree.Project("0trig","DijetMassCA8","categories<1 & categories >-1 & ht550==1 & trig==1")
hist0trig.Sumw2()


lv=TLine(890, 0, 890, 1)
lv.SetLineColor(2)
lh=TLine(massmin, 0.99, massmax, 0.99)
lh.SetLineColor(9)


histalltrigeff=TH1F("alltrigeff","; M_{jj}/GeV",nb,massmin,massmax)
histalltrigeff.Divide(histalltrig,histallmonitor,1,1)
canvasalltrigeff=TCanvas("","",0,0,300,300)
if sety==1:
    histalltrigeff.SetMaximum(ymax)
    histalltrigeff.SetMinimum(ymin)
histalltrigeff.Draw("p,e")
lv.Draw()
lh.Draw()
canvasalltrigeff.SaveAs("histograms/histo_alltrig.pdf")


hist0trigeff=TH1F("0trigeff","; M_{jj}/GeV",nb,massmin,massmax)
hist0trigeff.Divide(hist0trig,hist0monitor,1,1)
canvas0trigeff=TCanvas("","",0,0,300,300)
if sety==1:
    hist0trigeff.SetMaximum(ymax)
    hist0trigeff.SetMinimum(ymin)
hist0trigeff.Draw("p,e")
lv.Draw()
lh.Draw()
canvas0trigeff.SaveAs("histograms/histo_0trig.pdf")





