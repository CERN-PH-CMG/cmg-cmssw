#!/usr/bin/env python
import ROOT 
from math import *
import os, os.path
import sys

def doSpam(text,x1,y1,x2,y2,align=12,fill=False,textSize=0.033,_noDelete={}):
    cmsprel = ROOT.TPaveText(x1,y1,x2,y2,"NDC");
    cmsprel.SetTextSize(textSize);
    cmsprel.SetFillColor(0);
    cmsprel.SetFillStyle(1001 if fill else 0);
    cmsprel.SetLineStyle(2);
    cmsprel.SetLineColor(0);
    cmsprel.SetTextAlign(align);
    cmsprel.SetTextFont(42);
    cmsprel.AddText(text);
    cmsprel.Draw("same");
    _noDelete[text] = cmsprel; ## so it doesn't get deleted by PyROOT
    return cmsprel

def doLegend(x1,y1,x2,y2,textSize=0.035):
    leg = ROOT.TLegend(x1,y1,x2,y2)
    leg.SetFillColor(0)
    leg.SetShadowColor(0)
    leg.SetTextFont(42)
    leg.SetTextSize(textSize)
    return leg



ROOT.gROOT.SetBatch(True)
ROOT.gROOT.ProcessLine(".x ../tdrstyle.cc")
ROOT.gStyle.SetOptStat(0)
ROOT.gStyle.SetOptTitle(0)
c1 = ROOT.TCanvas("c1","c1", 600, 600)
c1.SetWindowSize(600 + (600 - c1.GetWw()), 600 + (600 - c1.GetWh()));
c1.SetTicky(0)
c1.SetTickx(0)
c1.SetLeftMargin(0.37)


results = {}
postfix = sys.argv[1] if len(sys.argv) > 1 else ""
if postfix == "":
    results = {
      '2l'    : [7.7809, 2.0299, 2.7468, 3.9219, 5.7509, 8.1871],
      'ee'    : [9.4109, 4.7529, 6.5029, 9.4688, 14.1866, 20.7481],
      'em'    : [6.8722, 2.6046, 3.5537, 5.1094, 7.533, 10.8783],
      'mumu'  : [12.5072, 2.8754, 3.9232, 5.6406, 8.3612, 12.1043],
      '3l'    : [8.8295, 2.0769, 2.866, 4.2031, 6.3644, 9.4474],
      'comb'  : [6.6315, 1.4297, 1.94, 2.8047, 4.1351, 5.9371],
      '4l'    : [6.7873, 4.0246, 5.7377, 8.8438, 14.2725, 22.653],
    }
elif postfix == "NJet":
    results = {
      'ee'    : [9.2052, 4.8156, 6.5887, 9.5938, 14.2974, 20.9799],
      'em'    : [6.804, 2.6205, 3.5557, 5.1406, 7.579, 10.9449],
      'mumu'  : [12.3717, 2.9073, 3.9448, 5.7031, 8.4538, 12.1689],
      '2l'    : [7.557, 2.0467, 2.7809, 3.9844, 5.8108, 8.2491],
      '3l'    : [8.8511, 2.1712, 2.9832, 4.3594, 6.601, 9.7466],
      '4l'    : [6.7873, 4.0246, 5.7377, 8.8438, 14.2725, 22.653],
      'comb'  : [6.4466, 1.4616, 1.9942, 2.8672, 4.2272, 6.0694],
    }
elif postfix == "QMVA":
    results = {
      '4l'    : [6.7873, 4.0246, 5.7377, 8.8438, 14.2725, 22.653],
      '2l'    : [8.7756, 1.6806, 2.2867, 3.2969, 4.9133, 7.1298],
      'ee'    : [11.1866, 4.6094, 6.3199, 9.2188, 13.8855, 20.3513],
      'em'    : [6.4745, 2.4582, 3.368, 4.8594, 7.2418, 10.5678],
      'mumu'  : [13.8929, 2.1387, 2.9512, 4.3281, 6.5881, 9.7463],
      '3l'    : [6.3581, 1.8015, 2.5037, 3.7344, 5.7141, 8.601],
      'comb'  : [6.2663, 1.1758, 1.6121, 2.3516, 3.5232, 5.1528],
    }
