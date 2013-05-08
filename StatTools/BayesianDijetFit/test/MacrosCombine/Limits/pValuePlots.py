import ROOT as rt
import os
import sys
from array import *

# define the plot canvas
def setStyle(c,histo):
    # canvas style
    rt.gStyle.SetOptStat(0)
    rt.gStyle.SetOptTitle(0)        
    
    c.SetTickx(1)
    c.SetTicky(1)
    
    c.SetRightMargin(0.14)
    c.SetTopMargin(0.14)
    c.SetLeftMargin(0.14)
    c.SetBottomMargin(0.14)
    
    # set x axis
    histo.GetXaxis().SetLabelFont(42)
    histo.GetXaxis().SetLabelSize(0.04)
    histo.GetXaxis().SetTitleFont(42)
    histo.GetXaxis().SetTitleSize(0.05)
    histo.GetXaxis().SetTitleOffset(1.2)
    histo.GetXaxis().CenterTitle(True)

    # set y axis
    histo.GetYaxis().SetLabelFont(42)
    histo.GetYaxis().SetLabelSize(0.04)
    histo.GetYaxis().SetTitleFont(42)
    histo.GetYaxis().SetTitleSize(0.05)
    histo.GetYaxis().SetTitleOffset(1.2)
    histo.GetYaxis().CenterTitle(True)

def PlotPValue(filePVALNAME, label):

    filePVAL = open(filePVALNAME)
    pvalPairs = eval(filePVAL.readline())

    mass = []
    pval = []

    for entry in pvalPairs:
        mass.append(entry[0])
        pval.append(entry[1])

    pvalGraph = rt.TGraph(len(mass), array('d',mass), array('d',pval))
    c1 = rt.TCanvas("c1","c1", 300, 300)
    c1.SetLogy()
    pvalGraph.Draw("AL")
    htemp = pvalGraph.GetHistogram()
    c1.SetLogy()
    setStyle(c1,htemp)
    htemp.GetXaxis().SetTitle("Mass (GeV)")
    htemp.GetYaxis().SetTitle("p-value")
    htemp.SetMinimum(1E-5)
    htemp.SetMaximum(1)

    # sigmas
    sigmas = [0.8413447,0.9772499,0.9986501,0.9999683,0.9999997]
    line = rt.TLine()
    line.SetLineColor(2)
    for sigma in sigmas:
        line.DrawLine(htemp.GetXaxis().GetXmin(), 1-sigma,htemp.GetXaxis().GetXmax(), 1-sigma)
        line.Draw("SAME")
        pvalGraph.Draw("LSAME")
    text = []
    for i in range(1,6):
        text.append(rt.TLatex(htemp.GetXaxis().GetXmax()*0.92, (1-sigmas[i-1])*1.10,"%i #sigma" %i))
        #text = rt.TLatex(1800.,0.001,"%i #sigma" %i)
        #text.SetNDC()
        #text.SetTextFont(42)
        #text.SetTextSize(0.038)
        text[i-1].SetTextColor(2)
        text[i-1].Draw("SAME")

    c1.Update()
    c1.SaveAs("pvalue_%s.pdf" %label)

def PlotMu(muFILENAME, label):
    
    fileMU = open(muFILENAME)
    muPairs = eval(fileMU.readline())

    mass = []
    mu = []
    muplus = []
    muminus = []
    for entry in muPairs:
        mass.append(entry[0])
        mu.append(entry[1]*20.)
        muminus.append(entry[2]*20.+entry[1]*20.)
        muplus.append(entry[3]*20.+entry[1]*20.)

    #nominal
    muGraph = rt.TGraph(len(mass), array('d',mass), array('d',mu))

    # green band
    muBAND = []
    massBAND = []
    for i in range(0, len(mass)):
        muBAND.append(muplus[i])
        massBAND.append(mass[i])
    for i in range(1, len(mass)+1):
        muBAND.append(muminus[len(mass)-i])
        massBAND.append(mass[len(mass)-i])

    print massBAND
    print muBAND

    muGraphBAND = rt.TGraph(len(massBAND), array('d',massBAND), array('d',muBAND))
    c1 = rt.TCanvas("c1","c1", 300, 300)
    muGraphBAND.SetFillStyle(1001)
    muGraphBAND.SetFillColor(rt.kGreen)
    muGraphBAND.Draw("AF")
    htemp = muGraphBAND.GetHistogram()
    setStyle(c1,htemp)
    htemp.GetXaxis().SetTitle("Mass (GeV)")
    htemp.GetYaxis().SetTitle("Best-fit Yield")
    htemp.SetMinimum(0)
    htemp.SetMaximum(60)
    muGraph.Draw("PLSAME")

    c1.Update()
    c1.SaveAs("mu_%s.pdf" %label)

