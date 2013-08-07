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

for chan in channels:
    print "chan =",chan

    if "q" in chan:
       masses =[1000.0, 1100.0, 1200.0, 1300.0, 1400.0, 1500.0, 1600.0, 1700.0, 1800.0, 1900.0, 2000.0, 2100.0, 2200.0, 2300.0, 2400.0, 2500.0, 2600.0, 2700.0, 2800.0, 2900.0, 3000.0, 3100.0, 3200.0, 3300.0, 3400.0, 3500.0, 3600.0, 3700.0, 3800.0, 3900.0, 4000.0]
       bins=[3,4,"34"]
    else:
       masses =[1000.0, 1100.0, 1200.0, 1300.0, 1400.0, 1500.0, 1600.0, 1700.0, 1800.0, 1900.0, 2000.0, 2100.0, 2200.0, 2300.0, 2400.0, 2500.0, 2600.0, 2700.0, 2800.0, 2900.0]
       bins=[0,1,"01"]

    for bin in bins:
        sig=[]
        pval=[]
        for mass in masses:
            print "mass =",mass

            f_fit=file("Limits/Xvv.mX"+str(mass)+"_"+chan+"_8TeV_channel"+str(bin)+".out")
            print "we look inside the fit output for bin "+str(bin)
	    s=0
	    p=0
            for line in f_fit.readlines():
                #            print line
                if "Significance:" in line:
                    #                limits+=[float(line.split(":")[2])]
                    print float(line.split(":")[1])
                    s=float(line.split(":")[1])
                if "(p-value " in line:
                    print float(line.split("=")[1].split(")")[0])
                    p=float(line.split("=")[1].split(")")[0])
                if "Null p-value " in line:
                    print float(line.split(":")[1].strip().split(" ")[0])
                    p=float(line.split(":")[1].strip().split(" ")[0])
            f_fit.close()
	    sig+=[s]
	    pval+=[p]


        print "sig",[(masses[i],sig[i]) for i in range(len(masses))]

        sSig=[(masses[i],sig[i]) for i in range(len(masses))]

        f = open("Limits/Xvv_"+chan+"_8TeV_channel"+str(bin)+".txt", "w")
        f.write(str(sSig))
        f.close()

        print "pval",[(masses[i],pval[i]) for i in range(len(masses))]

        sPval=[(masses[i],pval[i]) for i in range(len(masses))]

        f = open("Limits/Xvv_"+chan+"_8TeV_Pval_channel"+str(bin)+".txt", "w")
        f.write(str(sPval))
        f.close()
