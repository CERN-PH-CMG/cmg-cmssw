#Creates and saves as PDF the histograms for the signal jet mass peak shift study. It works the same way as histo.py, except that you must select only the "merged" plot, which contain the values from the two highest pt jets.

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
gStyle.SetLabelSize(0.03, "XYZ")
gStyle.SetNdivisions(510, "XYZ")
gStyle.SetLegendBorderSize(0)




samples = [("dijetWtag_Moriond_HHPy61000.root","M(Rad)=1 TeV","signal1000",0),       
	  ("dijetWtag_Moriond_HHPy61500.root","M(Rad)=1.5 TeV","signal1500",1),     
	  ("dijetWtag_Moriond_HHPy62000.root","M(Rad)=2 TeV","signal2000",0),       
	  ("dijetWtag_Moriond_HHPy62500.root","M(Rad)=2.5 TeV","signal2500",0),     
	  ("dijetWtag_Moriond_HHPy63000.root","M(Rad)=3 TeV","signal3000",0)]

select = "(deta < 1.3) & (categories < 1) & (categories > -1)"

axis=[1, 0, 650] # 0 (no) or 1 (yes) in position 0, ymin in position 1 & ymax in position 2.
# A vertical line is created at the Higgs mass. It will begin at 0 and end at ymax.

stack = [1, 0, "; Jet mass /GeV", 0.2, 0.7, 0.9, 0.9] # creates an additionnal canvas with all the selected plots drawn on it.
#Format : linear, log, title, legend coordinates

