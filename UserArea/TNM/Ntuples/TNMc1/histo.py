import os, sys
import array
from ROOT import * 
from os import path

"""gROOT.Reset()
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
gStyle.SetLabelSize(0.05, "XYZ")
gStyle.SetNdivisions(510, "XYZ")
gStyle.SetLegendBorderSize(0)"""



samples = [("dijetWtag_Moriond_QCD1000.root","QCD background"),     # mettre bruit QCD first!
           ("dijetWtag_Moriond_Run2012Ar.root","data"),
           ("dijetWtag_Moriond_HHPy61500.root","M(Rad)=1.5 TeV")] 
set=""

plots = [("DijetMassCA8", "DijetMassCA8>890", "Masse des jets en Gev"),
           ("deta","deta<5","#Delta#eta")]

names = ["histo_masse_dijet",
         "histo_delta_eta"]



nb=50                # nb: nombre de bins
logs=[0,0]            # pour chaque plot: 1 pour creer un histo
                     # en plus avec une echelle log, 0 sinon.


for plot in plots:
    hists=[]
    s=THStack("s",plots[plots.index(plot)][2])
    canvas = TCanvas("","",0,0,200,200)
    for sample in samples:
        f=TFile.Open(sample[0])
        tree=f.Get("dijetWtag")
        if plot[2]=="Masse des jets en Gev":
            hist=TH1F(sample[0],sample[1],nb,890,2000);
        if plot[2]=="#Delta#eta":
            hist=TH1F(sample[0],sample[1],nb,0,4);

        tree.Project(sample[0],plot[0],plot[1])
        hists += [hist]

    integral=hists[1].Integral()             # les donnees exp sont la ref de normalisation
    for hist in hists:                       # ainsi, les barres d'erreurs n'ont pas
        hist.Scale(integral/hist.Integral()) # a etre normalisees
        if hists.index(hist)==0:
            hist.SetLineWidth(0)             # Le bruit QCD est trace en premier pour
            hist.SetFillColor(kOrange)       # que le remplissage ne cache pas les 
            hist.SetLineColor(kOrange)       # autres courbes
            s.Add(hist)
        if hists.index(hist)==1:
            hist.SetOption("p,E")
            hist.SetLineWidth(2)
            hist.SetLineStyle(1)
            hist.SetMarkerStyle(20)
            hist.SetMarkerSize(0.15)
            hist.SetLineColor(1)
            s.Add(hist,"p,e")
        if hists.index(hist)==2:
            hist.SetLineWidth(2)
            hist.SetLineStyle(2)
            hist.SetLineColor(kRed)
            s.Add(hist)

    s.Draw("nostack")
    """legend=TLegend(0.52,0.7,0.9,0.9,"CMS Simu")
    legend.SetTextSize(0.03)
    legend.SetFillStyle(0)
    legend.Draw("same")"""
    canvas.BuildLegend(0.55,0.75,0.9,0.9)
    canvas.SaveAs("histogrammes/"+names[plots.index(plot)]+".pdf")
    canvas.SaveAs("histogrammes/"+names[plots.index(plot)]+".root")
    canvas.SaveAs("histogrammes/"+names[plots.index(plot)]+".C")
    if logs[plots.index(plot)]>0:
        canvas.SetLogy(1)
        canvas.SaveAs("histogrammes/log"+names[plots.index(plot)]+".pdf")
        canvas.SaveAs("histogrammes/log"+names[plots.index(plot)]+".root")
        canvas.SaveAs("histogrammes/log"+names[plots.index(plot)]+".C")
