import os, sys
import array
from ROOT import * 

gROOT.Reset()
gROOT.SetStyle("Plain")
gStyle.SetOptStat(0)
gStyle.SetOptFit(0)
gStyle.SetTitleOffset(1.2,"Y")
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
gStyle.SetLegendBorderSize(0)

if __name__ == '__main__':

    samples=["Resonance_Shapes_RSGraviton_ak5_QQtoQQ_fat30.root",
	     #"Resonance_Shapes_RSGraviton_2012_D6T_ak5_QQtoQQ_fat30.root",
	     #"Resonance_Shapes_RSGraviton_2012_Z2star_ak5_QQtoQQ_fat30.root",
	     "Resonance_Shapes_RSGraviton_ak5_GGtoGG_fat30.root",
	     #"Resonance_Shapes_RSGraviton_2012_D6T_ak5_GGtoGG_fat30.root",
	     #"Resonance_Shapes_RSGraviton_2012_Z2star_ak5_GGtoGG_fat30.root",
	     "Resonance_Shapes_Qstar_ak5_fat30.root",
	     #"Resonance_Shapes_Qstar_2012_D6T_ak5_fat30.root",
	     #"Resonance_Shapes_Qstar_2012_Z2star_ak5_fat30.root",
             ]
    
    labels=["RSG D6T qq 7 TeV",
            #"RSG D6T qq 8 TeV",
            #"RSG Z2* qq 8 TeV",
            "RSG D6T gg 7 TeV",
            #"RSG D6T gg 8 TeV",
            #"RSG Z2* gg 8 TeV",
            "q* D6T qg 7 TeV",
            #"q* D6T qg 8 TeV",
            #"q* Z2* qg 8 TeV",
            ]
    
    masses=[1000,2000,3000,4000]

    colors=[1,2,3,4,6,7,8,9,11,12]
    styles=[1,2,3,4,5,6,7,8,9,10]
    
    index=0
    i=0
    canvas = TCanvas("","",0,0,200,200)
    legend=TLegend(0.5,0.6,0.95,0.90,"")
    for sample in samples:
      for mass in masses:
        f=TFile.Open("../Results/"+sample)
        hist=f.Get("h_qstar_"+str(mass))
	hist.GetYaxis().SetRangeUser(0,0.005)
        hist.SetLineColor(colors[index])
        hist.SetLineStyle(styles[index])
        hist.GetXaxis().SetTitle("M_{jj} [GeV]")
	if i==0:
            hist.Draw("hist")
        else:	
            hist.Draw("histsame")
        i+=1
      index+=1
      legend.AddEntry(hist,labels[samples.index(sample)],"l")

    legend.SetTextSize(0.04)
    legend.SetFillStyle(0)
    legend.Draw("same")
    canvas.SaveAs("masses_2011.root")
    canvas.SaveAs("masses_2011.pdf")