plots = [(0, 0, "genjetmass1", "; gen jet 1 mass/GeV", 100, 160, "", 0.55, 0.4, 0.95, 0.9, 0),
	 (0, 0, "genjetmass2", "; gen jet 2 mass/GeV", 100, 160, "", 0.55, 0.4, 0.95, 0.9, 0),
	 (0, 0, "genjetmass", "; gen jet mass/GeV", 65, 170, "merged", 0.55, 0.4, 0.95, 0.9, 0, "gen jets"),
         (0, 0, "prunedgenjetmass1", "; pruned gen jet 1 mass/GeV", 100, 160, "", 0.45, 0.4, 0.95, 0.9, 0),
	 (0, 0, "prunedgenjetmass2", "; pruned gen jet 2 mass/GeV", 100, 160, "", 0.45, 0.4, 0.95, 0.9, 0),
	 (1, 0, "prunedgenjetmass", "; pruned gen jet mass/GeV", 90, 140, "merged", 0.45, 0.4, 0.95, 0.9, 0, "pruned gen jets, MET/ET > 0.01"),
	 (0, 0, "dmgentoprunedgen1", "; #DeltaM1/Mgen1", 0, 1, "", 0.45, 0.4, 0.95, 0.9, 0),
	 (0, 0, "dmgentoprunedgen2", "; #DeltaM2/Mgen2", 0, 1, "", 0.45, 0.4, 0.95, 0.9, 0),
	 (0, 0, "dmgentoprunedgen", "; #DeltaM2/Mgen", -0.5, 1.5, "merged", 0.45, 0.4, 0.95, 0.9, 0),
	 (0, 0, "dpgentoprunedgen1", "; #DeltaPt1/Ptgen1", 0, 1, "", 0.45, 0.4, 0.95, 0.9, 0),
	 (0, 0, "dpgentoprunedgen2", "; #DeltaPt2/Ptgen2", 0, 1, "", 0.45, 0.4, 0.95, 0.9, 0),
	 (0, 0, "dpgentoprunedgen", "; #DeltaM2/Mgen", -0.2, 0.2, "merged", 0.45, 0.4, 0.95, 0.9, 0),
	 (0, 0, "drgentoprunedgen1", "; #DeltaR1", 0, 1, "", 0.45, 0.4, 0.95, 0.9, 0),
	 (0, 0, "drgentoprunedgen2", "; #DeltaR2", 0, 1, "", 0.45, 0.4, 0.95, 0.9, 0),
	 (0, 0, "drgentoprunedgen", "; #DeltaR", 0, 1, "merged", 0.45, 0.4, 0.95, 0.9, 0),
	 (0, 0, "recojetmass1", "; reco jet 1 mass/GeV", 100, 160, "", 0.45, 0.4, 0.95, 0.9, 0),
	 (0, 0, "recojetmass2", "; reco jet 2 mass/GeV", 100, 160, "", 0.45, 0.4, 0.95, 0.9, 0),
	 (0, 0, "recojetmass", "; reco jet mass/GeV", 65, 160, "merged", 0.45, 0.4, 0.95, 0.9, 0, "reco jets"),
	 (0, 0, "dmgentoreco1", "; #DeltaM1/Mgen1", 0, 1, "", 0.45, 0.4, 0.95, 0.9, 0),
	 (0, 0, "dmgentoreco2", "; #DeltaM2/Mgen2", 0, 1, "", 0.45, 0.4, 0.95, 0.9, 0),
	 (0, 0, "dmgentoreco", "; #DeltaM/Mgen", -1, 3, "merged", 0.45, 0.4, 0.95, 0.9, 0),
	 (0, 0, "dpgentoreco1", "; #DeltaPt1/Ptgen1", 0, 1, "", 0.45, 0.4, 0.95, 0.9, 0),
	 (0, 0, "dpgentoreco2", "; #DeltaPt2/Ptgen2", 0, 1, "", 0.45, 0.4, 0.95, 0.9, 0),
	 (0, 0, "dpgentoreco", "; #DeltaPt/Ptgen", -75, 75, "merged", 0.45, 0.4, 0.95, 0.9, 0),
	 (0, 0, "drgentoreco1", "; #DeltaR1", 0, 1, "", 0.45, 0.4, 0.95, 0.9, 0),
	 (0, 0, "drgentoreco2", "; #DeltaR2", 0, 1, "", 0.45, 0.4, 0.95, 0.9, 0),
	 (0, 0, "drgentoreco", "; #DeltaR", 0, 0.5, "merged", 0.45, 0.4, 0.95, 0.9, 0),
	 (0, 0, "prunedrecojetmass1", "; pruned reco jet 1 mass/GeV", 100, 160, "", 0.45, 0.4, 0.95, 0.9, 0),
	 (0, 0, "prunedrecojetmass2", "; pruned reco jet 2 mass/GeV", 100, 160, "", 0.45, 0.4, 0.95, 0.9, 0),
	 (1, 0, "prunedrecojetmass", "; pruned reco jet mass/GeV", 60, 160, "merged", 0.19, 0.65, 0.95, 0.9, 1, "pruned reco jets, MET/ET > 0.01"),
	 (0, 0, "dmrecotoprunedreco1", "; #DeltaM1/Mreco1", 0, 1, "", 0.45, 0.4, 0.95, 0.9, 0),
	 (0, 0, "dmrecotoprunedreco2", "; #DeltaM2/Mreco2", 0, 1, "", 0.45, 0.4, 0.95, 0.9, 0),
	 (0, 0, "dmrecotoprunedreco", "; #DeltaM2/Mreco", -0.5, 1.5, "merged", 0.45, 0.4, 0.95, 0.9, 0),
	 (0, 0, "dprecotoprunedreco1", "; #DeltaP1/Preco1", 0, 1, "", 0.45, 0.4, 0.95, 0.9, 0),
	 (0, 0, "dprecotoprunedreco2", "; #DeltaP2/Preco2", 0, 1, "", 0.45, 0.4, 0.95, 0.9, 0),
	 (0, 0, "dprecotoprunedreco", "; #DeltaP2/Preco", -0.5, 1.5, "merged", 0.45, 0.4, 0.95, 0.9, 0),
	 (0, 0, "drrecotoprunedreco1", "; #DeltaR1", 0, 1, "", 0.45, 0.4, 0.95, 0.9, 0),
	 (0, 0, "drrecotoprunedreco2", "; #DeltaR2", 0, 1, "", 0.45, 0.4, 0.95, 0.9, 0),
	 (0, 0, "drrecotoprunedreco", "; #DeltaR", 0, 1, "merged", 0.45, 0.4, 0.95, 0.9, 0),
	 (0, 0, "dmprunedgentoprunedreco1", "; #DeltaM1/Mprunedgen1", 0, 1, "", 0.45, 0.4, 0.95, 0.9, 0),
	 (0, 0, "dmprunedgentoprunedreco2", "; #DeltaM2/Mprunedgen2", 0, 1, "", 0.45, 0.4, 0.95, 0.9, 0),
	 (0, 0, "dmprunedgentoprunedreco", "; M_{pruned gen} - M_{pruned reco}", -20, 20, "merged", 0.45, 0.4, 0.95, 0.9, 0, "jets"),
	 (0, 0, "dpprunedgentoprunedreco1", "; #DeltaPt1/Ptprunedgen1", 0, 1, "", 0.45, 0.4, 0.95, 0.9, 0),
	 (0, 0, "dpprunedgentoprunedreco2", "; #DeltaPt2/Ptprunedgen2", 0, 1, "", 0.45, 0.4, 0.95, 0.9, 0),
	 (0, 0, "dpprunedgentoprunedreco", "; #DeltaPt/Ptprunedgen", -90, 90, "merged", 0.45, 0.4, 0.95, 0.9, 0, "jets"),
	 (0, 0, "drprunedgentoprunedreco1", "; #DeltaR1", 0, 1, "", 0.45, 0.4, 0.95, 0.9, 0),
	 (0, 0, "drprunedgentoprunedreco2", "; #DeltaR2", 0, 1, "", 0.45, 0.4, 0.95, 0.9, 0),
	 (0, 0, "drprunedgentoprunedreco", "; #DeltaR", 0, 0.5, "merged", 0.45, 0.4, 0.95, 0.9, 0)]
       

