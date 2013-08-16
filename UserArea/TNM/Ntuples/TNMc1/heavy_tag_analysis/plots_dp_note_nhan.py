import os, sys
import array
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
gStyle.SetLabelSize(0.05, "XYZ")
gStyle.SetNdivisions(510, "XYZ")
gStyle.SetLegendBorderSize(0)

TGaxis.SetMaxDigits(3)

if __name__ == '__main__':

 runSet=1

 if runSet<10:
   samples = ["dijetWtag_Moriond_Mar6.root",
	    "dijetWtag_Moriond_QCD1000.root",
	    "dijetWtag_Moriond_WWPy61500.root",
	    "dijetWtag_Moriond_Mar6.root",
	    ]
   set=""

 if runSet==1:
  plots = [("Jet1pt","((abs(Jet1eta)<2.5)&&(Jet1pt>700))","jet p_{T}"),
           ("Jet1CA8Mass","((abs(Jet1eta)<2.5)&&(Jet1pt>700))","pruned jet mass"),
           ("Jet1CA8MassDrop","((abs(Jet1eta)<2.5)&&(Jet1pt>700))","mass drop"),
           ("Jet1CA8Nsub","((abs(Jet1eta)<2.5)&&(Jet1pt>700))","#tau_{2}/#tau_{1}", ),
           ("Jet1CA8MassDrop","((abs(Jet1eta)<2.5)&&(Jet1pt>700)&&(Jet1CA8Mass>70)&&(Jet1CA8Mass<100))","mass drop"),
           ("Jet1CA8Nsub","((abs(Jet1eta)<2.5)&&(Jet1pt>700)&&(Jet1CA8Mass>70)&&(Jet1CA8Mass<100))","#tau_{2}/#tau_{1}", ),
           ]

  names = ["dp_note_pt",
	   "dp_note_mass",
	   "dp_note_massdrop",
	   "dp_note_tau21",
	   "dp_note_massdrop_aftermass",
	   "dp_note_tau21_aftermass",
	   ]

 hists=[]
 
 colors=[1,2,4,7,6,8,9,11,40,41,42,43,44,45,46,47]
 styles=[1,2,3,4,1,2,3,4,1,2,3,4,1,2,3,4,1,2,3,4]

 results=[]
 for mcOnly in ["","mc"]:
  if mcOnly:
      samples=samples[1:-1]
  for plot in plots:
   integral=None
   canvas = TCanvas("","",0,0,200,200)
   if mcOnly:
      #legend=TLegend(0.52,0.7,0.9,0.9,"CMS Simulation")
      legend=TLegend(0.40,0.7,0.88,0.9)      
   else:
      #legend=TLegend(0.52,0.7,0.9,0.9,"CMS Preliminary")
      legend=TLegend(0.40,0.7,0.88,0.9)
   dataPlotted=False
   for sample in samples:
    signal = "Hpp" in sample or "Py6" in sample
    f=TFile.Open(sample)
    tree=f.Get("dijetWtag")
    if plot[2]=="jet p_{T}":
       hist=TH1F("plot"+names[plots.index(plot)],"plot"+names[plots.index(plot)],50,700,1500);
       hist.GetYaxis().SetRangeUser(0,50000)
    if plot[2]=="pruned jet mass":
       hist=TH1F("plot"+names[plots.index(plot)],"plot"+names[plots.index(plot)],40,0,200);
       hist.GetYaxis().SetRangeUser(0,70000)
    if plot[2]=="mass drop":
       hist=TH1F("plot"+names[plots.index(plot)],"plot"+names[plots.index(plot)],25,0,1);
       hist.GetYaxis().SetRangeUser(0,60000)
       if "aftermass" in names[plots.index(plot)]:
           hist.GetYaxis().SetRangeUser(0,6000)
    if plot[2]=="#tau_{2}/#tau_{1}":
       hist=TH1F("plot"+names[plots.index(plot)],"plot"+names[plots.index(plot)],25,0,1);
       hist.GetYaxis().SetRangeUser(0,55000)
       if "aftermass" in names[plots.index(plot)]:
           hist.GetYaxis().SetRangeUser(0,5000)
    tree.Project("plot"+names[plots.index(plot)],plot[0],plot[1])
    hist.SetTitle("")
    hist.SetLineColor(colors[samples.index(sample)])
    hist.SetLineStyle(1)
    #hist.SetLineWidth(2)
    hist.SetFillStyle(0)
    hist.SetMarkerStyle(20)
    #hist.SetMarkerSize(2)
    hist.GetXaxis().SetTitle(plot[2])
    hist.GetYaxis().SetTitle("N")
    if integral==None:
        integral=hist.Integral()
    else:
        hist.Scale(integral/hist.Integral())
    if mcOnly:
        hist.Scale(1.0/hist.Integral())
        hist.GetYaxis().SetRangeUser(0,0.4)
        if plot[2]=="jet p_{T}":
            hist.GetYaxis().SetRangeUser(0,0.2)
        if plot[2]=="pruned jet mass":
            hist.GetYaxis().SetRangeUser(0,0.3)
        if plot[2]=="mass drop" and "aftermass" in names[plots.index(plot)]:
            hist.GetYaxis().SetRangeUser(0,0.60)
    if "Mar6" in sample:
        #        legend.AddEntry(hist,"data #sqrt{s} = 8 TeV 19.6/fb","ple")
        if not dataPlotted:
             legend.AddEntry(hist,"data","ple")   
             hist.Draw("pe")
	     dataPlotted=True
	else:
             hist.Draw("pesame")
    if "QCD" in sample:
        legend.AddEntry(hist,"QCD Madgraph+Pythia6","f")
        hist.SetFillStyle(3002);
        hist.SetLineColor(1);
        hist.SetFillColor(colors[samples.index(sample)])        
	if mcOnly:
            hist.Draw("hist")
        else:
            hist.Draw("histsame")
    if "WW" in sample:
