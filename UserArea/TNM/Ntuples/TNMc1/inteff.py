#Creates the graph of the signal tagging efficiency for category 0 (double-tag) & category 3 (single-tag).

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



nb=100


f1000=TFile.Open("/afs/cern.ch/work/s/sthebaud/private/rootfiles/dijetWtag_Moriond_HHPy61000.root")
tree1000=f1000.Get("dijetWtag")

hist1000all=TH1F("1000all","; M_{jj}/GeV",nb,0,14000)
hist1000single=TH1F("1000single","; M_{jj}/GeV",nb,0,14000)
hist1000double=TH1F("1000double","; M_{jj}/GeV",nb,0,14000)
tree1000.Project("1000all","DijetMassCA8","deta < 1.3 & categories<7 & categories>-2")
tree1000.Project("1000single","DijetMassCA8","deta < 1.3 & categories<4 & categories>2")
tree1000.Project("1000double","DijetMassCA8","deta < 1.3 & categories<1 & categories>-1")

n1000all=hist1000all.Integral()
n1000single=hist1000single.Integral()
n1000double=hist1000double.Integral()
eff1000single=n1000single/n1000all
eff1000double=n1000double/n1000all



f1500=TFile.Open("/afs/cern.ch/work/s/sthebaud/private/rootfiles/dijetWtag_Moriond_HHPy61500.root")
tree1500=f1500.Get("dijetWtag")

hist1500all=TH1F("1500all","; M_{jj}/GeV",nb,0,14000)
hist1500single=TH1F("1500single","; M_{jj}/GeV",nb,0,14000)
hist1500double=TH1F("1500double","; M_{jj}/GeV",nb,0,14000)
tree1500.Project("1500all","DijetMassCA8","deta < 1.3 & categories<7 & categories>-2")
tree1500.Project("1500single","DijetMassCA8","deta < 1.3 & categories<4 & categories>2")
tree1500.Project("1500double","DijetMassCA8","deta < 1.3 & categories<1 & categories>-1")

n1500all=hist1500all.Integral()
n1500single=hist1500single.Integral()
n1500double=hist1500double.Integral()
eff1500single=n1500single/n1500all
eff1500double=n1500double/n1500all



f2000=TFile.Open("/afs/cern.ch/work/s/sthebaud/private/rootfiles/dijetWtag_Moriond_HHPy62000.root")
tree2000=f2000.Get("dijetWtag")

hist2000all=TH1F("2000all","; M_{jj}/GeV",nb,0,14000)
hist2000single=TH1F("2000single","; M_{jj}/GeV",nb,0,14000)
hist2000double=TH1F("2000double","; M_{jj}/GeV",nb,0,14000)
tree2000.Project("2000all","DijetMassCA8","deta < 1.3 & categories<7 & categories>-2")
tree2000.Project("2000single","DijetMassCA8","deta < 1.3 & categories<4 & categories>2")
tree2000.Project("2000double","DijetMassCA8","deta < 1.3 & categories<1 & categories>-1")

n2000all=hist2000all.Integral()
n2000single=hist2000single.Integral()
n2000double=hist2000double.Integral()
eff2000single=n2000single/n2000all
eff2000double=n2000double/n2000all



f2500=TFile.Open("/afs/cern.ch/work/s/sthebaud/private/rootfiles/dijetWtag_Moriond_HHPy62500.root")
tree2500=f2500.Get("dijetWtag")

hist2500all=TH1F("2500all","; M_{jj}/GeV",nb,0,14000)
hist2500single=TH1F("2500single","; M_{jj}/GeV",nb,0,14000)
hist2500double=TH1F("2500double","; M_{jj}/GeV",nb,0,14000)
tree2500.Project("2500all","DijetMassCA8","deta < 1.3 & categories<7 & categories>-2")
tree2500.Project("2500single","DijetMassCA8","deta < 1.3 & categories<4 & categories>2")
tree2500.Project("2500double","DijetMassCA8","deta < 1.3 & categories<1 & categories>-1")

n2500all=hist2500all.Integral()
n2500single=hist2500single.Integral()
n2500double=hist2500double.Integral()
eff2500single=n2500single/n2500all
eff2500double=n2500double/n2500all



f3000=TFile.Open("/afs/cern.ch/work/s/sthebaud/private/rootfiles/dijetWtag_Moriond_HHPy63000.root")
tree3000=f3000.Get("dijetWtag")

hist3000all=TH1F("3000all","; M_{jj}/GeV",nb,0,14000)
hist3000single=TH1F("3000single","; M_{jj}/GeV",nb,0,14000)
hist3000double=TH1F("3000double","; M_{jj}/GeV",nb,0,14000)
tree3000.Project("3000all","DijetMassCA8","deta < 1.3 & categories<7 & categories>-2")
tree3000.Project("3000single","DijetMassCA8","deta < 1.3 & categories<4 & categories>2")
tree3000.Project("3000double","DijetMassCA8","deta < 1.3 & categories<1 & categories>-1")

n3000all=hist3000all.Integral()
n3000single=hist3000single.Integral()
n3000double=hist3000double.Integral()
eff3000single=n3000single/n3000all
eff3000double=n3000double/n3000all


xtemp=[1000,1500,2000,2500,3000]
effsingletemp=[eff1000single,eff1500single,eff2000single,eff2500single,eff3000single]
effdoubletemp=[eff1000double,eff1500double,eff2000double,eff2500double,eff3000double]

x=TVectorD(5)
for i in range(5):
    x[i]=xtemp[i]

effsingle=TVectorD(5)
for i in range(5):
    effsingle[i]=effsingletemp[i]

effdouble=TVectorD(5)
for i in range(5):
    effdouble[i]=effdoubletemp[i]



canvainteff = TCanvas("","",0,0,300,300)
graphsingle=TGraph(x,effsingle)
graphdouble=TGraph(x,effdouble)

graphsingle.SetTitle("; M(R)/GeV")
graphsingle.SetLineWidth(2)
graphsingle.SetLineStyle(1)
graphsingle.SetLineColor(50)

graphdouble.SetLineWidth(2)
graphdouble.SetLineStyle(1)
graphdouble.SetLineColor(8)

graphsingle.Draw("alp")
yaxis=graphsingle.GetYaxis()
graphdouble.Draw("same")
yaxis.SetRangeUser(0,1)

legend=TLegend(0.35, 0.6, 0.9, 0.9, "CMS Preliminary (L=19.6 /fb)")
legend.SetTextSize(0.04)
legend.SetFillStyle(0)
legend.AddEntry(graphsingle,"Single tag")
legend.AddEntry(graphdouble,"Double tag")
legend.Draw("same")

canvainteff.SaveAs("histograms/histo_inteff"+".pdf")