names = ["histo_genjetmass1",
	 "histo_genjetmass2",
	 "histo_genjetmass",
         "histo_prunedgenjetmass1",
	 "histo_prunedgenjetmass2",
	 "histo_prunedgenjetmass",
	 "histo_dmgentoprunedgen1",
	 "histo_dmgentoprunedgen2",
	 "histo_dmgentoprunedgen",
	 "histo_dpgentoprunedgen1",
	 "histo_dpgentoprunedgen2",
	 "histo_dpgentoprunedgen",
	 "histo_drgentoprunedgen1",
	 "histo_drgentoprunedgen2",
	 "histo_drgentoprunedgen",
	 "histo_recojetmass1",
	 "histo_recojetmass2",
	 "histo_recojetmass",
	 "histo_dmgentoreco1",
	 "histo_dmgentoreco2",
	 "histo_dmgentoreco",
	 "histo_dpgentoreco1",
	 "histo_dpgentoreco2",
	 "histo_dpgentoreco",
	 "histo_drgentoreco1",
	 "histo_drgentoreco2",
	 "histo_drgentoreco",
	 "histo_prunedrecojetmass1",
	 "histo_prunedrecojetmass2",
	 "histo_prunedrecojetmass",
	 "histo_dmrecotoprunedreco1",
	 "histo_dmrecotoprunedreco2",
	 "histo_dmrecotoprunedreco",
	 "histo_dprecotoprunedreco1",
	 "histo_dprecotoprunedreco2",
	 "histo_dprecotoprunedreco",
	 "histo_drrecotoprunedreco1",
	 "histo_drrecotoprunedreco2",
	 "histo_drrecotoprunedreco",
	 "histo_dmprunedgentoprunedreco1",
	 "histo_dmprunedgentoprunedreco2",
	 "histo_dmprunedgentoprunedreco",
	 "histo_dpprunedgentoprunedreco1",
	 "histo_dpprunedgentoprunedreco2",
	 "histo_dpprunedgentoprunedreco",
	 "histo_drprunedgentoprunedreco1",
	 "histo_drprunedgentoprunedreco2",
	 "histo_drprunedgentoprunedreco"]

nb=50

colors=[8,9,50,28,6,2,4,3] #The colors for the stacked histogram.

