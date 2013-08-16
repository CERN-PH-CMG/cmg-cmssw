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

channel=[0, 1]
sChannel=["WW","ZZ"]

#bin=[0,1,2,3,4,5]
bin=[2]
sBin=["m2mg0", "m2mg1", "m2mg2", "m1mg0", "m1mg1", "m0"]

for ibin in bin:

    for chan in channel:

        masses =[1000.0, 1100.0, 1200.0, 1300.0, 1400.0, 1500.0, 1600.0, 1700.0, 1800.0, 1900.0, 2000.0, 2100.0, 2200.0, 2300.0]

        for mass in masses:
            print "mass = ",mass
            outputname = "Xvv.mX"+str(mass)+"_" + sChannel[chan] + "_MarkovChainMC_8TeV_channel"+str(ibin)+".src"
            logname = "Xvv.mX"+str(mass)+"_" + sChannel[chan] + "_MarkovChainMC_8TeV_channel"+str(ibin)+".out"
            outputfile = open(outputname,'w')
            outputfile.write('#!/bin/bash\n')
            outputfile.write("cd ${CMSSW_BASE}/src/CMGTools/StatTools/MacrosCombine/Limits; eval `scramv1 run -sh`\n")
            outputfile.write("combine ../datacards/Xvv.mX"+str(mass)+"_" + sChannel[chan] + "_8TeV_channel"+str(ibin)+".txt -M MarkovChainMC -v2 -m "+str(mass+chan)+"\n")
            outputfile.write("mv higgsCombineTest.MarkovChainMC.mH"+str(int(mass+chan))+".root Xvv.mX"+str(mass)+"_" + sChannel[chan] + "_MarkovChainMC_8TeV_channel"+str(ibin)+"_"+sBin[ibin]+".root\n")
            outputfile.close()
  
            command="rm "+"Xvv.mX"+str(mass)+"_" + sChannel[chan] + "_MarkovChainMC_8TeV_channel"+str(ibin)+"_"+sBin[ibin]+".root"
	    print command
            os.system(command)
            command="rm "+logname
	    print command
            os.system(command)
            command="bsub -q 8nh -o "+logname+" source "+outputname
	    print command
            os.system(command)
