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
sigWW=[]
pvalWW=[]

sigZZ=[]
pvalZZ=[]

sChan=["WW","ZZ"]
bin=[0,1,2,3,4,5]
sBin=["m2mg0", "m2mg1", "m2mg2", "m1mg0", "m1mg1", "m0"]

for ibin in bin:
    sigWW=[]
    pvalWW=[]

    sigZZ=[]
    pvalZZ=[]
    for chan in channel:

        masses =[1200.0, 1300.0, 1400.0, 1500.0, 1600.0, 1700.0, 1800.0, 1900.0, 2000.0, 2100.0, 2200.0, 2300.0]

        for mass in masses:
            print "mass = ",mass
        
            os.system("combine ../datacards/Xvv.mX"+str(mass)+"_"+sChan[chan]+"_8TeV_channel"+str(ibin)+".txt -M ProfileLikelihood -v2 -m "+str(mass) + " --signif &>Xvv.mX"+str(mass)+"_"+sChan[chan]+"_8TeV_channel"+str(ibin)+".out")
            f_fit=file("Xvv.mX"+str(mass)+"_"+sChan[chan]+"_8TeV_channel"+str(ibin)+".out")

 
            print "we look inside the fit output for bin "+str(ibin)
            for line in f_fit.readlines():
                #            print line
                if "Significance:" in line:
                    #                limits+=[float(line.split(":")[2])]
                    print float(line.split(":")[1])
                    if chan == 0:
                        sigWW+=[float(line.split(":")[1])]
                    if chan == 1:
                        sigZZ+=[float(line.split(":")[1])] 
                if "(p-value " in line:
                    print float(line.split("=")[1].split(")")[0])
                    if chan == 0:
                        pvalWW+=[float(line.split("=")[1].split(")")[0])]
                    if chan == 1:
                        pvalZZ+=[float(line.split("=")[1].split(")")[0])] 
                    f_fit.close()


    print "sigWW",[(masses[i],sigWW[i]) for i in range(len(masses))]
    print "sigZZ",[(masses[i],sigZZ[i]) for i in range(len(masses))]

    print "pvalWW",[(masses[i],pvalWW[i]) for i in range(len(masses))]
    print "pvalZZ",[(masses[i],pvalZZ[i]) for i in range(len(masses))]

    sSigWW=[(masses[i],sigWW[i]) for i in range(len(masses))]
    sSigZZ=[(masses[i],sigZZ[i]) for i in range(len(masses))]

    sPvalWW=[(masses[i],pvalWW[i]) for i in range(len(masses))]
    sPvalZZ=[(masses[i],pvalZZ[i]) for i in range(len(masses))]

    fWW = open("Xvv_WW_8TeV_Pval_channel"+str(ibin)+"_"+sBin[ibin]+".txt", "w")
    fWW.write(str(sPvalWW))
    fWW.close()

    fZZ = open("Xvv_ZZ_8TeV_Pval_channel"+str(ibin)+"_"+sBin[ibin]+".txt", "w")
    fZZ.write(str(sPvalZZ))
    fZZ.close()


    fWW = open("Xvv_WW_8TeV_Sig_channel"+str(ibin)+"_"+sBin[ibin]+".txt", "w")
    fWW.write(str(sSigWW))
    fWW.close()

    fZZ = open("Xvv_ZZ_8TeV_Sig_channel"+str(ibin)+"_"+sBin[ibin]+".txt", "w")
    fZZ.write(str(sSigZZ))
    fZZ.close()
