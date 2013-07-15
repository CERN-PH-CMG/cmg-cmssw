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

channels=["WW","ZZ","WZ","qW","qZ"]
channels=["qW","WW"]

fullToys=False

for chan in channels:
    print "chan =",chan

    if "q" in chan:
       masses =[1000.0, 1100.0, 1200.0, 1300.0, 1400.0, 1500.0, 1600.0, 1700.0, 1800.0, 1900.0, 2000.0, 2100.0, 2200.0, 2300.0, 2400.0, 2500.0, 2600.0, 2700.0, 2800.0, 2900.0, 3000.0, 3100.0, 3200.0, 3300.0, 3400.0, 3500.0, 3600.0, 3700.0, 3800.0, 3900.0, 4000.0]
       bins=[3,4,"34"]
    else:
       masses =[1000.0, 1100.0, 1200.0, 1300.0, 1400.0, 1500.0, 1600.0, 1700.0, 1800.0, 1900.0, 2000.0, 2100.0, 2200.0, 2300.0, 2400.0, 2500.0, 2600.0, 2700.0, 2800.0, 2900.0]
       bins=[0,1,"01"]

    if fullToys:
      points=[]
      for p in range(1,10):
    	points+=[float(p/10.)]
    	points+=[float(p/10.+0.05)]
    	points+=[float(p/1.)]
    	points+=[float(p/1.+0.5)]
    	points+=[float(p*10.)]
    	points+=[float(p*10.+5.)]
    else:
      points=[0.1]
   
    #points=[0.1]

    for bin in bins:

        for mass in masses:
          print "mass =",mass

          for point in points:

            outputname = "Xvv.mX"+str(mass)+"_"+chan+"_8TeV_channel"+str(bin)+"_limit"+str(int(point*10))+"_submit.src"
            logname = "Xvv.mX"+str(mass)+"_"+chan+"_8TeV_channel"+str(bin)+"_limit"+str(int(point*10))+"_submit.out"
            outputfile = open(outputname,'w')
            outputfile.write('#!/bin/bash\n')
            outputfile.write("cd ${CMSSW_BASE}/src/CMGTools/StatTools/MacrosCombine; eval `scramv1 run -sh`\n")
	    if fullToys:
              outputfile.write("combine datacards/Xvv.mX"+str(mass)+"_" + chan + "_8TeV_channel"+str(bin)+".txt -M HybridNew --frequentist --clsAcc 0 -T 100 -i 30 --singlePoint "+str(point)+" -s 10000"+str(int(point*100))+" --saveHybridResult --saveToys -m "+str(mass) + " -n "+chan+str(bin)+" &>Xvv.mX"+str(mass)+"_" + chan + "_Full_8TeV_channel"+str(bin)+"_toy"+str(int(point*10))+".out\n")
              outputfile.write("hadd -f grid_mX"+str(mass)+"_" + chan + "_8TeV_channel"+str(bin)+".root higgsCombine" + chan + str(bin)+".HybridNew.mH"+str(int(mass))+".10000*.root\n")
              outputfile.write("combine datacards/Xvv.mX"+str(mass)+"_" + chan + "_8TeV_channel"+str(bin)+".txt -M HybridNew --frequentist --grid grid_mX"+str(mass)+"_" + chan + "_8TeV_channel"+str(bin)+".root -m "+str(mass) + " -n "+chan+str(bin)+" &>Xvv.mX"+str(mass)+"_" + chan + "_Full_8TeV_channel"+str(bin)+"_obs.out\n")
              outputfile.write("combine datacards/Xvv.mX"+str(mass)+"_" + chan + "_8TeV_channel"+str(bin)+".txt -M HybridNew --frequentist --grid grid_mX"+str(mass)+"_" + chan + "_8TeV_channel"+str(bin)+".root -m "+str(mass) + " -n "+chan+str(bin)+" --expectedFromGrid 0.5 &>Xvv.mX"+str(mass)+"_" + chan + "_Full_8TeV_channel"+str(bin)+"_50.out\n")
	      outputfile.write("combine datacards/Xvv.mX"+str(mass)+"_" + chan + "_8TeV_channel"+str(bin)+".txt -M HybridNew --frequentist --grid grid_mX"+str(mass)+"_" + chan + "_8TeV_channel"+str(bin)+".root -m "+str(mass) + " -n "+chan+str(bin)+" --expectedFromGrid 0.16 &>Xvv.mX"+str(mass)+"_" + chan + "_Full_8TeV_channel"+str(bin)+"_16.out\n")
	      outputfile.write("combine datacards/Xvv.mX"+str(mass)+"_" + chan + "_8TeV_channel"+str(bin)+".txt -M HybridNew --frequentist --grid grid_mX"+str(mass)+"_" + chan + "_8TeV_channel"+str(bin)+".root -m "+str(mass) + " -n "+chan+str(bin)+" --expectedFromGrid 0.84 &>Xvv.mX"+str(mass)+"_" + chan + "_Full_8TeV_channel"+str(bin)+"_84.out\n")
	      outputfile.write("combine datacards/Xvv.mX"+str(mass)+"_" + chan + "_8TeV_channel"+str(bin)+".txt -M HybridNew --frequentist --grid grid_mX"+str(mass)+"_" + chan + "_8TeV_channel"+str(bin)+".root -m "+str(mass) + " -n "+chan+str(bin)+" --expectedFromGrid 0.025 &>Xvv.mX"+str(mass)+"_" + chan + "_Full_8TeV_channel"+str(bin)+"_025.out\n")
	      outputfile.write("combine datacards/Xvv.mX"+str(mass)+"_" + chan + "_8TeV_channel"+str(bin)+".txt -M HybridNew --frequentist --grid grid_mX"+str(mass)+"_" + chan + "_8TeV_channel"+str(bin)+".root -m "+str(mass) + " -n "+chan+str(bin)+" --expectedFromGrid 0.975 &>Xvv.mX"+str(mass)+"_" + chan + "_Full_8TeV_channel"+str(bin)+"_975.out\n")
            else:
                outputfile.write("combine datacards/Xvv.mX"+str(mass)+"_" + chan + "_8TeV_channel"+str(bin)+".txt -M Asymptotic -v2 -m "+str(mass) + " -n "+chan+str(bin)+" --rMax 1000 --rMin 0.01 &>Xvv.mX"+str(mass)+"_" + chan + "_Asymptotic_8TeV_channel"+str(bin)+".out\n")
                outputfile.write("mv higgsCombine"+chan+str(bin)+".Asymptotic.mH"+str(int(mass))+".root Limits/Xvv.mX"+str(mass)+"_" + chan + "_Asymptotic_8TeV_channel"+str(bin)+".root")
            outputfile.close()
  
            command="rm "+logname
	    print command
            os.system(command)
	    if fullToys:
                command="""bsub -q 8nh -o """+logname+" source "+outputname
            else:
	        command="bsub -q 1nh -o "+logname+" source "+outputname
	    print command
            os.system(command)
	    #os.system("source "+outputname)
