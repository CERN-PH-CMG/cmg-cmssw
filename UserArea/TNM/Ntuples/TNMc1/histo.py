#Creates and saves in PDF & ROOT formats histograms for the data, QCD MC & signals sample files. Set the parameters and inputs, then run the program.

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




# Parameters and inputs:


samples = [("dijetWtag_Moriond_QCD500.root","QCD bg Ht=500-1000","QCD500",1),       # The QCD backgrounds first, 
          ("dijetWtag_Moriond_QCD1000.root","QCD bg Ht 1000-#infty","QCD1000",1),   #
          ("dijetWtag_Moriond_Run2012.root","Exp data","data",1),                   # then the data, 
          ("dijetWtag_Moriond_HHPy61000.root","M(Rad)=1 TeV","signal1000",0),       #
	  ("dijetWtag_Moriond_HHPy61500.root","M(Rad)=1.5 TeV","signal1500",1),     #
	  ("dijetWtag_Moriond_HHPy62000.root","M(Rad)=2 TeV","signal2000",0),       # and then the signals.
	  ("dijetWtag_Moriond_HHPy62500.root","M(Rad)=2.5 TeV","signal2500",0),     #
	  ("dijetWtag_Moriond_HHPy63000.root","M(Rad)=3 TeV","signal3000",0)]       # Set 1 to include, 0 otherwise.

prefex="/afs/cern.ch/work/s/sthebaud/private/rootfiles/"


division = 0   # if 1, only N(data)/N(QCD) will be plotted. This requires the files QCD500, QCD1000 and data to be included. Else, set 0.


select = "deta<1.3 & categories<7 & categories >-2"   # The drawn histograms will only take the event satisfying the "select" condition.

axis=[0, 1000000, 10000000] # to define the Y axis manually, put 1 in first position, ymin in second and ymax in third.

plots = [( 0, 1, "DijetMassCA8", "; M_{jj}/GeV", 890, 3500, "QCDmerge y", 0.45, 0.61, 0.9, 0.9, 1),
         ( 0, 0, "deta", "; #Delta#eta", 0, 5, "QCDmerge y", 0.4, 0.4, 0.9, 0.9, 0),
	 ( 1, 0, "dphi", "; #Delta#phi", 1, 5, "QCDmerge y", 0.4, 0.4, 0.9, 0.9, 0),
	 ( 0, 0, "met", "; MET/#sum E_{T}", 0, 1, "QCDmerge y", 0.4, 0.4, 0.9, 0.9, 0),
         ( 0, 0, "Jet1pt", "; jet 1 P_{T}/Gev", 200, 1500, "QCDmerge y", 0.4, 0.4, 0.9, 0.9, 0),
         ( 0, 0, "Jet2pt", "; jet 2 P_{T}/Gev", 200, 2000, "QCDmerge y", 0.4, 0.4, 0.9, 0.9, 0),
         ( 0, 0, "Jet1eta", "; jet 1 #eta", -3, 3, "QCDmerge y", 0.4, 0.4, 0.9, 0.9, 0),
         ( 0, 0, "Jet2eta", "; jet 2 #eta", -3, 3, "QCDmerge y", 0.4, 0.4, 0.9, 0.9, 0),
         ( 0, 0, "Jet1CA8Mass", "; jet 1 mass/GeV", 105, 145, "QCDmerge y", 0.45, 0.4, 0.95, 0.9, 1),
         ( 0, 0, "Jet2CA8Mass", "; jet 2 mass/GeV", 0, 300, "QCDmerge y", 0.4, 0.4, 0.9, 0.9, 0),
         ( 0, 0, "Jet1CA8Nsub", "; jet 1 #tau_{12}", 0, 1, "QCDmerge y", 0.2, 0.4, 0.7, 0.9, 0),
	 ( 0, 0, "Jet2CA8Nsub", "; jet 2 #tau_{12}", 0, 1, "QCDmerge y", 0.2, 0.4, 0.7, 0.9, 0),
	 ( 0, 0, "Jet1dr", "; jet 1 dR", 0, 2, "QCDmerge y", 0.2, 0.4, 0.7, 0.9, 1),
	 ( 0, 0, "Jet2dr", "; jet 2 dR", 0, 2, "QCDmerge y", 0.2, 0.4, 0.7, 0.9, 1),
	 ( 0, 0, "nbvert", "; Number of primary vertices", 0, 50, "QCDmerge y", 0.48, 0.6, 0.9, 0.9, 1),
	 ( 0, 0, "Jet1mef", "; jet 1 muon energy fraction", 0, 1, "QCDmerge y", 0.4, 0.4, 0.9, 0.9, 0),
	 ( 0, 0, "Jet2mef", "; jet 2 muon energy fraction", 0, 1, "QCDmerge y", 0.4, 0.4, 0.9, 0.9, 0),
	 ( 0, 0, "Jet1nhef", "; jet 1 neutral hadron energy fraction", 0, 1, "QCDmerge y", 0.4, 0.4, 0.9, 0.9, 0),
	 ( 0, 0, "Jet2nhef", "; jet 2 neutral hadron energy fraction", 0, 1, "QCDmerge y", 0.4, 0.4, 0.9, 0.9, 0),
	 ( 0, 0, "Jet1neef", "; jet 1 neutral EM energy fraction", 0, 1, "QCDmerge y", 0.4, 0.4, 0.9, 0.9, 0),
	 ( 0, 0, "Jet2neef", "; jet 2 neutral EM energy fraction", 0, 1, "QCDmerge y", 0.4, 0.4, 0.9, 0.9, 0),
	 ( 0, 0, "Jet1nbcons", "; jet 1 number of constituants", 0, 400, "QCDmerge y", 0.4, 0.4, 0.9, 0.9, 0),
	 ( 0, 0, "Jet2nbcons", "; jet 2 number of constituants", 0, 400, "QCDmerge y", 0.4, 0.4, 0.9, 0.9, 0),
	 ( 0, 0, "Jet1chef", "; jet 1 charged hadron energy fraction", 0, 1, "QCDmerge y", 0.4, 0.4, 0.9, 0.9, 0),
	 ( 0, 0, "Jet2chef", "; jet 2 charged hadron energy fraction", 0, 1, "QCDmerge y", 0.4, 0.4, 0.9, 0.9, 0),
	 ( 0, 0, "Jet1ceef", "; jet 1 charged EM energy fraction", 0, 1, "QCDmerge y", 0.4, 0.4, 0.9, 0.9, 0),          
	 ( 0, 0, "Jet2ceef", "; jet 2 charged EM energy fraction", 0, 1, "QCDmerge y", 0.4, 0.4, 0.9, 0.9, 0),         
	 ( 0, 0, "Jet1cmult", "; jet 1 charge multiplicity; Events", 0, 200, "QCDmerge y", 0.4, 0.4, 0.9, 0.9, 0),
	 ( 0, 0, "Jet2cmult", "; jet 2 charge multiplicity; Events", 0, 200, "QCDmerge y", 0.4, 0.4, 0.9, 0.9, 0)]

