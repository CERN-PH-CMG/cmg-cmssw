from ROOT import *
import ROOT
import array, math
import os

gROOT.Reset()
gROOT.SetStyle("Plain")
gStyle.SetOptStat(0)
gStyle.SetOptFit(0)
gStyle.SetTitleOffset(1.2,"Y")
gStyle.SetPadLeftMargin(0.18)
gStyle.SetPadBottomMargin(0.15)
gStyle.SetPadTopMargin(0.03)
gStyle.SetPadRightMargin(0.05)
gStyle.SetMarkerSize(1.5)
gStyle.SetHistLineWidth(1)
gStyle.SetStatFontSize(0.020)
gStyle.SetTitleSize(0.06, "XYZ")
gStyle.SetLabelSize(0.05, "XYZ")
gStyle.SetNdivisions(510, "XYZ")
gStyle.SetLegendBorderSize(0)

channel=[0]#, 1
sigWW=[]
pvalWW=[]

sigZZ=[]
pvalZZ=[]

sChan=["WW"]#,"ZZ"
#bin=[0,1,2,3,4,5]
bin=["01"]

masses =[1000.0, 1100.0, 1200.0, 1300.0, 1400.0, 1500.0, 1600.0, 1700.0, 1800.0, 1900.0, 2000.0, 2100.0, 2200.0, 2300.0, 2400.0, 2500.0, 2600.0, 2700.0]

toysPerJob=200
jobs=50

for ibin in bin:
    for chan in channel:
        for mass in masses:
	  for job in range(jobs):
            outputname = "Xvv.mX"+str(mass)+"_"+sChan[chan]+"_8TeV_channel"+str(ibin)+"_submit"+str(job)+".src"
            logname = "Xvv.mX"+str(mass)+"_"+sChan[chan]+"_8TeV_channel"+str(ibin)+"_toys"+str(job)+".out"
            outputfile = open(outputname,'w')
            outputfile.write('#!/bin/bash\n')
            outputfile.write("cd ${CMSSW_BASE}/src/CMGTools/StatTools/MacrosCombine; eval `scramv1 run -sh`\n")
            outputfile.write("combine datacards/Xvv.mX"+str(mass)+"_"+sChan[chan]+"_8TeV_channel"+str(ibin)+".txt -M ProfileLikelihood -v2 -m "+str(mass) + " --signif -t "+str(toysPerJob)+" -s "+str(job)+"\n")
            outputfile.close()
  
            command="rm "+logname
	    print command
            os.system(command)
            command="bsub -q 1nh -o "+logname+" source "+outputname
	    print command
            os.system(command)
