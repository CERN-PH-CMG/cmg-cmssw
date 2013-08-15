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

muWW=[]
muMinusWW=[]
muPlusWW=[]

muZZ=[]
muMinusZZ=[]
muPlusZZ=[]

muWZ=[]
muMinusWZ=[]
muPlusWZ=[]


channels=["WW","ZZ","WZ"]
bins=[0,1,2,"012"]

efficiencyScale=30000.*0.005/19.6000 # fb

for bin in bins:

    muWW=[]
    muMinusWW=[]
    muPlusWW=[]

    muZZ=[]
    muMinusZZ=[]
    muPlusZZ=[]

    muWZ=[]
    muMinusWZ=[]
    muPlusWZ=[]

    for chan in channels:
        print "chan =",chan

        masses =[1000.0, 1100.0, 1200.0, 1300.0, 1400.0, 1500.0, 1600.0, 1700.0, 1800.0, 1900.0, 2000.0, 2100.0, 2200.0, 2300.0]
        for mass in masses:
            print "mass =",mass

            fileOut="Limits/Xvv.mX"+str(mass)+"_" + chan + "_MaxLikelihoodFit_8TeV_channel"+str(bin)+".out"
            proc="combine datacards/Xvv.mX"+str(mass)+"_" + chan + "_8TeV_channel"+str(bin)+".txt -M MaxLikelihoodFit -v2 -m "+str(mass) + " --rMax 100 --rMin -100 &>"+fileOut
            print proc
            os.system(proc)
            
            f_fit=file(fileOut)
 
            print "we look inside the fit output for bin "+str(bin)
	    failed = False
	    Mu=0
	    MuPlus=0
	    MuMinus=0
            for line in f_fit.readlines():
		if "     r" in line and "+/-" in line:
                    Mu=efficiencyScale*float(line.replace("  "," ").replace("  "," ").replace("  "," ").replace("  "," ").strip(" ").split(" ")[2])
		    MuMinus=-efficiencyScale*float(line.replace("  "," ").replace("  "," ").replace("  "," ").replace("  "," ").strip(" ").split(" ")[4])
		    MuPlus=efficiencyScale*float(line.replace("  "," ").replace("  "," ").replace("  "," ").replace("  "," ").strip(" ").split(" ")[4])
                if "fail" in line:
		    print line
		    failed=True
                if "Best fit r:" in line:
		    if failed: break
                    Mu=efficiencyScale*float(line.split(":")[1].split(" ")[1])
		    MuMinus=efficiencyScale*float(line.split(":")[1].split(" ")[3].split("/")[0])
		    MuPlus=efficiencyScale*float(line.split(":")[1].split(" ")[3].split("/")[1])
		    break

            print Mu
            print MuMinus
            print MuPlus
            if chan == "WW":
                muWW+=[Mu]
                muMinusWW+=[MuMinus]
                muPlusWW+=[MuPlus]
            if chan == "ZZ":
                muZZ+=[Mu]
                muMinusZZ+=[MuMinus]
                muPlusZZ+=[MuPlus]
            if chan == "WZ":
                muWZ+=[Mu]
                muMinusWZ+=[MuMinus]
                muPlusWZ+=[MuPlus]
            f_fit.close()

    print "muWW",[(masses[i],muWW[i],muMinusWW[i],muPlusWW[i]) for i in range(len(masses))]
    print "muZZ",[(masses[i],muZZ[i],muMinusZZ[i],muPlusZZ[i]) for i in range(len(masses))]
    print "muWZ",[(masses[i],muWZ[i],muMinusWZ[i],muPlusWZ[i]) for i in range(len(masses))]
    sWW=[(masses[i],muWW[i],muMinusWW[i],muPlusWW[i]) for i in range(len(masses))]
    sZZ=[(masses[i],muZZ[i],muMinusZZ[i],muPlusZZ[i]) for i in range(len(masses))]
    sWZ=[(masses[i],muWZ[i],muMinusWZ[i],muPlusWZ[i]) for i in range(len(masses))]

    fWW = open("Xvv_WW_8TeV_channel"+str(bin)+".txt", "w")
    fWW.write(str(sWW))
    fWW.close()
    
    fZZ = open("Xvv_ZZ_8TeV_channel"+str(bin)+".txt", "w")
    fZZ.write(str(sZZ))
    fZZ.close()
    
    fWZ = open("Xvv_WZ_8TeV_channel"+str(bin)+".txt", "w")
    fWZ.write(str(sWZ))
    fWZ.close()