prefex="/afs/cern.ch/work/s/sthebaud/private/rootfiles/"









ltrees=[]
for sample in samples:
	if sample[3]==1:
		f=TFile.Open(prefex+sample[0])
		tree=f.Get("missingmass")
		ltrees += [(sample[2],tree)]
	if sample[3]==0:
		ltrees += [("nope","nothing")]

stacklhists=[]

for plot in plots:
	if (plot[0]==1 or plot[1]==1):
		lhists=[]
		canvas = TCanvas("","",0,0,300,300)
		legend=TLegend(plot[7], plot[8], plot[9], plot[10], "CMS Preliminary (L=19.6 /fb)")
		legend.SetTextSize(0.04)
		legend.SetFillStyle(0)
                l=TLine(125.3, 0, 125.3, axis[2])
                l.SetLineColor(1)

                for ltree in ltrees:
			i=-1
			if ltree[0]=="signal1000":
                            i=0
			if ltree[0]=="signal1500":
                            i=1
			if ltree[0]=="signal2000":
                            i=2
			if ltree[0]=="signal2500":
                            i=3
			if ltree[0]=="signal3000":
                            i=4
			if i>-1:
                            hist=TH1F(samples[i][0],plot[3],nb,plot[4],plot[5])
			    if plot[6]=="":
				    ltree[1].Project(samples[i][0],plot[2],select)
			    if plot[6]=="merged":
				    hist1=TH1F(samples[i][0]+"1",plot[3],nb,plot[4],plot[5])
				    hist2=TH1F(samples[i][0]+"2",plot[3],nb,plot[4],plot[5])
				    ltree[1].Project(samples[i][0]+"1",plot[2]+"1",select)
				    ltree[1].Project(samples[i][0]+"2",plot[2]+"2",select)
				    hist.Add(hist1,hist2,1,1)
			    lhists += [(ltree[0],hist)]
			if i==-1:
                            lhists += [("nope","nothing")]



                for lhist in lhists:
			    if lhist[0]=="signal1000":
				    lhist[1].SetLineWidth(2)
				    lhist[1].SetLineStyle(1)
				    if (stack[0] or stack[1]==1) == 1:
					    stacklhists += [(plot[2]+"1000",lhist[1])]
				    lhist[1].SetLineColor(50)	
				    legend.AddEntry(lhist[1],samples[0][1])
                                    if axis[0]==1:
                                        lhist[1].SetMaximum(axis[2])
                                        lhist[1].SetMinimum(axis[1])
                                    lhist[1].Draw()
			    if lhist[0]=="signal1500":
				    lhist[1].SetLineWidth(2)
				    lhist[1].SetLineStyle(1)
				    if (stack[0] == 1 or stack[1]==1):
					    stacklhists += [(plot[2]+"1500",lhist[1])]
				    lhist[1].SetLineColor(28)	
				    legend.AddEntry(lhist[1],samples[1][1])
				    if (lhists[0][0]=="nope"):
					    if axis[0]==1:
						    lhist[1].SetMaximum(axis[2])
						    lhist[1].SetMinimum(axis[1])
					    lhist[1].Draw()
				    else:
					    lhist[1].Draw("same")
			    if lhist[0]=="signal2000":
				    lhist[1].SetLineWidth(2)
				    lhist[1].SetLineStyle(1)
				    lhist[1].Scale(1)
				    if (stack[0] == 1 or stack[1]==1):
					    stacklhists += [(plot[2]+"2000",lhist[1])]
				    lhist[1].SetLineColor(3)
				    legend.AddEntry(lhist[1],samples[2][1])
				    if ((lhists[0][0]=="nope") and (lhists[1][0]=="nope")):
					    if axis[0]==1:
						    lhist[1].SetMaximum(axis[2])
						    lhist[1].SetMinimum(axis[1])
					    lhist[1].Draw()
				    else:
					    lhist[1].Draw("same")
			    if lhist[0]=="signal2500":
				    lhist[1].SetLineWidth(2)
				    lhist[1].SetLineStyle(1)
				    if (stack[0] == 1 or stack[1]==1):
					    stacklhists += [(plot[2]+"2500",lhist[1])]
				    lhist[1].SetLineColor(2)				 	
				    legend.AddEntry(lhist[1],samples[3][1])
				    if ((lhists[0][0]=="nope") and (lhists[1][0]=="nope") and (lhists[2][0]=="nope")):
					    if axis[0]==1:
						    lhist[1].SetMaximum(axis[2])
						    lhist[1].SetMinimum(axis[1])
					    lhist[1].Draw()
				    else:
					    lhist[1].Draw("same")
			    if lhist[0]=="signal3000":
				    lhist[1].SetLineWidth(2)
				    lhist[1].SetLineStyle(1)
				    if (stack[0] == 1 or stack[1]==1):
					    stacklhists += [(plot[2]+"3000",lhist[1])]
				    lhist[1].SetLineColor(4)	
				    legend.AddEntry(lhist[1],samples[4][1])
				    if ((lhists[0][0]=="nope") and (lhists[1][0]=="nope") and (lhists[2][0]=="nope") and (lhists[3][0]=="nope")):
					    if axis[0]==1:
						    lhist[1].SetMaximum(axis[2])
						    lhist[1].SetMinimum(axis[1])
					    lhist[1].Draw()
				    else:
					    lhist[1].Draw("same")

                #legend.AddEntry("","#sqrt{s} = 8 TeV","")
                l.Draw()
                if plot[11]==1:
                    legend.Draw("same")
                if plot[0]==1:
                    canvas.SaveAs("histograms/"+names[plots.index(plot)]+".jpg")
                    canvas.SaveAs("histograms/"+names[plots.index(plot)]+".pdf")
                    canvas.SaveAs("histograms/"+names[plots.index(plot)]+".eps")
                    canvas.SaveAs("histograms/"+names[plots.index(plot)]+".root")
                    """canvas.SaveAs("histograms/"+names[plots.index(plot)]+".C")"""
                if plot[1]==1:
                    canvas.SetLogy(1)
                    canvas.SaveAs("histograms/log"+names[plots.index(plot)]+".jpg")
                    canvas.SaveAs("histograms/log"+names[plots.index(plot)]+".pdf")
                    canvas.SaveAs("histograms/log"+names[plots.index(plot)]+".eps")
                    canvas.SaveAs("histograms/log"+names[plots.index(plot)]+".root")


