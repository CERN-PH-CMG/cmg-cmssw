import os, sys
import array
from ROOT import * 
from os import path

#gROOT.Reset()
#gROOT.SetStyle("Plain")
gROOT.ProcessLine('.L tdrstyle.C')
gStyle.SetOptStat(0)
gStyle.SetOptFit(0)
gStyle.SetTitleOffset(1.3,"Y")
gStyle.SetPadLeftMargin(0.15)
gStyle.SetPadBottomMargin(0.15)
gStyle.SetPadTopMargin(0.08)
gStyle.SetPadRightMargin(0.08)
gStyle.SetMarkerSize(0.5)
gStyle.SetHistLineWidth(1)
#gStyle.SetStatFontSize(0.020)
gStyle.SetTitleSize(0.06, "XYZ")
gStyle.SetLabelSize(0.05, "XYZ")
gStyle.SetNdivisions(506, "XYZ")
gStyle.SetLegendBorderSize(0)

TGaxis.SetMaxDigits(3)

if __name__ == '__main__':

 scenario="_lowPU"
 #scenario=""

 samples = ["substructure_pas_QCD500.root",
             "substructure_pas_QCD1000.root",
             "substructure_pas_WWPy61000.root",
            ]

 selection="(abs(Jet1eta)<2.4)&&(deta<1.3)&&(DijetMass>890)&&(Jet1pt>400)&&(Jet1pt<600)"
 selection_mass="&&(Jet1Mass>60)&&(Jet1Mass<100)"
 plots=[("Jet1Nsub","#tau_{2}/#tau_{1}",-1,10,False),
        ("Jet1NsubPruned","pruned #tau_{2}/#tau_{1}",-1,10,False),
        ("Jet1MassDrop","mass drop",-1,10,False),
        ("Jet1C2beta17","C_{2} (#beta=1.7)",-1,10,False),
        ]

 if scenario=="_lowPU":
   plots=[("Jet1Nsub","#tau_{2}/#tau_{1} <PU>=22",-1,10,False),
        ("Jet1NsubPruned","pruned #tau_{2}/#tau_{1} <PU>=22",-1,10,False),
        ("Jet1MassDrop","mass drop <PU>=22",-1,10,False),
        ("Jet1C2beta17","C_{2} (#beta=1.7) <PU>=22",-1,10,False),
        ("Jet1Nsub","#tau_{2}/#tau_{1} <PU>=12",-1,10,False,"lowPU"),
        ("Jet1NsubPruned","pruned #tau_{2}/#tau_{1} <PU>=12",-1,10,False,"lowPU"),
        ("Jet1MassDrop","mass drop <PU>=12",-1,10,False,"lowPU"),
        ("Jet1C2beta17","C_{2} (#beta=1.7) <PU>=12",-1,10,False,"lowPU"),
        ]

 names = ["tau21",
	   "tau21pruned",
	   "massdrop",
	   "C2beta17",
	   "tau21_lowPU",
	   "tau21pruned_lowPU",
	   "massdrop_lowPU",
	   "C2beta17_lowPU",
	   ]

 colors=[1,2,4,6,1,2,4,6]
 widths=[2,2,2,2,1,1,1,1]
 styles=[1,2,3,4,1,2,3,4]

 results=[]

 canvas = TCanvas("","",0,0,200,200)
 canvas.SetLogy(False)
 mg = TMultiGraph()
 mg2 = TMultiGraph()
 legend=TLegend(0.2,0.2,0.85,0.5)

 for plot in plots:
  if "lowPU" in names[plots.index(plot)] and not "&&(nPU<17)" in selection:
      selection+="&&(nPU<17)"
  event_count=[]
  for sample in samples:
    f=TFile.Open(sample)
    tree=f.Get("dijetWtag")
    hist=TH1F("DijetMass","DijetMass",10000,0,10000);
    tree.Project("DijetMass","DijetMass",selection)
    before=hist.Integral()
    print "before mass cut",before
    histname="plot"+names[plots.index(plot)]
    hist=TH1F(histname,histname,10000,plot[2],plot[3]);
    tree.Project(histname,plot[0],selection+selection_mass)
    after=hist.Integral()
    print "after mass cut",after
    integral=[]
    for b in range(hist.GetNbinsX()):
      if plot[4]:
        integral+=[hist.Integral(b+1,hist.GetNbinsX())]
      else:
        integral+=[hist.Integral(0,b+1)]
    if "QCD1000" in sample:
       weight=204.0/13798133*30522161/8426.0
       event_count[-1][0]+=before*weight
       event_count[-1][1]+=after*weight
       for b in range(hist.GetNbinsX()):
           event_count[-1][2][b]+=integral[b]*weight
    else:
       event_count+=[[before,after,integral]]

  groc = TGraphErrors(1)
  groc.SetLineColor(colors[plots.index(plot)])
  groc.SetLineWidth(widths[plots.index(plot)])
  groc.SetLineStyle(styles[plots.index(plot)])
  for b in range(len(event_count[0][2])):
      groc.SetPoint(b, event_count[1][2][b]/event_count[1][0], 1.-event_count[0][2][b]/event_count[0][0])
  mg.Add(groc,"L")
  legend.AddEntry(groc,plot[1],"l")

  groc2 = TGraphErrors(1)
  groc2.SetLineColor(colors[plots.index(plot)])
  groc2.SetLineWidth(widths[plots.index(plot)])
  groc2.SetLineStyle(styles[plots.index(plot)])
  for b in range(len(event_count[0][2])):
      groc2.SetPoint(b, event_count[1][2][b]/event_count[1][1], 1.-event_count[0][2][b]/event_count[0][1])
  mg2.Add(groc2,"L")

 mg.SetTitle("")
 mg.Draw("AP")
 mg.GetXaxis().SetTitle("#epsilon_{sig}")
 mg.GetYaxis().SetTitle("1 - #epsilon_{bkg}")
 mg.GetXaxis().SetRangeUser(0,1)
 mg.GetYaxis().SetRangeUser(0.9,1)

 legend.SetTextSize(0.036)
 legend.SetFillStyle(0)
 legend.Draw("same")

 legend4=TLegend(0.73,0.85,0.9,0.9,"CA R=0.8")
 legend4.SetTextSize(0.03)
 legend4.SetFillStyle(0)
 legend4.Draw("same")

 legend2=TLegend(0.65,0.8,0.9,0.85,"400 < p_{T} < 600 GeV")
 legend2.SetTextSize(0.03)
 legend2.SetFillStyle(0)
 legend2.Draw("same")

 legend2a=TLegend(0.73,0.75,0.9,0.8,"|#eta|<2.4")
 legend2a.SetTextSize(0.03)
 legend2a.SetFillStyle(0)
 legend2a.Draw("same")

 banner = TLatex(0.3,0.93,"CMS Simulation, #sqrt{s} = 8 TeV, dijets");
 banner.SetNDC()
 banner.SetTextSize(0.04)
 banner.Draw();  

 canvas.SaveAs("substructure_pas_roc"+scenario+".png")
 canvas.SaveAs("substructure_pas_roc"+scenario+".pdf")
 canvas.SaveAs("substructure_pas_roc"+scenario+".root")
 canvas.SaveAs("substructure_pas_roc"+scenario+".C")
 canvas.SaveAs("substructure_pas_roc"+scenario+".eps")

 canvas = TCanvas("","",0,0,200,200)
 canvas.SetLogy(False)
 mg2.SetTitle("")
 mg2.Draw("AP")
 mg2.GetXaxis().SetTitle("#epsilon_{sig}")
 mg2.GetYaxis().SetTitle("1 - #epsilon_{bkg}")
 mg2.GetXaxis().SetRangeUser(0,1)
 mg2.GetYaxis().SetRangeUser(0,1)
 legend.Draw("same")
 legend4.Draw("same")
 legend2.Draw("same")
 legend2a.Draw("same")
 banner.Draw();  

 canvas.SaveAs("substructure_pas_roc2"+scenario+".png")
 canvas.SaveAs("substructure_pas_roc2"+scenario+".pdf")
 canvas.SaveAs("substructure_pas_roc2"+scenario+".root")
 canvas.SaveAs("substructure_pas_roc2"+scenario+".C")
 canvas.SaveAs("substructure_pas_roc2"+scenario+".eps")
