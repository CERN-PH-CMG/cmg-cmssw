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

 theory=False
 runSet=2

 names = ["npu",
           "npv",
	   "pt",
	   "eta",
	   "mass",
	   "massdrop",
	   "massdrop_aftermass",
	   "tau21",
	   "tau21_aftermass",
	   "tau21pruned",
	   "tau21pruned_aftermass",
	   #"C2beta15",
	   "C2beta17",
	   #"C2beta20",
	   "jetcharge03",
	   "jetcharge05",
	   "jetcharge10",
	   "jetcharge03_aftermass",
	   "jetcharge05_aftermass",
	   "jetcharge10_aftermass",
	   "nconstituents",
	   "ncharged01",
	   "nneutral01",
	   "chargedpt2",
	   "pt2",
	   ]

 plots = [("nPU","((abs(Jet1eta)<2.4)&&(deta<1.3)&&(DijetMass>890)&&(Jet1pt>500)&&(Jet1pt<750))","pileup interactions"),
           ("numberOfPrimaryVertices","((abs(Jet1eta)<2.4)&&(deta<1.3)&&(DijetMass>890)&&(Jet1pt>500)&&(Jet1pt<750))","number of vertices"),
           ("Jet1pt","((abs(Jet1eta)<2.4)&&(deta<1.3)&&(DijetMass>890)&&(Jet1pt>500)&&(Jet1pt<750))","jet p_{T} (GeV)"),
           ("Jet1eta","((abs(Jet1eta)<2.4)&&(deta<1.3)&&(DijetMass>890)&&(Jet1pt>500)&&(Jet1pt<750))","jet #eta"),
           ("Jet1Mass","((abs(Jet1eta)<2.4)&&(deta<1.3)&&(DijetMass>890)&&(Jet1pt>500)&&(Jet1pt<750))","pruned jet mass (GeV)"),
           ("Jet1MassDrop","((abs(Jet1eta)<2.4)&&(deta<1.3)&&(DijetMass>890)&&(Jet1pt>500)&&(Jet1pt<750))","mass drop"),
           ("Jet1MassDrop","((abs(Jet1eta)<2.4)&&(deta<1.3)&&(DijetMass>890)&&(Jet1pt>500)&&(Jet1pt<750)&&(Jet1Mass>60)&&(Jet1Mass<100))","mass drop"),
           ("Jet1Nsub","((abs(Jet1eta)<2.4)&&(deta<1.3)&&(DijetMass>890)&&(Jet1pt>500)&&(Jet1pt<750))","#tau_{2}/#tau_{1}", ),
           ("Jet1Nsub","((abs(Jet1eta)<2.4)&&(deta<1.3)&&(DijetMass>890)&&(Jet1pt>500)&&(Jet1pt<750)&&(Jet1Mass>60)&&(Jet1Mass<100))","#tau_{2}/#tau_{1}", ),
           ("Jet1NsubPruned","((abs(Jet1eta)<2.4)&&(deta<1.3)&&(DijetMass>890)&&(Jet1pt>500)&&(Jet1pt<750))","pruned #tau_{2}/#tau_{1}", ),
           ("Jet1NsubPruned","((abs(Jet1eta)<2.4)&&(deta<1.3)&&(DijetMass>890)&&(Jet1pt>500)&&(Jet1pt<750)&&(Jet1Mass>60)&&(Jet1Mass<100))","pruned #tau_{2}/#tau_{1}", ),
           #("Jet1C2beta15","((abs(Jet1eta)<2.4)&&(deta<1.3)&&(DijetMass>890)&&(Jet1pt>500)&&(Jet1pt<750))","C_{2} (#beta=1.5)", ),
           ("Jet1C2beta17","((abs(Jet1eta)<2.4)&&(deta<1.3)&&(DijetMass>890)&&(Jet1pt>500)&&(Jet1pt<750))","C_{2} (#beta=1.7)", ),
           #("Jet1C2beta20","((abs(Jet1eta)<2.4)&&(deta<1.3)&&(DijetMass>890)&&(Jet1pt>500)&&(Jet1pt<750))","C_{2} (#beta=2.0)", ),
           ("Jet1jetCharge03","((abs(Jet1eta)<2.4)&&(deta<1.3)&&(DijetMass>890)&&(Jet1pt>500)&&(Jet1pt<750)&&(Jet1genWcharge>=0))","jet charge (#kappa=0.3)", ),
           ("Jet1jetCharge05","((abs(Jet1eta)<2.4)&&(deta<1.3)&&(DijetMass>890)&&(Jet1pt>500)&&(Jet1pt<750)&&(Jet1genWcharge>=0))","jet charge (#kappa=0.5)", ),
           ("Jet1jetCharge10","((abs(Jet1eta)<2.4)&&(deta<1.3)&&(DijetMass>890)&&(Jet1pt>500)&&(Jet1pt<750)&&(Jet1genWcharge>=0))","jet charge (#kappa=1.0)", ),
           ("Jet1jetCharge03","((abs(Jet1eta)<2.4)&&(deta<1.3)&&(DijetMass>890)&&(Jet1pt>500)&&(Jet1pt<750)&&(Jet1genWcharge>=0)&&(Jet1Mass>60)&&(Jet1Mass<100)&&(Jet1MassDrop<0.25))","jet charge (#kappa=0.3)", ),
           ("Jet1jetCharge05","((abs(Jet1eta)<2.4)&&(deta<1.3)&&(DijetMass>890)&&(Jet1pt>500)&&(Jet1pt<750)&&(Jet1genWcharge>=0)&&(Jet1Mass>60)&&(Jet1Mass<100)&&(Jet1MassDrop<0.25))","jet charge (#kappa=0.5)", ),
           ("Jet1jetCharge10","((abs(Jet1eta)<2.4)&&(deta<1.3)&&(DijetMass>890)&&(Jet1pt>500)&&(Jet1pt<750)&&(Jet1genWcharge>=0)&&(Jet1Mass>60)&&(Jet1Mass<100)&&(Jet1MassDrop<0.25))","jet charge (#kappa=1.0)", ),
           ("Jet1nConstituents","((abs(Jet1eta)<2.4)&&(deta<1.3)&&(DijetMass>890)&&(Jet1pt>500)&&(Jet1pt<750))","jet constituents", ),
           ("Jet1Ncharged01","((abs(Jet1eta)<2.4)&&(deta<1.3)&&(DijetMass>890)&&(Jet1pt>500)&&(Jet1pt<750))","charged particles (p_{T}^{rel}>0.1)", ),
           ("Jet1Nneutral01","((abs(Jet1eta)<2.4)&&(deta<1.3)&&(DijetMass>890)&&(Jet1pt>500)&&(Jet1pt<750))","neutral particles (p_{T}^{rel}>0.1)", ),
           ("Jet1ChargedPt2","((abs(Jet1eta)<2.4)&&(deta<1.3)&&(DijetMass>890)&&(Jet1pt>500)&&(Jet1pt<750))","3rd charged particle p_{T}", ),
           ("Jet1Pt2","((abs(Jet1eta)<2.4)&&(deta<1.3)&&(DijetMass>890)&&(Jet1pt>500)&&(Jet1pt<750))","3rd particle p_{T}", ),
           ]

 if runSet==1:
  samples = ["substructure_pas_QCD500.root",
             "substructure_pas_QCD1000.root",
             "substructure_pas_WWPy61000.root",
            ]
  colors=[1,1,1,2,2,2]
  styles=[2,3,1,2,3,1]
  widths=[2,1,2,2,1,2]
  sets=["Gen","lowPU",""]

 if runSet==2:
  samples = ["substructure_pas_ReRun2012ABCD.root",
             "substructure_pas_QCD500.root",
             "substructure_pas_QCD1000.root",
             "substructure_pas_QCDHerwig.root",
             "substructure_pas_QCDPythia8170.root",
             "substructure_pas_QCDPythia8300.root",
             "substructure_pas_QCDPythia8470.root",
             "substructure_pas_QCDPythia8600.root",
             "substructure_pas_QCDPythia8800.root",
             "substructure_pas_QCDPythia81000.root",
             "substructure_pas_QCDPythia81400.root",
             "substructure_pas_QCDPythia81800.root",
             "substructure_pas_ReRun2012ABCD.root",
            ]
  colors=[1,2,4,6,1]
  styles=[1,1,2,3,1]
  widths=[2,2,2,2,2]
  ndata=15000
  sets=[""]

  names = ["npv",
	   "pt",
	   "eta",
	   "mass",
	   "massdrop",
	   "massdrop_aftermass",
	   "tau21",
	   "tau21_aftermass",
	   "tau21pruned",
	   "tau21pruned_aftermass",
	   #"C2beta15",
	   "C2beta17",
	   #"C2beta20",
	   "jetcharge03",
	   "jetcharge05",
	   "jetcharge10",
	   "jetcharge03_aftermass",
	   "jetcharge05_aftermass",
	   "jetcharge10_aftermass",
	   "nconstituents",
	   "ncharged01",
	   "nneutral01",
	   "chargedpt2",
	   "pt2",
	   ]
  plots = [("numberOfPrimaryVertices","weight*vertexWeight*((abs(Jet1eta)<2.4)&&(deta<1.3)&&(DijetMass>890)&&(Jet1pt>500)&&(Jet1pt<750))","number of vertices"),
           ("Jet1pt","weight*vertexWeight*((abs(Jet1eta)<2.4)&&(deta<1.3)&&(DijetMass>890)&&(Jet1pt>500)&&(Jet1pt<750))","jet p_{T} (GeV)"),
           ("Jet1eta","weight*vertexWeight*((abs(Jet1eta)<2.4)&&(deta<1.3)&&(DijetMass>890)&&(Jet1pt>500)&&(Jet1pt<750))","jet #eta"),
           ("Jet1Mass","weight*vertexWeight*((abs(Jet1eta)<2.4)&&(deta<1.3)&&(DijetMass>890)&&(Jet1pt>500)&&(Jet1pt<750))","pruned jet mass (GeV)"),
           ("Jet1MassDrop","weight*vertexWeight*((abs(Jet1eta)<2.4)&&(deta<1.3)&&(DijetMass>890)&&(Jet1pt>500)&&(Jet1pt<750))","mass drop"),
           ("Jet1MassDrop","weight*vertexWeight*((abs(Jet1eta)<2.4)&&(deta<1.3)&&(DijetMass>890)&&(Jet1pt>500)&&(Jet1pt<750)&&(Jet1Mass>60)&&(Jet1Mass<100))","mass drop"),
           ("Jet1Nsub","weight*vertexWeight*((abs(Jet1eta)<2.4)&&(deta<1.3)&&(DijetMass>890)&&(Jet1pt>500)&&(Jet1pt<750))","#tau_{2}/#tau_{1}", ),
           ("Jet1Nsub","weight*vertexWeight*((abs(Jet1eta)<2.4)&&(deta<1.3)&&(DijetMass>890)&&(Jet1pt>500)&&(Jet1pt<750)&&(Jet1Mass>60)&&(Jet1Mass<100))","#tau_{2}/#tau_{1}", ),
           ("Jet1NsubPruned","weight*vertexWeight*((abs(Jet1eta)<2.4)&&(deta<1.3)&&(DijetMass>890)&&(Jet1pt>500)&&(Jet1pt<750))","pruned #tau_{2}/#tau_{1}", ),
           ("Jet1NsubPruned","weight*vertexWeight*((abs(Jet1eta)<2.4)&&(deta<1.3)&&(DijetMass>890)&&(Jet1pt>500)&&(Jet1pt<750)&&(Jet1Mass>60)&&(Jet1Mass<100))","pruned #tau_{2}/#tau_{1}", ),
           #("Jet1C2beta15","weight*vertexWeight*((abs(Jet1eta)<2.4)&&(deta<1.3)&&(DijetMass>890)&&(Jet1pt>500)&&(Jet1pt<750))","C_{2} (#beta=1.5)", ),
           ("Jet1C2beta17","weight*vertexWeight*((abs(Jet1eta)<2.4)&&(deta<1.3)&&(DijetMass>890)&&(Jet1pt>500)&&(Jet1pt<750))","C_{2} (#beta=1.7)", ),
           #("Jet1C2beta20","weight*vertexWeight*((abs(Jet1eta)<2.4)&&(deta<1.3)&&(DijetMass>890)&&(Jet1pt>500)&&(Jet1pt<750))","C_{2} (#beta=2.0)", ),
           ("Jet1jetCharge03","weight*vertexWeight*((abs(Jet1eta)<2.4)&&(deta<1.3)&&(DijetMass>890)&&(Jet1pt>500)&&(Jet1pt<750))","jet charge (#kappa=0.3)", ),
           ("Jet1jetCharge05","weight*vertexWeight*((abs(Jet1eta)<2.4)&&(deta<1.3)&&(DijetMass>890)&&(Jet1pt>500)&&(Jet1pt<750))","jet charge (#kappa=0.5)", ),
           ("Jet1jetCharge10","weight*vertexWeight*((abs(Jet1eta)<2.4)&&(deta<1.3)&&(DijetMass>890)&&(Jet1pt>500)&&(Jet1pt<750))","jet charge (#kappa=1.0)", ),
           ("Jet1jetCharge03","weight*vertexWeight*((abs(Jet1eta)<2.4)&&(deta<1.3)&&(DijetMass>890)&&(Jet1pt>500)&&(Jet1pt<750)&&(Jet1Mass>60)&&(Jet1Mass<100)&&(Jet1MassDrop<0.25))","jet charge (#kappa=0.3)", ),
           ("Jet1jetCharge05","weight*vertexWeight*((abs(Jet1eta)<2.4)&&(deta<1.3)&&(DijetMass>890)&&(Jet1pt>500)&&(Jet1pt<750)&&(Jet1Mass>60)&&(Jet1Mass<100)&&(Jet1MassDrop<0.25))","jet charge (#kappa=0.5)", ),
           ("Jet1jetCharge10","weight*vertexWeight*((abs(Jet1eta)<2.4)&&(deta<1.3)&&(DijetMass>890)&&(Jet1pt>500)&&(Jet1pt<750)&&(Jet1Mass>60)&&(Jet1Mass<100)&&(Jet1MassDrop<0.25))","jet charge (#kappa=1.0)", ),
           ("Jet1nConstituents","weight*vertexWeight*((abs(Jet1eta)<2.4)&&(deta<1.3)&&(DijetMass>890)&&(Jet1pt>500)&&(Jet1pt<750))","jet constituents", ),
           ("Jet1Ncharged01","weight*vertexWeight*((abs(Jet1eta)<2.4)&&(deta<1.3)&&(DijetMass>890)&&(Jet1pt>500)&&(Jet1pt<750))","charged particles (p_{T}^{rel}>0.1)", ),
           ("Jet1Nneutral01","weight*vertexWeight*((abs(Jet1eta)<2.4)&&(deta<1.3)&&(DijetMass>890)&&(Jet1pt>500)&&(Jet1pt<750))","neutral particles (p_{T}^{rel}>0.1)", ),
           ("Jet1ChargedPt2","weight*vertexWeight*((abs(Jet1eta)<2.4)&&(deta<1.3)&&(DijetMass>890)&&(Jet1pt>500)&&(Jet1pt<750))","3rd charged particle p_{T}", ),
           ("Jet1Pt2","weight*vertexWeight*((abs(Jet1eta)<2.4)&&(deta<1.3)&&(DijetMass>890)&&(Jet1pt>500)&&(Jet1pt<750))","3rd particle p_{T}", ),
           ]

 if runSet==3:
  samples = ["substructure_pas_QCD500.root",
             "substructure_pas_QCD1000.root",
             "substructure_pas_QCD500.root",
             "substructure_pas_QCD1000.root",
             "substructure_pas_WWPy61000.root",
             "substructure_pas_WWPy63000.root",
             ]
  colors=[1,1,2,2]
  styles=[2,1,2,1]
  widths=[1,2,1,2]
  sets=[""]

 if runSet==4:
  samples = ["substructure_pas_QCD500.root",
             "substructure_pas_QCD1000.root",
             "substructure_pas_QCD500.root",
             "substructure_pas_QCD1000.root",
             "substructure_pas_WWPy61000.root",
             "substructure_pas_WWPy61000.root",
             ]
  colors=[1,1,2,2]
  styles=[2,1,2,1]
  widths=[1,2,1,2]
  sets=[""]

 if runSet==5:
  samples = ["substructure_pas_QCD500.root",
             "substructure_pas_QCD1000.root",
             "substructure_pas_WWPy61000.root",
            ]
  colors=[1,1,1,2,2,2]
  styles=[2,3,1,2,3,1]
  widths=[2,1,2,2,1,2]
  sets=["Gen","GenPt2",""]

  plots = [("Jet1Nsub","((abs(Jet1eta)<2.4)&&(deta<1.3)&&(DijetMass>890)&&(Jet1pt>500)&&(Jet1pt<750))","#tau_{2}/#tau_{1}", ),
           ]

  names = ["tau21",
	   ]

 if runSet==6:
  if theory:
   samples = ["substructure_pas_WWPy61000.root",
             "substructure_pas_WWPy61000_12PU.root",
             "substructure_pas_WWPy61000_22PU.root",
             ]
  else:
   samples = ["substructure_pas_WWPy61000.root",
             "substructure_pas_WWPy61000_12PU.root",
             "substructure_pas_WWPy61000_22PU.root",
             "substructure_pas_WWPy61000.root",
             ]
  colors=[1,7,2,7,2,4,4,4,4,2,4]
  styles=[1,2,3,4,5,2,3,4,5,1,1]
  widths=[2,2,2,2,2,2,2,2,2,1,1]
  sets=["Gen","GenCHS","lowPU",""]#"GenPUcorrected","GenPUcorrectedCHS"

  plots = [("Jet1Nsub","((abs(Jet1eta)<2.4)&&(deta<1.3)&&(DijetMass>890)&&(Jet1pt>500)&&(Jet1pt<750))","#tau_{2}/#tau_{1}", ),
           ("Jet1C2beta17","((abs(Jet1eta)<2.4)&&(deta<1.3)&&(DijetMass>890)&&(Jet1pt>500)&&(Jet1pt<750))","C_{2} (#beta=1.7)", ),
           ("Jet1nConstituents","((abs(Jet1eta)<2.4)&&(deta<1.3)&&(DijetMass>890)&&(Jet1pt>500)&&(Jet1pt<750))","jet constituents", ),
           ]

  names = ["tau21",
	   "C2beta17",
	   "nconstituents",
	   ]

 if runSet==7:
  samples = ["substructure_pas_QCD500.root",
             "substructure_pas_QCD1000.root",
             "substructure_pas_WWPy61000.root",
             "substructure_pas_WWPy61000.root",
            ]
  colors=[1,1,2,2,4,4]
  styles=[2,1,2,1,2,1]
  widths=[2,2,2,2,1,1]
  sets=["Gen",""]

  plots = [("Jet1jetCharge03","((abs(Jet1eta)<2.4)&&(deta<1.3)&&(DijetMass>890)&&(Jet1pt>500)&&(Jet1pt<750))","jet charge (#kappa=0.3)", ),
           ("Jet1jetCharge05","((abs(Jet1eta)<2.4)&&(deta<1.3)&&(DijetMass>890)&&(Jet1pt>500)&&(Jet1pt<750))","jet charge (#kappa=0.5)", ),
           ("Jet1jetCharge10","((abs(Jet1eta)<2.4)&&(deta<1.3)&&(DijetMass>890)&&(Jet1pt>500)&&(Jet1pt<750))","jet charge (#kappa=1.0)", ),
           ("Jet1jetCharge03","((abs(Jet1eta)<2.4)&&(deta<1.3)&&(DijetMass>890)&&(Jet1pt>500)&&(Jet1pt<750)&&(Jet1Mass>60)&&(Jet1Mass<100)&&(Jet1MassDrop<0.25))","jet charge (#kappa=0.3)", ),
           ("Jet1jetCharge05","((abs(Jet1eta)<2.4)&&(deta<1.3)&&(DijetMass>890)&&(Jet1pt>500)&&(Jet1pt<750)&&(Jet1Mass>60)&&(Jet1Mass<100)&&(Jet1MassDrop<0.25))","jet charge (#kappa=0.5)", ),
           ("Jet1jetCharge10","((abs(Jet1eta)<2.4)&&(deta<1.3)&&(DijetMass>890)&&(Jet1pt>500)&&(Jet1pt<750)&&(Jet1Mass>60)&&(Jet1Mass<100)&&(Jet1MassDrop<0.25))","jet charge (#kappa=1.0)", ),
           ]

  names = ["jetcharge03",
	   "jetcharge05",
	   "jetcharge10",
	   "jetcharge03_aftermass",
	   "jetcharge05_aftermass",
	   "jetcharge10_aftermass",
	   ]

 if runSet==8:
  samples = ["substructure_pas_WWPy61000.root",
             "substructure_pas_WWHpp1000.root",
             ]
  colors=[1,1,1,2,2,2]
  styles=[2,3,1,2,3,1]
  widths=[2,1,2,2,1,2]
  sets=["Gen",""]

  plots = [("abs(costheta1)","((abs(Jet1eta)<2.4)&&(deta<1.3)&&(DijetMass>890)&&(Jet1pt>500)&&(Jet1pt<750))","cos #theta", ),
           ("abs(costheta2)","((abs(Jet1eta)<2.4)&&(deta<1.3)&&(DijetMass>890)&&(Jet1pt>500)&&(Jet1pt<750))","cos #theta", ),
           ] + plots

  names = ["costheta1",
	   "costheta2",
	   ] + names


 results=[]
 for plot in plots:
  if runSet==2:
    canvas = TCanvas("","",0,0,200,260)
    canvas.Divide(1,2,0,0,0)
    canvas.GetPad(1).SetPad(0.0,0.28,1.0,1.0)
    canvas.GetPad(1).SetLeftMargin(0.13)
    canvas.GetPad(1).SetRightMargin(0.08)
    canvas.GetPad(1).SetTopMargin(0.08)
    canvas.GetPad(1).SetBottomMargin(0.05)
    canvas.GetPad(2).SetPad(0.0,0.0,1.0,0.28)
    canvas.GetPad(2).SetLeftMargin(0.13)
    canvas.GetPad(2).SetRightMargin(0.08)
    canvas.GetPad(2).SetTopMargin(0.08)
    canvas.GetPad(2).SetBottomMargin(0.40)
    canvas.cd(1)
  else:
    canvas = TCanvas("","",0,0,200,200)
  canvas.GetPad(1).SetLogy(False)
  if runSet==2:
    legend=TLegend(0.45,0.7,0.85,0.9)
  else:
    legend=TLegend(0.45,0.6,0.85,0.9)
  dataPlotted=False
  counter=0
  integral=1
  originalIntegral={}
  maximum=0
  s=0
  hists=[]
  for sample in samples:
   s+=1
   for gen in sets:
    if (names[plots.index(plot)]=="npu" or names[plots.index(plot)]=="npv" or "costheta" in names[plots.index(plot)]) and gen=="Gen":
       continue
    if runSet==6 and not gen=="Gen" and s==1:
       continue
    if runSet==6 and not "Gen" in gen and (s==2 or s==3):
       continue
    if runSet==6 and "Gen" in gen and s==4:
       continue
    if runSet==6 and "PUcorrected" in gen and plot[0]!="Jet1Nsub":
       continue
    print sample, gen

    f=TFile.Open(sample)
    tree=f.Get("dijetWtag")

    signal = "Hpp" in sample or "Py6" in sample
    histname="plot"+names[plots.index(plot)]+gen+str(s)
    if plot[2]=="cos #theta":
       hist=TH1F(histname,histname,25,0,1);
       hist.GetYaxis().SetRangeUser(0,50000)
    if plot[2]=="pileup interactions":
       hist=TH1F(histname,histname,25,0,50);
       hist.GetYaxis().SetRangeUser(0,50000)
    if plot[2]=="number of vertices":
       hist=TH1F(histname,histname,25,0,50);
       hist.GetYaxis().SetRangeUser(0,50000)
    if plot[2]=="jet #eta":
       hist=TH1F(histname,histname,25,-2.4,2.4);
       hist.GetYaxis().SetRangeUser(0,50000)
    if "C_{2}" in plot[2]:
       hist=TH1F(histname,histname,25,0,0.8);
       hist.GetYaxis().SetRangeUser(0,50000)
    if "jet charge (#kappa=0.3)" in plot[2]:
       hist=TH1F(histname,histname,25,-4,4);
       hist.GetYaxis().SetRangeUser(0,50000)
       hist.GetXaxis().SetRangeUser(-4,4)
    if "jet charge (#kappa=0.5)" in plot[2]:
       hist=TH1F(histname,histname,25,-2,2);
       hist.GetYaxis().SetRangeUser(0,50000)
       hist.GetXaxis().SetRangeUser(-2,2)
    if "jet charge (#kappa=1.0)" in plot[2]:
       hist=TH1F(histname,histname,25,-1,1);
       hist.GetYaxis().SetRangeUser(0,50000)
       hist.GetXaxis().SetRangeUser(-1,1)
    if plot[2]=="jet constituents":
       hist=TH1F(histname,histname,25,0,200);
       hist.GetYaxis().SetRangeUser(0,50000)
    if plot[2]=="charged particles (p_{T}^{rel}>0.1)":
       hist=TH1F(histname,histname,25,0,25);
       hist.GetYaxis().SetRangeUser(0,50000)
    if plot[2]=="neutral particles (p_{T}^{rel}>0.1)":
       hist=TH1F(histname,histname,25,0,25);
       hist.GetYaxis().SetRangeUser(0,50000)
    if plot[2]=="3rd charged particle p_{T}":
       hist=TH1F(histname,histname,25,0,250);
       hist.GetYaxis().SetRangeUser(0,50000)
    if plot[2]=="3rd particle p_{T}":
       hist=TH1F(histname,histname,25,0,250);
       hist.GetYaxis().SetRangeUser(0,50000)
    if "jet p_{T}" in plot[2]:
       if runSet==3:
          hist=TH1F(histname,histname,40,0,3000);
       elif runSet==2:
          hist=TH1F(histname,histname,40,300,1000);
       else:
          hist=TH1F(histname,histname,40,300,1000);
       hist.GetYaxis().SetRangeUser(0.001,50000)
       canvas.GetPad(1).SetLogy(True)
    if "pruned jet mass" in plot[2]:
       hist=TH1F(histname,histname,40,0,150);
       hist.GetYaxis().SetRangeUser(0,50000)
    if plot[2]=="mass drop":
       hist=TH1F(histname,histname,25,0,1);
       hist.GetYaxis().SetRangeUser(0,60000)
       if "aftermass" in names[plots.index(plot)]:
           hist.GetYaxis().SetRangeUser(0,6000)
    if "#tau_{2}/#tau_{1}" in plot[2]:
       hist=TH1F(histname,histname,25,0,1);
       hist.GetYaxis().SetRangeUser(0,75000)
       if "aftermass" in names[plots.index(plot)]:
           hist.GetYaxis().SetRangeUser(0,5000)

    if gen=="lowPU":
        variable,cutstring=plot[0],plot[1]+"&&(nPU<17)"
    elif runSet==3 and (s==3 or s==4 or s==6):
        variable,cutstring=plot[0],plot[1].replace("&&(Jet1pt>500)&&(Jet1pt<750)","&&(Jet1pt>1500)&&(Jet1pt<2000)")
    elif runSet==4 and (counter==1 or counter==3):
        variable,cutstring=plot[0],plot[1]+"&&(abs(Jet1eta)<1.0)"
    elif gen=="GenPt2":
        variable,cutstring="Gen"+plot[0]+"Pt2",plot[1].replace("Jet","GenJet").replace("DijetMass","GenDijetMass").replace("deta","Gendeta")
    elif gen=="GenCHS" and plot[0]=="Jet1nConstituents":
        variable,cutstring="GenJet1NCHS",plot[1].replace("Jet","GenJet").replace("DijetMass","GenDijetMass").replace("deta","Gendeta")
    elif gen=="GenCHS":
        variable,cutstring="Gen"+plot[0]+"CHS",plot[1].replace("Jet","GenJet").replace("DijetMass","GenDijetMass").replace("deta","Gendeta")
    elif gen=="GenPUcorrected":
        variable,cutstring="Gen"+plot[0]+"PUcorrected",plot[1].replace("Jet","GenJet").replace("DijetMass","GenDijetMass").replace("deta","Gendeta")
    elif gen=="GenPUcorrectedCHS":
        variable,cutstring="Gen"+plot[0]+"PUcorrectedCHS",plot[1].replace("Jet","GenJet").replace("DijetMass","GenDijetMass").replace("deta","Gendeta")
    elif runSet==7 and s==3 and gen=="Gen":
        variable,cutstring=gen+plot[0],plot[1].replace("Jet",gen+"Jet").replace("DijetMass","GenDijetMass").replace("deta","Gendeta")+"&&(Jet1genWcharge<0)"
    elif runSet==7 and s==3 and gen!="Gen":
        variable,cutstring=gen+plot[0],plot[1].replace("Jet",gen+"Jet")+"&&(Jet1genWcharge<0)"
    elif runSet==7 and s==4 and gen=="Gen":
        variable,cutstring=gen+plot[0],plot[1].replace("Jet",gen+"Jet").replace("DijetMass","GenDijetMass").replace("deta","Gendeta")+"&&(Jet1genWcharge>0)"
    elif runSet==7 and s==4 and gen!="Gen":
        variable,cutstring=gen+plot[0],plot[1].replace("Jet",gen+"Jet")+"&&(Jet1genWcharge>0)"
    elif gen=="Gen":
        variable,cutstring=gen+plot[0],plot[1].replace("Jet",gen+"Jet").replace(gen+"Jet1genWcharge","Jet1genWcharge").replace("DijetMass","GenDijetMass").replace("deta","Gendeta")
    else:
        variable,cutstring=gen+plot[0],plot[1].replace("Jet",gen+"Jet").replace(gen+"Jet1genWcharge","Jet1genWcharge")
    print histname,variable,cutstring
    tree.Project(histname,variable,cutstring)
    if "QCD" in sample:
        originalIntegral[histname]=hist.Integral()
    hist.SetTitle("")
    hist.SetFillStyle(0)
    hist.SetMarkerStyle(20)
    #hist.SetMarkerSize(2)
    if runSet==2:
      hist.GetXaxis().SetTitle("")
      hist.GetXaxis().SetLabelColor(0)
    else:
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

    hists+=[hist]

    if "QCD1000" in sample:
        histname500="plot"+names[plots.index(plot)]+gen+str(s-1)
        for his in reversed(hists):
	    if histname500==his.GetName():
	        oldIntegral=his.Integral()
		if his.Integral()>0:
                    his.Scale(originalIntegral[histname500]/his.Integral())
		if hist.Integral()>0:
                    hist.Scale(originalIntegral[histname]/hist.Integral())
		weight=204.0/13798133*30522161/8426.0
	        his.Add(hist,weight)
		if oldIntegral>0:
                    his.Scale(oldIntegral/his.Integral())
		else:
                    his.Scale(integral/his.Integral())
 	        hist=his
 	        break
    if "QCD500" in sample:
	continue
    
    if "QCDPythia8" in sample and not "170" in sample:
        samplenames=["170","300","470","600","800","1000","1400","1800"]
	samplenumbers=[800046,490042,500051,492988,400059,400050,200070,194313]
	samplecrossections=[37974.99,1938.868,124.8942,29.55049,3.871308,0.8031018,0.03637225,0.00197726]
	samplenumber=0
        for samplename in samplenames:
          if samplename in sample:
            samplenumber=samplenames.index(samplename)
        histnameFirst="plot"+names[plots.index(plot)]+gen+str(s-samplenumber)
        for his in reversed(hists):
	    if histnameFirst==his.GetName():
	        oldIntegral=his.Integral()
		if his.Integral()>0:
                    his.Scale(originalIntegral[histnameFirst]/his.Integral())
		if hist.Integral()>0:
                    hist.Scale(originalIntegral[histname]/hist.Integral())
                weight=samplecrossections[samplenumber]/samplenumbers[samplenumber]*samplenumbers[0]/samplecrossections[0]
	        his.Add(hist,weight)
		if oldIntegral>0:
		    originalIntegral[histnameFirst]=his.Integral()
                    his.Scale(oldIntegral/his.Integral())
		elif his.Integral()>0:
		    originalIntegral[histnameFirst]=his.Integral()
                    his.Scale(integral/his.Integral())
 	        hist=his
		break
    if "QCDPythia8" in sample and not "1800" in sample:
        continue
    
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

    if hist.GetMaximum()>maximum and hist.GetMaximum()<hist.Integral():
        maximum=hist.GetMaximum()

    if "jet p_{T}" in plot[2] and runSet==2:
  	hists[0].GetYaxis().SetRangeUser(100,maximum*20.0)
    elif "jet p_{T}" in plot[2]:
  	hists[0].GetYaxis().SetRangeUser(0.001,maximum*20.0)
    else:
        hists[0].GetYaxis().SetRangeUser(0,maximum*2.0)

    if runSet==2:
      canvas.cd(2)
      ratio=hist.Clone(hist.GetName()+"clone")
      hists+=[ratio]
      ratio.Divide(hist,hists[0])
      for b in range(hist.GetNbinsX()):
        if hists[0].GetBinContent(b+1)>0:
          ratio.SetBinError(b+1,hists[0].GetBinError(b+1)/hists[0].GetBinContent(b+1))
      ratio.GetYaxis().SetTitle("data/MC")
      ratio.GetYaxis().SetTitleColor(1)
      ratio.GetYaxis().SetTitleSize(0.16)
      ratio.SetMarkerSize(0.1)
      ratio.GetYaxis().SetLabelSize(0.14)
      ratio.GetYaxis().SetRangeUser(0,2)
      ratio.GetYaxis().SetNdivisions(503)
      ratio.GetXaxis().SetLabelColor(1)
      ratio.GetXaxis().SetTitle(plot[2])
      ratio.GetXaxis().SetTitleSize(0.16)
      ratio.GetXaxis().SetTitleOffset(0.8)
      ratio.GetXaxis().SetLabelSize(0.14)
      if counter==0:
        ratio.Draw("histe")
      else:
        ratio.Draw("histsame")
      #line=TLine(ratio.GetXaxis().GetBinLowEdge(1),1,ratio.GetXaxis().GetBinLowEdge(ratio.GetNbinsX()+1),1)
      #hists+=[line]
      #line.Draw("same")
      canvas.cd(1)

    if "Run" in sample and counter==0:
      legend.AddEntry(hist,"data","ple")
    if "QCD1000" in sample:
      if gen=="Gen" or runSet==2:
        legend.AddEntry(hist,"QCD Madgraph+Pythia6","l")
      elif runSet==3 and counter==0:
        legend.AddEntry(hist,"QCD 500 < p_{T} < 750 GeV","l")
      elif runSet==3 and counter==1:
        legend.AddEntry(hist,"QCD 1.5 < p_{T} < 2.0 TeV","l")
      elif runSet==4 and counter==0:
        legend.AddEntry(hist,"QCD |#eta| < 2.4","l")
      elif runSet==4 and counter==1:
        legend.AddEntry(hist,"QCD |#eta| < 1.0","l")
      elif gen=="lowPU":
        legend.AddEntry(hist," + <PU>=12 + simulation","l")
      elif gen=="GenPt2":
        legend.AddEntry(hist," with p_{T}^{particles}>2 GeV","l")
      else:
        legend.AddEntry(hist," + <PU>=22 + simulation","l")
    if "QCDHerwig" in sample:
        legend.AddEntry(hist,"QCD Herwig++","l")
    if "QCDPythia8" in sample:
        legend.AddEntry(hist,"QCD Pythia8","l")
    if "WWPy6" in sample:
      if runSet==3 and "1000" in sample:
        legend.AddEntry(hist,"W 500 < p_{T} < 750 GeV","l")
      elif runSet==3 and "3000" in sample:
        legend.AddEntry(hist,"W 1.5 < p_{T} < 2.0 TeV","l")
      elif runSet==4 and counter==0:
        legend.AddEntry(hist,"W |#eta| < 2.4","l")
      elif runSet==4 and counter==1:
        legend.AddEntry(hist,"W |#eta| < 1.0","l")
      elif runSet==6 and counter==1:
        legend.AddEntry(hist," + <PU>=12","l")
      elif runSet==6 and counter==2:
        legend.AddEntry(hist," + <PU>=12 + CHS","l")
      #elif runSet==6 and counter==3:
      #  legend.AddEntry(hist," + <PU>=12 + SC","l")
      #elif runSet==6 and counter==4:
      #  legend.AddEntry(hist," + <PU>=12 + CHS + SC","l")
      elif runSet==6 and counter==3:
        legend.AddEntry(hist," + <PU>=22","l")
      elif runSet==6 and counter==4:
        legend.AddEntry(hist," + <PU>=22 + CHS","l")
      #elif runSet==6 and counter==7:
      #  legend.AddEntry(hist," + <PU>=22 + SC","l")
      #elif runSet==6 and counter==8:
      #  legend.AddEntry(hist," + <PU>=22 + CHS + SC","l")
      elif gen=="lowPU":
        legend.AddEntry(hist," + <PU>=12 + simulation","l")
      elif gen=="GenPt2":
        legend.AddEntry(hist," with p_{T}^{particles}>2 GeV","l")
      elif gen=="Gen" and runSet==7 and s==3:
        legend.AddEntry(hist,"W^{+} (G_{RS}) Pythia6","l")
      elif gen=="Gen" and runSet==7 and s==4:
        legend.AddEntry(hist,"W^{-} (G_{RS}) Pythia6","l")
      elif gen=="Gen" or runSet==2 or (runSet==8 and "costheta" in names[plots.index(plot)]):
        legend.AddEntry(hist,"G_{RS} #rightarrow WW Pythia6","l")
      else:
        legend.AddEntry(hist," + <PU>=22 + simulation","l")
    if "WWHpp" in sample:
      if gen=="Gen" or runSet==2 or (runSet==8 and "costheta" in names[plots.index(plot)]):
        legend.AddEntry(hist,"G_{RS} #rightarrow WW Herwig++","l")
      else:
        legend.AddEntry(hist," + <PU>=12 + simulation","l")
    counter+=1

  legend.SetTextSize(0.036)
  legend.SetFillStyle(0)
  legend.Draw("same")

  legend4=TLegend(0.23,0.85,0.5,0.9,"CA R=0.8")
  legend4.SetTextSize(0.03)
  legend4.SetFillStyle(0)
  legend4.Draw("same")

  if runSet!=3:
    legend2=TLegend(0.15,0.8,0.5,0.85,"500 < p_{T} < 750 GeV")
    legend2.SetTextSize(0.03)
    legend2.SetFillStyle(0)
    legend2.Draw("same")

  legend2a=TLegend(0.23,0.75,0.5,0.8,"|#eta|<2.4")
  legend2a.SetTextSize(0.03)
  legend2a.SetFillStyle(0)
  legend2a.Draw("same")

  if runSet==2:
    banner = TLatex(0.27,0.93,"CMS Preliminary, 19.6 fb^{-1}, #sqrt{s} = 8 TeV, dijets");
  elif runSet==6 and theory:
    banner = TLatex(0.32,0.93,"Pythia6, #sqrt{s} = 8 TeV, dijets");
  else:
    banner = TLatex(0.3,0.93,"CMS Simulation, #sqrt{s} = 8 TeV, dijets");
  banner.SetNDC()
  banner.SetTextSize(0.04)
  banner.Draw();  

  if "aftermass" in names[plots.index(plot)]:
     legend3=TLegend(0.15,0.7,0.5,0.75,"60 < m_{j} < 100 GeV")
     legend3.SetTextSize(0.03)
     legend3.SetFillStyle(0)
     legend3.Draw("same")

  canvas.SaveAs("substructure_pas_"+names[plots.index(plot)]+"_"+str(runSet+100*theory)+".png")
  canvas.SaveAs("substructure_pas_"+names[plots.index(plot)]+"_"+str(runSet+100*theory)+".pdf")
  canvas.SaveAs("substructure_pas_"+names[plots.index(plot)]+"_"+str(runSet+100*theory)+".root")
  canvas.SaveAs("substructure_pas_"+names[plots.index(plot)]+"_"+str(runSet+100*theory)+".C")
  canvas.SaveAs("substructure_pas_"+names[plots.index(plot)]+"_"+str(runSet+100*theory)+".eps")
