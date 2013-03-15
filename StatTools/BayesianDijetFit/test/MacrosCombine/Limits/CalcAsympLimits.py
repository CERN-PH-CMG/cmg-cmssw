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
sigWW=[]
pvalWW=[]

sigZZ=[]
pvalZZ=[]

for chan in channel:

    masses =[1200.0, 1300.0, 1400.0, 1500.0, 1600.0, 1700.0, 1800.0, 1900.0, 2000.0, 2100.0, 2200.0, 2300.0]
#    masses =[1200.0]
    for mass in masses:
        print "mass = ",mass

        os.system("combine ../datacards/Xvv.mX"+str(mass)+"_" + sChannel[chan] + "_8TeV_channel2.txt -M Asymptotic -v2 -m "+str(mass) + " --signif &>Xvv.mX"+str(mass)+"_" + sChannel[chan] + "_Asymptotic_8TeV_channel2.out")
        os.system("cp higgsCombineTest.Asymptotic.mH1200.root gravitonCombineTest.Asymptotic.mH"+str(mass)+"_" + sChannel[chan] + ".root")

 