#format: linear scale? (0=n, 1=y), log scale? (ditto), "variable name in the tree", "Title; X axis name; Y axis name", Xmin, Xmax, merging the 2 QCD backgrounds?, legend coordinates, legend? (0=n, 1=y)


names = ["histo_dijetmass",       #name of the produced histograms
         "histo_deta",
	 "histo_dphi",
	 "histo_met",
         "histo_pt1",
         "histo_pt2",
         "histo_eta1",
         "histo_eta2",
         "histo_mass1",
         "histo_mass2",
         "histo_nsub1",
         "histo_nsub2",
	 "histo_dr1",
	 "histo_dr2",
	 "histo_nbvert",
	 "histo_mef1",
	 "histo_mef2",
	 "histo_nhef1",
	 "histo_nhef2",
	 "histo_neef1",
	 "histo_neef2",
	 "histo_nbcons1",
	 "histo_nbcons2",
	 "histo_chef1",
	 "histo_chef2",
	 "histo_ceef1",
	 "histo_ceef2",
	 "histo_cmult1",
	 "histo_cmult2"]

    

x=0.3                                # x: normalization of the signal 
r=18.67393705                        # r: normalization of QCD 500 with regard to QCD 1000
nb=50                                # nb: number of bins. 





# Set the desired parameters and run the program. The code that follows shouldn't be changed if everything works fine.





ltrees=[]
for sample in samples:      #creates the list of the trees from the included source files.
	if sample[3]==1:
		f=TFile.Open(prefex+sample[0])
		tree=f.Get("dijetWtag")
		ltrees += [(sample[2],tree)]
	if sample[3]==0:
		ltrees += [("nope","nothing")]
	

