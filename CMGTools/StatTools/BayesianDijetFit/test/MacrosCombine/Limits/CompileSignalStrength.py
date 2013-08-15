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
sChan=["WW","ZZ"]
bin=[0,1,2,3,4,5]
sBin=["m2mg0", "m2mg1", "m2mg2", "m1mg0", "m1mg1", "m0"]
muExp=[]

for ibin in bin:
    for chan in channel:

        masses =[1200.0, 1300.0, 1400.0, 1500.0, 1600.0, 1700.0, 1800.0, 1900.0, 2000.0, 2100.0, 2200.0, 2300.0]

        muExp=[]

        for mass in masses:
            print "mass = ",mass
        
            f_card=file("../datacards/Xvv.mX"+str(mass)+"_"+ sChan[chan] +"_8TeV_channel"+str(bin[ibin])+".txt")

            for line in f_card.readlines():
                #            print line
                if "rate" in line:
                    print float(line.split("                        ")[1].split(" ")[0])
                    muExp+=[float(line.split("                        ")[1].split(" ")[0])]
            f_card.close()

        sMuExp=[(masses[i],muExp[i]) for i in range(len(masses))]

        f_out=open("Xvv.mX_"+sChan[chan]+"_MuExp_8TeV_channel"+str(bin[ibin])+"_"+sBin[ibin]+".txt", "w")
        f_out.write(str(sMuExp))
        f_out.close()
