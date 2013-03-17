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
muWW=[]
muMinusWW=[]
muPlusWW=[]

muZZ=[]
muMinusZZ=[]
muPlusZZ=[]


sChan=["WW","ZZ"]
#bin=[0,1,2,3,4,5]
sBin=["m2mg0", "m2mg1", "m2mg2", "m1mg0", "m1mg1", "m0"]
bin=[4]

for ibin in bin:

    muWW=[]
    muMinusWW=[]
    muPlusWW=[]

    muZZ=[]
    muMinusZZ=[]
    muPlusZZ=[]

    for chan in channel:

        masses =[1200.0, 1300.0, 1400.0, 1500.0, 1600.0, 1700.0, 1800.0, 1900.0, 2000.0, 2100.0, 2200.0, 2300.0]
        #masses =[1900.0, 2000.0]
        for mass in masses:
            print "mass = ",mass

            fileOut="Xvv.mX"+str(mass)+"_" + sChan[chan] + "_MaxLikelihoodFit_8TeV_channel"+str(ibin)+".out"
            proc="combine ../datacards/Xvv.mX"+str(mass)+"_" + sChan[chan] + "_8TeV_channel"+str(ibin)+".txt -M MaxLikelihoodFit -v2 -m "+str(mass) + " --rMin -10 &>"+fileOut
            print proc
            os.system(proc)
            
            f_fit=file(fileOut)
 
            print "we look inside the fit output for bin "+str(ibin)
            found  = 0
            for line in f_fit.readlines():

                if "Best fit r:" in line:
                    found = 1

                    print line.split(":")[1].split(" ")[1]
                    print line.split(":")[1].split(" ")[3].split("/")[0]
                    print line.split(":")[1].split(" ")[3].split("/")[1]
                    if chan == 0:
                        muWW+=[float(line.split(":")[1].split(" ")[1])]
                        muMinusWW+=[float(line.split(":")[1].split(" ")[3].split("/")[0])]
                        muPlusWW+=[float(line.split(":")[1].split(" ")[3].split("/")[1])]
                    if chan == 1:
                        seq=line.split(":")[1]
                        muZZ+=[float(line.split(":")[1].split(" ")[1])]
                        muMinusZZ+=[float(line.split(":")[1].split(" ")[3].split("/")[0])]
                        muPlusZZ+=[float(line.split(":")[1].split(" ")[3].split("/")[1])]
            f_fit.close()
            if found == 0:
                if chan == 0:
                    muWW+=[0]
                    muMinusWW+=[0]
                    muPlusWW+=[0]
                if chan == 1:
                    muZZ+=[0]
                    muMinusZZ+=[0]
                    muPlusZZ+=[0]

    print "muWW",[(masses[i],muWW[i],muMinusWW[i],muPlusWW[i]) for i in range(len(masses))]
    print "muZZ",[(masses[i],muZZ[i],muMinusZZ[i],muPlusZZ[i]) for i in range(len(masses))]
    sWW=[(masses[i],muWW[i],muMinusWW[i],muPlusWW[i]) for i in range(len(masses))]
    sZZ=[(masses[i],muZZ[i],muMinusZZ[i],muPlusZZ[i]) for i in range(len(masses))]

    fWW = open("Xvv_WW_8TeV_channel"+str(ibin)+"_"+sBin[ibin]+".txt", "w")
    fWW.write(str(sWW))
    fWW.close()
    
    fZZ = open("Xvv_ZZ_8TeV_channel"+str(ibin)+"_"+sBin[ibin]+".txt", "w")
    fZZ.write(str(sZZ))
    fZZ.close()