if (stack[0]==1 or stack[1]==1):
	stackcanvas = TCanvas("","",0,0,300,300)
	stacklegend=TLegend(stack[3], stack[4], stack[5], stack[6], "CMS Preliminary (L=19.6 /fb)")
	stacklegend.SetTextSize(0.04)
	stacklegend.SetFillStyle(0)
	i=0
	for lhist in stacklhists:
		lhist[1].SetLineColor(colors[i])
		lhist[1].SetLineStyle(1)
		mean=lhist[1].GetMean()
		stacklegend.AddEntry(lhist[1],lhist[0])
		stacklegend.AddEntry("","Mean = "+str(round(mean,1)),"")
		if i==0:
			if axis[0]==1:
				lhist[1].SetMaximum(axis[2])
				lhist[1].SetMinimum(axis[1])
			lhist[1].SetTitle(stack[2])
			lhist[1].Draw()
		if i>0:
			lhist[1].Draw("same")
		i=i+1

	stacklegend.Draw("same")
	l.Draw("same")
	if stack[0]==1:
		stackcanvas.SaveAs("histograms/histostack.jpg")
		stackcanvas.SaveAs("histograms/histostack.pdf")
		stackcanvas.SaveAs("histograms/histostack.eps")
		stackcanvas.SaveAs("histograms/histostack.root")
	if stack[1]==1:
		stackcanvas.SetLogy(1)
		stackcanvas.SaveAs("histograms/loghistostack.jpg")
		stackcanvas.SaveAs("histograms/loghistostack.pdf")
		stackcanvas.SaveAs("histograms/loghistostack.eps")
		stackcanvas.SaveAs("histograms/loghistostack.root")
	