#        legend.AddEntry(hist,"G_{RS} #rightarrow WW, 1.5 TeV, Pythia6","l")
        legend.AddEntry(hist,"Signal W-jet,","l")
        legend.AddEntry(hist,"G_{RS}(1.5 TeV) #rightarrow WW , Pythia6","")        
        hist.SetLineStyle(2);
        hist.SetLineWidth(2);        
        hist.Draw("histsame")
    hists+=[hist]

   legend.SetTextSize(0.04)
   legend.SetFillStyle(0)
   legend.Draw("same")

#   legend2=TLegend(0.53,0.62,0.9,0.75,"jet p_{T}>700 GeV, |#eta|<2.5")
#   legend2.SetTextSize(0.03)
#   legend2.SetFillStyle(0)
#   legend2.Draw("same")

   banner = TLatex(0.3,0.93,("CMS Preliminary, 19 fb^{-1} at #sqrt{s} = 8TeV, dijets"));
   banner.SetNDC()
   banner.SetTextSize(0.04)
   banner.Draw();  

#   if "aftermass" in names[plots.index(plot)]:
#      legend3=TLegend(0.53,0.54,0.9,0.75,"70 < pruned jet mass <100 GeV")
#      legend3.SetTextSize(0.03)
#      legend3.SetFillStyle(0)
#      legend3.Draw("same")

   canvas.SaveAs("data2012_v4_tree_"+names[plots.index(plot)]+mcOnly+".png")
   canvas.SaveAs("data2012_v4_tree_"+names[plots.index(plot)]+mcOnly+".pdf")
   canvas.SaveAs("data2012_v4_tree_"+names[plots.index(plot)]+mcOnly+".root")
   canvas.SaveAs("data2012_v4_tree_"+names[plots.index(plot)]+mcOnly+".C")
   canvas.SaveAs("data2012_v4_tree_"+names[plots.index(plot)]+mcOnly+".eps")
