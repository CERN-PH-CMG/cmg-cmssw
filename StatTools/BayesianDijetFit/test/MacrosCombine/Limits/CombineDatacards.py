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



for chan in channel:

    masses =[1000.0, 1100.0, 1200.0, 1300.0, 1400.0, 1500.0, 1600.0, 1700.0, 1800.0, 1900.0, 2000.0, 2100.0, 2200.0, 2300.0]

    for mass in masses:
        print "mass = ",mass

        bin0="../datacards/Xvv.mX"+str(mass)+"_" + sChannel[chan] + "_8TeV_channel0.txt "
        bin1="../datacards/Xvv.mX"+str(mass)+"_" + sChannel[chan] + "_8TeV_channel1.txt "
        bin2="../datacards/Xvv.mX"+str(mass)+"_" + sChannel[chan] + "_8TeV_channel2.txt "
        bin3="../datacards/Xvv.mX"+str(mass)+"_" + sChannel[chan] + "_8TeV_channel3.txt "
        bin4="../datacards/Xvv.mX"+str(mass)+"_" + sChannel[chan] + "_8TeV_channel4.txt "
        bin5="../datacards/Xvv.mX"+str(mass)+"_" + sChannel[chan] + "_8TeV_channel5.txt "
        
        bin02="../datacards/Xvv.mX"+str(mass)+"_" + sChannel[chan] + "_8TeV_channel0_2.txt "
        bin05="../datacards/Xvv.mX"+str(mass)+"_" + sChannel[chan] + "_8TeV_channel0_5.txt "
        bin01345="../datacards/Xvv.mX"+str(mass)+"_" + sChannel[chan] + "_8TeV_channel01_345.txt " 

        comb02 = "combineCards.py " + bin0 + bin1 + bin2 + " >" + bin02  
        comb05 = "combineCards.py " + bin0 + bin1 + bin2 + bin3 + bin4 + bin5 + " >" + bin05  
        comb01345 = "combineCards.py " + bin0 + bin1 + bin3 + bin4 + bin5 + " >" + bin01345

        print comb02
        print comb05
        print comb01345
        
        os.system( comb02  )
        os.system( comb05  )
        os.system( comb01345 )


 