elif postfix == "QNJet":
    results = {
      '2l'    : [9.075, 1.6994, 2.3154, 3.3594, 4.9529, 7.1524],
      'ee'    : [11.2285, 4.5554, 6.2859, 9.2188, 13.8855, 20.4618],
      'em'    : [6.6524, 2.4898, 3.3923, 4.9219, 7.2958, 10.6218],
      'mumu'  : [14.2071, 2.1541, 2.9725, 4.3594, 6.601, 9.7466],
      '3l'    : [6.0498, 1.8618, 2.5875, 3.8594, 5.9054, 8.8889],
      'comb'  : [6.4094, 1.2039, 1.6472, 2.3984, 3.5935, 5.2555],
      '4l'    : [6.7873, 4.0246, 5.7377, 8.8438, 14.2725, 22.653],
    }
else: raise RuntimeError, "Embeh?"




frame = ROOT.TH2F("frame","frame", 100, 1.0, 30, 5, 0, 5);
frame.GetXaxis().SetTitle("95% CL upper limit on #mu = #sigma/#sigma_{SM}")
frame.GetYaxis().SetBinLabel(1, "#splitline{combined}{   #mu < %.1f (%.1f exp)}" % (results['comb'][0],results['comb'][3])  )
frame.GetYaxis().SetBinLabel(2, "#splitline{dilepton}{   #mu < %.1f (%.1f exp)}" % (results['2l'  ][0],results['2l'  ][3])  )
frame.GetYaxis().SetBinLabel(3, "#splitline{trilepton}{  #mu < %.1f (%.1f exp)}" % (results['3l'  ][0],results['3l'  ][3])  )
frame.GetYaxis().SetBinLabel(4, "#splitline{four-lepton}{#mu < %.1f (%.1f exp)}" % (results['4l'  ][0],results['4l'  ][3])  )
frame.GetYaxis().SetLabelSize(0.070);
frame.GetXaxis().SetNoExponent(1);
frame.GetXaxis().SetMoreLogLabels(1);
frame.Draw()
c1.SetLogx(1)


obs = ROOT.TGraphAsymmErrors(4)
exp = ROOT.TGraphAsymmErrors(4)
b68 = ROOT.TGraphAsymmErrors(4)
b95 = ROOT.TGraphAsymmErrors(4)
for i,l in enumerate(["comb","2l","3l","4l"]):
    obs.SetPoint(i, results[l][0], 0.5+i)
    exp.SetPoint(i, results[l][3], 0.5+i)
    b68.SetPoint(i, results[l][3], 0.5+i)
    b95.SetPoint(i, results[l][3], 0.5+i)
    b68.SetPointError(i, results[l][3]-results[l][2], results[l][4]-results[l][3], 0.5, 0.5)
    b95.SetPointError(i, results[l][3]-results[l][1], results[l][5]-results[l][3], 0.5, 0.5)
    exp.SetPointError(i, 0,0, 0.5, 0.5)
    obs.SetPointError(i, 0,0, 0.5, 0.5)

b68.SetFillColor(80);
b95.SetFillColor(90);
for X in exp, b68, b95:
    X.SetMarkerStyle(25);
    X.SetLineWidth(3);
    X.SetLineStyle(2);
obs.SetMarkerStyle(21);
obs.SetLineWidth(3);
obs.SetLineStyle(1);
b95.Draw("E2 SAME")
b68.Draw("E2 SAME")
exp.Draw("PZ SAME")
obs.Draw("PZ SAME")

#stopLine = ROOT.TLine(results['4l'][0]+results['4l'][1]-0.02,  0.25, results['4l'][0]+results['4l'][1]-0.02, 0.75);
#stopLine.SetLineStyle(2);
#stopLine.SetLineWidth(2);
#stopLine.Draw()

#combtext = "combined #mu = %.2f_{#scale[1.4]{-}%.2f}^{+%.2f}" % (results['comb'][0],-results['comb'][1],results['comb'][2])
frame.Draw("AXIS SAME")
doSpam("#sqrt{s} = 8 TeV,  L = 19.6 fb^{-1}",.48, .955, .975, .995, align=32, textSize=0.0355)
doSpam("CMS Preliminary",                    .38, .875, .690, .927, align=21, textSize=0.045)
doSpam("m_{H} = 125.7 GeV",                  .38, .795, .690, .867, align=21, textSize=0.045)
#doSpam( combtext,                            .32, .765, .950, .847, align=21, textSize=0.045)
leg = doLegend(.710,.795,.950,.937, textSize=0.0355)
leg.AddEntry(obs, "Observed", "LP")
leg.AddEntry(b68, "Exp. (68%)", "LPF")
leg.AddEntry(b95, "Exp. (95%)", "LPF")
leg.Draw()

