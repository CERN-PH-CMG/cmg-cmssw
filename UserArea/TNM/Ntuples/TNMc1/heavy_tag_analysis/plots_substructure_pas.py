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

 runSet=6

 if runSet==1:
  samples = ["substructure_pas_QCD1000.root",
             "substructure_pas_WWPy61000.root",
            ]
  colors=[1,1,1,2,2,2]
  styles=[2,3,1,2,3,1]
  widths=[2,1,2,2,1,2]
  sets=["Gen","lowPU",""]

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
           ("Jet1Ncharged01","((abs(Jet1eta)<2.5)&&(Jet1pt>500))","charged particles (p_{T}^{rel}>0.1)", ),
           ("Jet1Nneutral01","((abs(Jet1eta)<2.5)&&(Jet1pt>500))","neutral particles (p_{T}^{rel}>0.1)", ),
           ("Jet1ChargedPt2","((abs(Jet1eta)<2.5)&&(Jet1pt>500))","3rd charged particle p_{T}", ),
           ("Jet1Pt2","((abs(Jet1eta)<2.5)&&(Jet1pt>500))","3rd particle p_{T}", ),
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
  sets=[""]

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
           ("Jet1jetCharge03","((abs(Jet1eta)<2.5)&&(Jet1pt>1000)&&(Jet1Mass>70)&&(Jet1Mass<100)&&(Jet1MassDrop<0.25))","jet charge (#kappa=0.3)", ),
           ("Jet1nConstituents","((abs(Jet1eta)<2.5)&&(Jet1pt>1000))","jet constituents", ),
           ("Jet1Ncharged01","((abs(Jet1eta)<2.5)&&(Jet1pt>1000))","charged particles (p_{T}^{rel}>0.1)", ),
           ("Jet1Nneutral01","((abs(Jet1eta)<2.5)&&(Jet1pt>1000))","neutral particles (p_{T}^{rel}>0.1)", ),
           ("Jet1ChargedPt2","((abs(Jet1eta)<2.5)&&(Jet1pt>1000))","3rd charged particle p_{T}", ),
           ("Jet1Pt2","((abs(Jet1eta)<2.5)&&(Jet1pt>1000))","3rd particle p_{T}", ),
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
  sets=[""]

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
           ("Jet1Ncharged01","((abs(Jet1eta)<2.5)&&(Jet1pt>500))","charged particles (p_{T}^{rel}>0.1)", ),
           ("Jet1Nneutral01","((abs(Jet1eta)<2.5)&&(Jet1pt>500))","neutral particles (p_{T}^{rel}>0.1)", ),
           ("Jet1ChargedPt2","((abs(Jet1eta)<2.5)&&(Jet1pt>500))","3rd charged particle p_{T}", ),
           ("Jet1Pt2","((abs(Jet1eta)<2.5)&&(Jet1pt>500))","3rd particle p_{T}", ),
           ]

 if runSet==4:
  samples = ["substructure_pas_QCD1000.root",
             "substructure_pas_QCD1000.root",
             "substructure_pas_WWPy61000.root",
             "substructure_pas_WWPy61000.root",
             ]
  colors=[1,1,2,2]
  styles=[2,1,2,1]
  widths=[1,2,1,2]
  sets=[""]

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
           ("Jet1Ncharged01","((abs(Jet1eta)<2.5)&&(Jet1pt>500))","charged particles (p_{T}^{rel}>0.1)", ),
           ("Jet1Nneutral01","((abs(Jet1eta)<2.5)&&(Jet1pt>500))","neutral particles (p_{T}^{rel}>0.1)", ),
           ("Jet1ChargedPt2","((abs(Jet1eta)<2.5)&&(Jet1pt>500))","3rd charged particle p_{T}", ),
           ("Jet1Pt2","((abs(Jet1eta)<2.5)&&(Jet1pt>500))","3rd particle p_{T}", ),
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
	   "ncharged01",
	   "nneutral01",
	   "chargedpt2",
	   "pt2",
	   ]

 if runSet==5:
  samples = ["substructure_pas_QCD1000.root",
             "substructure_pas_WWPy61000.root",
            ]
  colors=[1,1,1,2,2,2]
  styles=[2,3,1,2,3,1]
  widths=[2,1,2,2,1,2]
  sets=["Gen","GenPt2",""]

  plots = [("Jet1Nsub","((abs(Jet1eta)<2.5)&&(Jet1pt>500))","#tau_{2}/#tau_{1}", ),
           ]

  names = ["tau21",
	   ]

 if runSet==6:
  samples = ["substructure_pas_WWPy61000.root",
             "substructure_pas_WWPy61000_12PU.root",
             "substructure_pas_WWPy61000_22PU.root",
             "substructure_pas_WWPy61000.root",
             ]
  colors=[1,2,2,4,4,2,4]
  styles=[1,2,3,2,3,1,1]
  widths=[2,2,2,2,2,1,1]
  sets=["Gen","GenCHS","lowPU",""]

  plots = [("Jet1Nsub","((abs(Jet1eta)<2.5)&&(Jet1pt>500))","#tau_{2}/#tau_{1}", ),
           ("Jet1C2beta17","((abs(Jet1eta)<2.5)&&(Jet1pt>500))","C_{2} (#beta=1.7)", ),
           ("Jet1nConstituents","((abs(Jet1eta)<2.5)&&(Jet1pt>500))","jet constituents", ),
           ]

  names = ["tau21",
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
  s=0
  for sample in samples:
   s+=1
   for gen in sets:
    if names[plots.index(plot)]=="npu" and gen=="Gen":
       continue
    if runSet==6 and not gen=="Gen" and s==1:
       continue
    if runSet==6 and not "Gen" in gen and (s==2 or s==3):
       continue
    if runSet==6 and "Gen" in gen and s==4:
       continue
    print sample, gen
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
    if plot[2]=="charged particles (p_{T}^{rel}>0.1)":
       hist=TH1F("plot"+names[plots.index(plot)]+gen,"plot"+names[plots.index(plot)]+gen,25,0,25);
       hist.GetYaxis().SetRangeUser(0,50000)
    if plot[2]=="neutral particles (p_{T}^{rel}>0.1)":
       hist=TH1F("plot"+names[plots.index(plot)]+gen,"plot"+names[plots.index(plot)]+gen,25,0,25);
       hist.GetYaxis().SetRangeUser(0,50000)
    if plot[2]=="3rd charged particle p_{T}":
       hist=TH1F("plot"+names[plots.index(plot)]+gen,"plot"+names[plots.index(plot)]+gen,25,0,250);
       hist.GetYaxis().SetRangeUser(0,50000)
    if plot[2]=="3rd particle p_{T}":
       hist=TH1F("plot"+names[plots.index(plot)]+gen,"plot"+names[plots.index(plot)]+gen,25,0,250);
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
    elif runSet==4 and (counter==1 or counter==3):
        print plot[0],plot[1]+"&&(abs(Jet1eta)<1.0)"
        tree.Project("plot"+names[plots.index(plot)]+gen,plot[0],plot[1]+"&&(abs(Jet1eta)<1.0)")
    elif gen=="GenPt2":
        print gen+plot[0],plot[1]
        tree.Project("plot"+names[plots.index(plot)]+gen,"Gen"+plot[0]+"Pt2",plot[1].replace("Jet",gen+"Jet"))
    elif gen=="GenCHS" and plot[0]=="Jet1nConstituents":
        print gen+plot[0],plot[1]
        tree.Project("plot"+names[plots.index(plot)]+gen,"GenJet1NCHS",plot[1].replace("Jet","GenJet"))
    elif gen=="GenCHS":
        print gen+plot[0],plot[1]
        tree.Project("plot"+names[plots.index(plot)]+gen,"Gen"+plot[0]+"CHS",plot[1].replace("Jet","GenJet"))
    else:
        print gen+plot[0],plot[1]
        tree.Project("plot"+names[plots.index(plot)]+gen,gen+plot[0],plot[1].replace("Jet",gen+"Jet"))
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
      elif "Gen" in gen:
        hist.Draw("lhist")
      else:
        hist.Draw("hist")
    else:
      if "Run" in sample:
        hist.Draw("pesame")
      elif "Gen" in gen:
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
      elif runSet==4 and counter==0:
        legend.AddEntry(hist,"QCD |#eta| < 2.5","l")
      elif runSet==4 and counter==1:
        legend.AddEntry(hist,"QCD |#eta| < 1.0","l")
      elif gen=="lowPU":
        legend.AddEntry(hist," with 12PU + simulation","l")
      elif gen=="GenPt2":
        legend.AddEntry(hist," with p_{T}^{particles}>2 GeV","l")
      else:
        legend.AddEntry(hist," with 22PU + simulation","l")
    if "WW" in sample:
      if runSet==3 and "1000" in sample:
        legend.AddEntry(hist,"W p_{T} > 500 GeV","l")
      elif runSet==3 and "3000" in sample:
        legend.AddEntry(hist,"W p_{T} > 1.5 TeV","l")
      elif runSet==4 and counter==0:
        legend.AddEntry(hist,"W |#eta| < 2.5","l")
      elif runSet==4 and counter==1:
        legend.AddEntry(hist,"W |#eta| < 1.0","l")
      elif runSet==6 and counter==1:
        legend.AddEntry(hist," with 12PU","l")
      elif runSet==6 and counter==2:
        legend.AddEntry(hist," with 12PU + CHS","l")
      elif runSet==6 and counter==3:
        legend.AddEntry(hist," with 22PU","l")
      elif runSet==6 and counter==4:
        legend.AddEntry(hist," with 22PU + CHS","l")
      elif gen=="lowPU":
        legend.AddEntry(hist," with 12PU + simulation","l")
      elif gen=="GenPt2":
        legend.AddEntry(hist," with p_{T}^{particles}>2 GeV","l")
      elif gen=="Gen" or runSet==2:
        legend.AddEntry(hist,"G_{RS} #rightarrow WW Pythia6","l")
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