for plot in plots:           # Loop over the selected variables to be drawn
	if (plot[0]==1 or plot[1]==1):
		lhists=[]
		s=THStack("s",plot[3])
		canvas = TCanvas("","",0,0,300,300)
		legend=TLegend(plot[7], plot[8], plot[9], plot[10], "CMS Preliminary (L=19.6 /fb)")
		legend.SetTextSize(0.04)
		legend.SetFillStyle(0)

		for ltree in ltrees:         #creates the list of the histograms for the variable "plot", from the included source files.
			i=-1
			if ltree[0]=="QCD500":
				i=0
			if ltree[0]=="QCD1000":
				i=1
			if ltree[0]=="data":
				i=2
			if ltree[0]=="signal1000":
				i=3
			if ltree[0]=="signal1500":
				i=4
			if ltree[0]=="signal2000":
				i=5
			if ltree[0]=="signal2500":
				i=6
			if ltree[0]=="signal3000":
				i=7
			if i>-1:
				hist=TH1F(samples[i][2],plot[3],nb,plot[4],plot[5])
				ltree[1].Project(samples[i][2],plot[2],select)
				lhists += [(ltree[0],hist)]
			if i==-1:
				lhists += [("nope","nothing")]

		integral=0
		qcdint0=0
		qcdint1=0
		if lhists[2][0]=="data":
			integral=lhists[2][1].Integral()             # the experimental data is the reference for normalization.
		if lhists[0][0]=="QCD500":
			lhists[0][1].Scale(r)
			qcdint0=lhists[0][1].Integral()
		if lhists[1][0]=="QCD1000":
			qcdint1=lhists[1][1].Integral()
		qcdint = qcdint0 + qcdint1
		if division==0:
			for lhist in lhists:                 # Loops over the histograms, parametrizes and draws them.
			    if lhist[0]=="QCD500":
				    lhist[1].SetLineWidth(0)
				    if plot[6]=="QCDmerge y":
					    lhist[1].SetFillColor(7)       
					    lhist[1].SetLineColor(7)
				    if plot[6]=="QCDmerge n":
					    lhist[1].SetFillColor(kOrange)       
					    lhist[1].SetLineColor(kOrange)
					    legend.AddEntry(lhist[1],samples[0][1],"f")
				    if integral>0:
					    lhist[1].Scale(integral/qcdint)
				    s.Add(lhist[1])
				    if lhists[1][0]=="nope":
					    if axis[0]==1:
						    s.SetMaximum(axis[2])
						    s.SetMinimum(axis[1])
					    s.Draw()
			    if lhist[0]=="QCD1000":
				    lhist[1].SetLineWidth(0)             
				    lhist[1].SetFillColor(7)       
				    lhist[1].SetLineColor(7)
				    if integral>0:
					    lhist[1].Scale(integral/qcdint)
				    if plot[6]=="QCDmerge y":
					    legend.AddEntry(lhist[1],"QCD background","f")
				    if plot[6]=="QCDmerge n":
					    legend.AddEntry(lhist[1],samples[1][1],"f")
				    s.Add(lhist[1])
				    if axis[0]==1:
					    s.SetMaximum(axis[2])
					    s.SetMinimum(axis[1])
				    s.Draw()
			    if lhist[0]=="data":
				    lhist[1].SetLineWidth(2)
				    lhist[1].SetLineStyle(1)
				    lhist[1].SetMarkerStyle(20)
				    lhist[1].SetMarkerSize(0.35)
				    lhist[1].SetLineColor(1)
				    legend.AddEntry(lhist[1],samples[2][1])
				    if ((lhists[0][0]=="nope") and (lhists[1][0]=="nope")):
					    if axis[0]==1:
						    lhist[1].SetMaximum(axis[2])
						    lhist[1].SetMinimum(axis[1])
					    lhist[1].Draw("p,e")
				    else:
					    lhist[1].Draw("same,p,e")
			    if lhist[0]=="signal1000":
				    lhist[1].SetLineWidth(2)
				    lhist[1].SetLineStyle(2)
				    lhist[1].SetLineColor(50)
				    if integral>0:
					    lhist[1].Scale(x*integral/lhist[1].Integral())
				    else:
					    if qcdint>0:
						    lhist[1].Scale(x*qcdint/lhist[1].Integral())	
				    legend.AddEntry(lhist[1],samples[3][1])
				    if ((lhists[0][0]=="nope") and (lhists[1][0]=="nope") and (lhists[2][0]=="nope")):
					    if axis[0]==1:
						    lhist[1].SetMaximum(axis[2])
						    lhist[1].SetMinimum(axis[1])
					    lhist[1].Draw()
				    else:
					    lhist[1].Draw("same")
			    if lhist[0]=="signal1500":
				    lhist[1].SetLineWidth(2)
				    lhist[1].SetLineStyle(2)
				    lhist[1].SetLineColor(28)
				    if integral>0:
					    lhist[1].Scale(x*integral/lhist[1].Integral())
				    else:
					    if qcdint>0:
						    lhist[1].Scale(x*qcdint/lhist[1].Integral())	
				    legend.AddEntry(lhist[1],samples[4][1])
				    if ((lhists[0][0]=="nope") and (lhists[1][0]=="nope") and (lhists[2][0]=="nope") and (lhists[3][0]=="nope")):
					    if axis[0]==1:
						    lhist[1].SetMaximum(axis[2])
						    lhist[1].SetMinimum(axis[1])
					    lhist[1].Draw()
				    else:
					    lhist[1].Draw("same")
			    if lhist[0]=="signal2000":
				    lhist[1].SetLineWidth(2)
				    lhist[1].SetLineStyle(2)
				    lhist[1].SetLineColor(3)
				    if integral>0:
					    lhist[1].Scale(x*integral/lhist[1].Integral())
				    else:
					    if qcdint>0:
						    lhist[1].Scale(x*qcdint/lhist[1].Integral())	
				    legend.AddEntry(lhist[1],samples[5][1])
				    if ((lhists[0][0]=="nope") and (lhists[1][0]=="nope") and (lhists[2][0]=="nope") and (lhists[3][0]=="nope") and (lhists[4][0]=="nope")):
					    if axis[0]==1:
						    lhist[1].SetMaximum(axis[2])
						    lhist[1].SetMinimum(axis[1])
					    lhist[1].Draw()
				    else:
					    lhist[1].Draw("same")
			    if lhist[0]=="signal2500":
				    lhist[1].SetLineWidth(2)
				    lhist[1].SetLineStyle(2)
				    lhist[1].SetLineColor(2)
				    if integral>0:
					    lhist[1].Scale(x*integral/lhist[1].Integral())
				    else:
					    if qcdint>0:
						    lhist[1].Scale(x*qcdint/lhist[1].Integral())	
				    legend.AddEntry(lhist[1],samples[6][1])
				    if ((lhists[0][0]=="nope") and (lhists[1][0]=="nope") and (lhists[2][0]=="nope") and (lhists[3][0]=="nope") and (lhists[4][0]=="nope") and (lhists[5][0]=="nope")):
					    if axis[0]==1:
						    lhist[1].SetMaximum(axis[2])
						    lhist[1].SetMinimum(axis[1])
					    lhist[1].Draw()
				    else:
					    lhist[1].Draw("same")
			    if lhist[0]=="signal3000":
				    lhist[1].SetLineWidth(2)
				    lhist[1].SetLineStyle(2)
				    lhist[1].SetLineColor(4)
				    if integral>0:
					    lhist[1].Scale(x*integral/lhist[1].Integral())
				    else:
					    if qcdint>0:
						    lhist[1].Scale(x*qcdint/lhist[1].Integral())	
				    legend.AddEntry(lhist[1],samples[7][1])
				    if ((lhists[0][0]=="nope") and (lhists[1][0]=="nope") and (lhists[2][0]=="nope") and (lhists[3][0]=="nope") and (lhists[4][0]=="nope") and (lhists[5][0]=="nope") and (lhists[6][0]=="nope")):
					    if axis[0]==1:
						    lhist[1].SetMaximum(axis[2])
						    lhist[1].SetMinimum(axis[1])
					    lhist[1].Draw()
				    else:
					    lhist[1].Draw("same")

					

		legend.AddEntry("","#sqrt{s} = 8 TeV","")
		if division==0:
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

		if division==1:
			canvadiv = TCanvas("","",0,0,300,300)
			divqcd=lhists[0][1]
			divqcd.Add(lhists[0][1],lhists[1][1],1,1)
			divdata=lhists[2][1]
			divqcd.Scale(integral/qcdint)
			divhisto=lhists[2][1]
			divhisto.Divide(divdata,divqcd,1,1)
			divhisto.Draw()
			if plot[0]==1:
				canvadiv.SaveAs("histograms/div"+names[plots.index(plot)]+".jpg")
				canvadiv.SaveAs("histograms/div"+names[plots.index(plot)]+".pdf")
				canvadiv.SaveAs("histograms/div"+names[plots.index(plot)]+".eps")
				canvadiv.SaveAs("histograms/div"+names[plots.index(plot)]+".root")
			if plot[1]==1:
				canvadiv.SetLogy(1)
				canvadiv.SaveAs("histograms/logdiv"+names[plots.index(plot)]+".jpg")
				canvadiv.SaveAs("histograms/logdiv"+names[plots.index(plot)]+".pdf")
				canvadiv.SaveAs("histograms/logdiv"+names[plots.index(plot)]+".eps")
				canvadiv.SaveAs("histograms/logdiv"+names[plots.index(plot)]+".root")

		