c1.Print("clslimit%s.png" % postfix)
c1.Print("clslimit%s.pdf" % postfix)

frame = ROOT.TH2F("frame","frame", 100, 1.0, 30, 8, 0, 8);
frame.GetXaxis().SetTitle("95% CL upper limit on #mu = #sigma/#sigma_{SM}")
frame.GetYaxis().SetBinLabel(1, "#splitline{combined}{   #mu < %.1f (%.1f exp)}" % (results['comb'][0],results['comb'][3])  )
frame.GetYaxis().SetBinLabel(2, "#splitline{dimuon}{   #mu < %.1f (%.1f exp)}" % (results['mumu'  ][0],results['mumu'  ][3])  )
frame.GetYaxis().SetBinLabel(3, "#splitline{dielectron}{   #mu < %.1f (%.1f exp)}" % (results['ee'  ][0],results['ee'  ][3])  )
frame.GetYaxis().SetBinLabel(4, "#splitline{cross-lepton}{   #mu < %.1f (%.1f exp)}" % (results['em'  ][0],results['em'  ][3])  )
frame.GetYaxis().SetBinLabel(5, "#splitline{trilepton}{  #mu < %.1f (%.1f exp)}" % (results['3l'  ][0],results['3l'  ][3])  )
frame.GetYaxis().SetBinLabel(6, "#splitline{four-lepton}{#mu < %.1f (%.1f exp)}" % (results['4l'  ][0],results['4l'  ][3])  )
frame.GetYaxis().SetLabelSize(0.070);
frame.GetXaxis().SetNoExponent(1);
frame.GetXaxis().SetMoreLogLabels(1);
frame.Draw()
c1.SetLogx(1)


obs = ROOT.TGraphAsymmErrors(6)
exp = ROOT.TGraphAsymmErrors(6)
b68 = ROOT.TGraphAsymmErrors(6)
b95 = ROOT.TGraphAsymmErrors(6)
for i,l in enumerate(["comb","mumu","ee","em","3l","4l"]):
    obs.SetPoint(i, results[l][0], 0.5+i)
    exp.SetPoint(i, results[l][3], 0.5+i)
    b68.SetPoint(i, results[l][3], 0.5+i)
    b95.SetPoint(i, results[l][3], 0.5+i)
    b68.SetPointError(i, results[l][3]-results[l][2], results[l][4]-results[l][3], 0.5, 0.5)
    b95.SetPointError(i, results[l][3]-results[l][1], results[l][5]-results[l][3], 0.5, 0.5)
    exp.SetPointError(i, 0,0, 0.5, 0.5)
    obs.SetPointError(i, 0,0, 0.5, 0.5)

b68.SetFillColor(80);
b95.SetFillColor(90);
for X in exp, b68, b95:
    X.SetMarkerStyle(25);
    X.SetLineWidth(3);
    X.SetLineStyle(2);
obs.SetMarkerStyle(21);
obs.SetLineWidth(3);
obs.SetLineStyle(1);
b95.Draw("E2 SAME")
b68.Draw("E2 SAME")
exp.Draw("PZ SAME")
obs.Draw("PZ SAME")

#stopLine = ROOT.TLine(results['4l'][0]+results['4l'][1]-0.02,  0.25, results['4l'][0]+results['4l'][1]-0.02, 0.75);
#stopLine.SetLineStyle(2);
#stopLine.SetLineWidth(2);
#stopLine.Draw()

#combtext = "combined #mu = %.2f_{#scale[1.4]{-}%.2f}^{+%.2f}" % (results['comb'][0],-results['comb'][1],results['comb'][2])
frame.Draw("AXIS SAME")
doSpam("#sqrt{s} = 8 TeV,  L = 19.6 fb^{-1}",.48, .955, .975, .995, align=32, textSize=0.0355)
doSpam("CMS Preliminary",                    .38, .875, .690, .927, align=21, textSize=0.045)
doSpam("m_{H} = 125.7 GeV",                  .38, .795, .690, .867, align=21, textSize=0.045)
#doSpam( combtext,                            .32, .765, .950, .847, align=21, textSize=0.045)
leg = doLegend(.710,.795,.950,.937, textSize=0.0355)
leg.AddEntry(obs, "Observed", "LP")
leg.AddEntry(b68, "Exp. (68%)", "LPF")
leg.AddEntry(b95, "Exp. (95%)", "LPF")
leg.Draw()

c1.Print("clslimitSplit%s.png" % postfix)
c1.Print("clslimitSplit%s.pdf" % postfix)