if __name__ == '__main__':
    """
    PlotPValue("Xvv_WW_8TeV_Pval_channel0_m2mg0.txt", "WW_channel0_m2mg0")
    PlotPValue("Xvv_WW_8TeV_Pval_channel1_m2mg1.txt", "WW_channel1_m2mg1")
    PlotPValue("Xvv_WW_8TeV_Pval_channel2_m2mg2.txt", "WW_channel2_m2mg2")
    PlotPValue("Xvv_WW_8TeV_Pval_channel3_m1mg0.txt", "WW_channel3_m1mg0")
    PlotPValue("Xvv_WW_8TeV_Pval_channel4_m1mg1.txt", "WW_channel4_m1mg1")
    PlotPValue("Xvv_WW_8TeV_Pval_channel5_m0.txt", "WW_channel5_m0")

    PlotPValue("Xvv_ZZ_8TeV_Pval_channel0_m2mg0.txt", "ZZ_channel0_m2mg0")
    PlotPValue("Xvv_ZZ_8TeV_Pval_channel1_m2mg1.txt", "ZZ_channel1_m2mg1")
    PlotPValue("Xvv_ZZ_8TeV_Pval_channel2_m2mg2.txt", "ZZ_channel2_m2mg2")
    PlotPValue("Xvv_ZZ_8TeV_Pval_channel3_m1mg0.txt", "ZZ_channel3_m1mg0")
    PlotPValue("Xvv_ZZ_8TeV_Pval_channel4_m1mg1.txt", "ZZ_channel4_m1mg1")
    PlotPValue("Xvv_ZZ_8TeV_Pval_channel5_m0.txt", "ZZ_channel5_m0")

    PlotMu("Xvv_WW_8TeV_channel0_m2mg0.txt", "WW_channel0_m2mg0")
    PlotMu("Xvv_WW_8TeV_channel1_m2mg1.txt", "WW_channel1_m2mg1")
    PlotMu("Xvv_WW_8TeV_channel2_m2mg2.txt", "WW_channel2_m2mg2")
    PlotMu("Xvv_WW_8TeV_channel3_m1mg0.txt", "WW_channel3_m1mg0")
    PlotMu("Xvv_WW_8TeV_channel4_m1mg1.txt", "WW_channel4_m1mg1")
    PlotMu("Xvv_WW_8TeV_channel5_m0.txt", "WW_channel5_m0")

    PlotMu("Xvv_ZZ_8TeV_channel0_m2mg0.txt", "ZZ_channel0_m2mg0")
    PlotMu("Xvv_ZZ_8TeV_channel1_m2mg1.txt", "ZZ_channel1_m2mg1")
    PlotMu("Xvv_ZZ_8TeV_channel2_m2mg2.txt", "ZZ_channel2_m2mg2")
    PlotMu("Xvv_ZZ_8TeV_channel3_m1mg0.txt", "ZZ_channel3_m1mg0")
    PlotMu("Xvv_ZZ_8TeV_channel4_m1mg1.txt", "ZZ_channel4_m1mg1")
    PlotMu("Xvv_ZZ_8TeV_channel5_m0.txt", "ZZ_channel5_m0")
    """
    PlotPValue("Xvv_WW_8TeV_Pval_channel0.txt", "WW_high_purity")
    PlotPValue("Xvv_WW_8TeV_Pval_channel1.txt", "WW_medium_purity")
    PlotPValue("Xvv_WW_8TeV_Pval_channel2.txt", "WW_low_purity")
    PlotPValue("Xvv_WW_8TeV_Pval_channel012.txt", "WW_combined")

    PlotPValue("Xvv_ZZ_8TeV_Pval_channel0.txt", "ZZ_high_puriy")
    PlotPValue("Xvv_ZZ_8TeV_Pval_channel1.txt", "ZZ_medium_purity")
    PlotPValue("Xvv_ZZ_8TeV_Pval_channel2.txt", "ZZ_low_purity")
    PlotPValue("Xvv_ZZ_8TeV_Pval_channel012.txt", "ZZ_combine")

    PlotPValue("Xvv_WZ_8TeV_Pval_channel0.txt", "WZ_high_puriy")
    PlotPValue("Xvv_WZ_8TeV_Pval_channel1.txt", "WZ_medium_purity")
    PlotPValue("Xvv_WZ_8TeV_Pval_channel2.txt", "WZ_low_purity")
    PlotPValue("Xvv_WZ_8TeV_Pval_channel012.txt", "WZ_combine")
    