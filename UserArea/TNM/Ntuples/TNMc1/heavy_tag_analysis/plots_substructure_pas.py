import os, sys
import array
from ROOT import * 
from os import path

gROOT.Reset()
gROOT.SetStyle("Plain")
gStyle.SetOptStat(0)
gStyle.SetOptFit(0)
gStyle.SetTitleOffset(1.0,"Y")
gStyle.SetPadLeftMargin(0.13)
gStyle.SetPadBottomMargin(0.15)
gStyle.SetPadTopMargin(0.08)
gStyle.SetPadRightMargin(0.08)
gStyle.SetMarkerSize(0.5)
gStyle.SetHistLineWidth(1)
gStyle.SetStatFontSize(0.020)
gStyle.SetTitleSize(0.06, "XYZ")
gStyle.SetLabelSize(0.05, "XYZ")
gStyle.SetNdivisions(510, "XYZ")
gStyle.SetLegendBorderSize(0)

TGaxis.SetMaxDigits(3)

if __name__ == '__main__':

 runSet=2

 if runSet==1:
  samples = ["substructure_pas_QCD1000.root",
             "substructure_pas_WWPy61000.root",
            ]
  colors=[1,1,1,2,2,2]
  styles=[2,3,1,2,3,1]
  widths=[2,1,2,2,1,2]

  plots = [("nPU","((abs(Jet1eta)<2.5)&&(Jet1pt>500))","pileup interactions"),
           ("Jet1pt","((abs(Jet1eta)<2.5)&&(Jet1pt>500))","jet p_{T} (GeV)"),
           ("Jet1Mass","((abs(Jet1eta)<2.5)&&(Jet1pt>500))","pruned jet mass (GeV)"),
           ("Jet1MassDrop","((abs(Jet1eta)<2.5)&&(Jet1pt>500))","mass drop"),
           ("Jet1MassDrop","((abs(Jet1eta)<2.5)&&(Jet1pt>500)&&(Jet1Mass>70)&&(Jet1Mass<100))","mass drop"),
           ("Jet1Nsub","((abs(Jet1eta)<2.5)&&(Jet1pt>500))","#tau_{2}/#tau_{1}", ),
           ("Jet1Nsub","((abs(Jet1eta)<2.5)&&(Jet1pt>500)&&(Jet1Mass>70)&&(Jet1Mass<100))","#tau_{2}/#tau_{1}", ),
           ("Jet1C2beta15","((abs(Jet1eta)<2.5)&&(Jet1pt>500))","C_{2} (#beta=1.5)", ),
           ("Jet1C2beta17","((abs(Jet1eta)<2.5)&&(Jet1pt>500))","C_{2} (#beta=1.7)", ),
           ("Jet1C2beta20","((abs(Jet1eta)<2.5)&&(Jet1pt>500))","C_{2} (#beta=2.0)", ),
           ("Jet1jetCharge03","((abs(Jet1eta)<2.5)&&(Jet1pt>500))","jet charge (#kappa=0.3)", ),
           ("Jet1jetCharge05","((abs(Jet1eta)<2.5)&&(Jet1pt>500))","jet charge (#kappa=0.5)", ),
           ("Jet1jetCharge10","((abs(Jet1eta)<2.5)&&(Jet1pt>500))","jet charge (#kappa=1.0)", ),
           ("Jet1jetCharge03","((abs(Jet1eta)<2.5)&&(Jet1pt>500)&&(Jet1Mass>70)&&(Jet1Mass<100)&&(Jet1MassDrop<0.25))","jet charge (#kappa=0.3)", ),
           ("Jet1nConstituents","((abs(Jet1eta)<2.5)&&(Jet1pt>500))","jet constituents", ),
           ]

 if runSet==2:
  samples = ["substructure_pas_Run2012ABCD.root",
             "substructure_pas_QCD1000.root",
             "substructure_pas_Run2012ABCD.root",
            ]
  colors=[1,2,1]
  styles=[1,1,1]
  widths=[2,2,2]
  ndata=15000

  plots = [("nPU","((abs(Jet1eta)<2.5)&&(Jet1pt>1000))","pileup interactions"),
           ("Jet1pt","((abs(Jet1eta)<2.5)&&(Jet1pt>1000))","jet p_{T} (GeV)"),
           ("Jet1Mass","((abs(Jet1eta)<2.5)&&(Jet1pt>1000))","pruned jet mass (GeV)"),
           ("Jet1MassDrop","((abs(Jet1eta)<2.5)&&(Jet1pt>1000))","mass drop"),
           ("Jet1MassDrop","((abs(Jet1eta)<2.5)&&(Jet1pt>1000)&&(Jet1Mass>70)&&(Jet1Mass<100))","mass drop"),
           ("Jet1Nsub","((abs(Jet1eta)<2.5)&&(Jet1pt>1000))","#tau_{2}/#tau_{1}", ),
           ("Jet1Nsub","((abs(Jet1eta)<2.5)&&(Jet1pt>1000)&&(Jet1Mass>70)&&(Jet1Mass<100))","#tau_{2}/#tau_{1}", ),
           ("Jet1C2beta15","((abs(Jet1eta)<2.5)&&(Jet1pt>1000))","C_{2} (#beta=1.5)", ),
           ("Jet1C2beta17","((abs(Jet1eta)<2.5)&&(Jet1pt>1000))","C_{2} (#beta=1.7)", ),
           ("Jet1C2beta20","((abs(Jet1eta)<2.5)&&(Jet1pt>1000))","C_{2} (#beta=2.0)", ),
           ("Jet1jetCharge03","((abs(Jet1eta)<2.5)&&(Jet1pt>1000))","jet charge (#kappa=0.3)", ),
           ("Jet1jetCharge05","((abs(Jet1eta)<2.5)&&(Jet1pt>1000))","jet charge (#kappa=0.5)", ),
           ("Jet1jetCharge10","((abs(Jet1eta)<2.5)&&(Jet1pt>1000))","jet charge (#kappa=1.0)", ),
           ("Jet1jetCharge03","((abs(Jet1eta)<2.5)&&(Jet1pt>500)&&(Jet1Mass>70)&&(Jet1Mass<100)&&(Jet1MassDrop<0.25))","jet charge (#kappa=0.3)", ),
           ("Jet1nConstituents","((abs(Jet1eta)<2.5)&&(Jet1pt>1000))","jet constituents", ),
           ]

 if runSet==3:
  samples = ["substructure_pas_QCD1000.root",
             "substructure_pas_QCD1000.root",
             "substructure_pas_WWPy61000.root",
             "substructure_pas_WWPy63000.root",
             ]
  colors=[1,1,2,2]
  styles=[2,1,2,1]
  widths=[1,2,1,2]

  plots = [("nPU","((abs(Jet1eta)<2.5)&&(Jet1pt>500))","pileup interactions"),
           ("Jet1pt","((abs(Jet1eta)<2.5)&&(Jet1pt>500))","jet p_{T} (GeV)"),
           ("Jet1Mass","((abs(Jet1eta)<2.5)&&(Jet1pt>500))","pruned jet mass (GeV)"),
           ("Jet1MassDrop","((abs(Jet1eta)<2.5)&&(Jet1pt>500))","mass drop"),
           ("Jet1MassDrop","((abs(Jet1eta)<2.5)&&(Jet1pt>500)&&(Jet1Mass>70)&&(Jet1Mass<100))","mass drop"),
           ("Jet1Nsub","((abs(Jet1eta)<2.5)&&(Jet1pt>500))","#tau_{2}/#tau_{1}", ),
           ("Jet1Nsub","((abs(Jet1eta)<2.5)&&(Jet1pt>500)&&(Jet1Mass>70)&&(Jet1Mass<100))","#tau_{2}/#tau_{1}", ),
           ("Jet1C2beta15","((abs(Jet1eta)<2.5)&&(Jet1pt>500))","C_{2} (#beta=1.5)", ),
           ("Jet1C2beta17","((abs(Jet1eta)<2.5)&&(Jet1pt>500))","C_{2} (#beta=1.7)", ),
           ("Jet1C2beta20","((abs(Jet1eta)<2.5)&&(Jet1pt>500))","C_{2} (#beta=2.0)", ),
           ("Jet1jetCharge03","((abs(Jet1eta)<2.5)&&(Jet1pt>500))","jet charge (#kappa=0.3)", ),
           ("Jet1jetCharge05","((abs(Jet1eta)<2.5)&&(Jet1pt>500))","jet charge (#kappa=0.5)", ),
           ("Jet1jetCharge10","((abs(Jet1eta)<2.5)&&(Jet1pt>500))","jet charge (#kappa=1.0)", ),
           ("Jet1jetCharge03","((abs(Jet1eta)<2.5)&&(Jet1pt>500)&&(Jet1Mass>70)&&(Jet1Mass<100)&&(Jet1MassDrop<0.25))","jet charge (#kappa=0.3)", ),
           ("Jet1nConstituents","((abs(Jet1eta)<2.5)&&(Jet1pt>500))","jet constituents", ),
           ]

 names = ["npu",
	   "pt",
	   "mass",
	   "massdrop",
	   "massdrop_aftermass",
	   "tau21",
	   "tau21_aftermass",
	   "C2beta15",
	   "C2beta17",
	   "C2beta20",
	   "jetcharge03",
	   "jetcharge05",
	   "jetcharge10",
	   "jetcharge03_aftermass",
	   "nconstituents",
	   ]

 hists=[]
 
 results=[]
 for plot in plots:
  integral=None
  canvas = TCanvas("","",0,0,200,200)
  canvas.SetLogy(False)
  if runSet==2:
    legend=TLegend(0.45,0.7,0.85,0.9)
  else:
    legend=TLegend(0.45,0.6,0.85,0.9)
  dataPlotted=False
  counter=0
  integral=1
  for sample in samples:
   for gen in ["Gen","lowPU",""]:
    if names[plots.index(plot)]=="npu" and gen=="Gen":
       continue
    if runSet>1 and gen!="":
       continue
    signal = "Hpp" in sample or "Py6" in sample
    f=TFile.Open(sample)
    tree=f.Get("dijetWtag")
    if plot[2]=="pileup interactions":
       hist=TH1F("plot"+names[plots.index(plot)]+gen,"plot"+names[plots.index(plot)]+gen,25,0,50);
       hist.GetYaxis().SetRangeUser(0,50000)
    if "C_{2}" in plot[2]:
       hist=TH1F("plot"+names[plots.index(plot)]+gen,"plot"+names[plots.index(plot)]+gen,25,0,0.8);
       hist.GetYaxis().SetRangeUser(0,50000)
    if "jet charge (#kappa=0.3)" in plot[2]:
       hist=TH1F("plot"+names[plots.index(plot)]+gen,"plot"+names[plots.index(plot)]+gen,25,-0.05,0.05);
       hist.GetYaxis().SetRangeUser(0,50000)
    if "jet charge (#kappa=0.5)" in plot[2]:
       hist=TH1F("plot"+names[plots.index(plot)]+gen,"plot"+names[plots.index(plot)]+gen,25,-0.08,0.08);
       hist.GetYaxis().SetRangeUser(0,50000)
    if "jet charge (#kappa=1.0)" in plot[2]:
       hist=TH1F("plot"+names[plots.index(plot)]+gen,"plot"+names[plots.index(plot)]+gen,25,-1,1);
       hist.GetYaxis().SetRangeUser(0,50000)
    if plot[2]=="jet constituents":
       hist=TH1F("plot"+names[plots.index(plot)]+gen,"plot"+names[plots.index(plot)]+gen,25,0,200);
       hist.GetYaxis().SetRangeUser(0,50000)
    if "jet p_{T}" in plot[2]:
       if runSet==3:
          hist=TH1F("plot"+names[plots.index(plot)]+gen,"plot"+names[plots.index(plot)]+gen,40,500,2000);
       elif runSet==2:
          hist=TH1F("plot"+names[plots.index(plot)]+gen,"plot"+names[plots.index(plot)]+gen,40,1000,2000);
       else:
          hist=TH1F("plot"+names[plots.index(plot)]+gen,"plot"+names[plots.index(plot)]+gen,40,500,1500);
       hist.GetYaxis().SetRangeUser(0.001,50000)
       canvas.SetLogy(True)
    if "pruned jet mass" in plot[2]:
       hist=TH1F("plot"+names[plots.index(plot)]+gen,"plot"+names[plots.index(plot)]+gen,40,0,200);
       hist.GetYaxis().SetRangeUser(0,50000)
    if plot[2]=="mass drop":
       hist=TH1F("plot"+names[plots.index(plot)]+gen,"plot"+names[plots.index(plot)]+gen,25,0,1);
       hist.GetYaxis().SetRangeUser(0,60000)
       if "aftermass" in names[plots.index(plot)]:
           hist.GetYaxis().SetRangeUser(0,6000)
    if plot[2]=="#tau_{2}/#tau_{1}":
       hist=TH1F("plot"+names[plots.index(plot)]+gen,"plot"+names[plots.index(plot)]+gen,25,0,1);
       hist.GetYaxis().SetRangeUser(0,55000)
       if "aftermass" in names[plots.index(plot)]:
           hist.GetYaxis().SetRangeUser(0,5000)
    if gen=="lowPU":
        print plot[0],plot[1]+"&&(nPU<17)"
        tree.Project("plot"+names[plots.index(plot)]+gen,plot[0],plot[1]+"&&(nPU<17)")
    elif runSet==3 and (counter==1 or counter==3):
        print plot[0],plot[1]+"&&(Jet1pt>1500)"
        tree.Project("plot"+names[plots.index(plot)]+gen,plot[0],plot[1]+"&&(Jet1pt>1500)")
    else:
        print gen+plot[0],plot[1]
        tree.Project("plot"+names[plots.index(plot)]+gen,gen+plot[0],plot[1])
    hist.SetTitle("")
    hist.SetFillStyle(0)
    hist.SetMarkerStyle(20)
    #hist.SetMarkerSize(2)
    hist.GetXaxis().SetTitle(plot[2])
    hist.GetYaxis().SetTitle("N")
    if "Run" in sample:
        integral=hist.Integral()
    if hist.Integral()>0:
        hist.Scale(integral/hist.Integral())

    hist.SetLineColor(colors[counter]);
    hist.SetLineStyle(styles[counter])        
    hist.SetLineWidth(widths[counter])
    print "mean",hist.GetMean()

    if counter==0:
      if "Run" in sample:
        hist.Draw("pe")
      elif gen=="Gen":
        hist.Draw("lhist")
      else:
        hist.Draw("hist")
    else:
      if "Run" in sample:
        hist.Draw("pesame")
      elif gen=="Gen":
        hist.Draw("lsame")
      else:
        hist.Draw("histsame")

    if runSet==2:
      hist.GetYaxis().SetRangeUser(0,0.3*ndata)
      if "jet p_{T}" in plot[2]:
    	hist.GetYaxis().SetRangeUser(0.001*ndata,1*ndata)
      if "pruned jet mass" in plot[2]:
    	hist.GetYaxis().SetRangeUser(0,0.4*ndata)
      if "C_{2}" in plot[2]:
    	hist.GetYaxis().SetRangeUser(0,0.4*ndata)
      if "jet charge" in plot[2]:
    	hist.GetYaxis().SetRangeUser(0,0.7*ndata)
      if plot[2]=="mass drop" and "aftermass" in names[plots.index(plot)]:
    	hist.GetYaxis().SetRangeUser(0,0.05*ndata)
      if plot[2]=="#tau_{2}/#tau_{1}" and "aftermass" in names[plots.index(plot)]:
    	hist.GetYaxis().SetRangeUser(0,0.03*ndata)
    else:
      hist.GetYaxis().SetRangeUser(0,0.3)
      if "jet p_{T}" in plot[2]:
    	hist.GetYaxis().SetRangeUser(0.001,1)
      if "pruned jet mass" in plot[2]:
    	hist.GetYaxis().SetRangeUser(0,0.4)
      if "jet charge" in plot[2]:
    	hist.GetYaxis().SetRangeUser(0,0.5)
      if plot[2]=="mass drop" and "aftermass" in names[plots.index(plot)]:
    	hist.GetYaxis().SetRangeUser(0,0.4)

    if "Run" in sample and counter==0:
      legend.AddEntry(hist,"data","ple")
    if "QCD" in sample:
      if gen=="Gen" or runSet==2:
        legend.AddEntry(hist,"QCD Madgraph+Pythia6","l")
      elif runSet==3 and counter==0:
        legend.AddEntry(hist,"QCD p_{T} > 500 GeV","l")
      elif runSet==3 and counter==1:
        legend.AddEntry(hist,"QCD p_{T} > 1.5 TeV","l")
      elif gen=="lowPU":
        legend.AddEntry(hist," with 12PU + simulation","l")
      else:
        legend.AddEntry(hist," with 22PU + simulation","l")
    if "WW" in sample:
      if gen=="Gen" or runSet==2:
        legend.AddEntry(hist,"G_{RS} #rightarrow WW Pythia6","l")
      elif runSet==3 and "1000" in sample:
        legend.AddEntry(hist,"W p_{T} > 500 GeV","l")
      elif runSet==3 and "3000" in sample:
        legend.AddEntry(hist,"W p_{T} > 1.5 TeV","l")
      elif gen=="lowPU":
        legend.AddEntry(hist," with 12PU + simulation","l")
      else:
        legend.AddEntry(hist," with 22PU + simulation","l")
    hists+=[hist]
    counter+=1

  legend.SetTextSize(0.036)
  legend.SetFillStyle(0)
  legend.Draw("same")

  legend4=TLegend(0.23,0.85,0.5,0.9,"CA R=0.8")
  legend4.SetTextSize(0.03)
  legend4.SetFillStyle(0)
  legend4.Draw("same")

  if runSet==3:
    legend2=TLegend(0.23,0.8,0.5,0.85,"|#eta|<2.5")
  elif runSet==2:
    legend2=TLegend(0.15,0.8,0.5,0.85,"p_{T}>1 TeV, |#eta|<2.5")
  else:
    legend2=TLegend(0.15,0.8,0.5,0.85,"p_{T}>500 GeV, |#eta|<2.5")
  legend2.SetTextSize(0.03)
  legend2.SetFillStyle(0)
  legend2.Draw("same")

  if runSet==2:
    banner = TLatex(0.27,0.93,"CMS Preliminary, 19.6 fb^{-1}, #sqrt{s} = 8TeV, dijets");
  else:
    banner = TLatex(0.3,0.93,"CMS Simulation, #sqrt{s} = 8TeV, dijets");
  banner.SetNDC()
  banner.SetTextSize(0.04)
  banner.Draw();  

  if "aftermass" in names[plots.index(plot)]:
     legend3=TLegend(0.15,0.75,0.5,0.8,"70 < m_{j} < 100 GeV")
     legend3.SetTextSize(0.03)
     legend3.SetFillStyle(0)
     legend3.Draw("same")

  canvas.SaveAs("substructure_pas_"+names[plots.index(plot)]+"_"+str(runSet)+".png")
  canvas.SaveAs("substructure_pas_"+names[plots.index(plot)]+"_"+str(runSet)+".pdf")
  canvas.SaveAs("substructure_pas_"+names[plots.index(plot)]+"_"+str(runSet)+".root")
  canvas.SaveAs("substructure_pas_"+names[plots.index(plot)]+"_"+str(runSet)+".C")
  canvas.SaveAs("substructure_pas_"+names[plots.index(plot)]+"_"+str(runSet)+".eps")
