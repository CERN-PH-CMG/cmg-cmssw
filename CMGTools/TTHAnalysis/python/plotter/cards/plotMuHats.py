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

ROOT.gROOT.SetBatch(True)
ROOT.gROOT.ProcessLine(".x ../tdrstyle.cc")
ROOT.gStyle.SetOptStat(0)
ROOT.gStyle.SetOptTitle(0)
c1 = ROOT.TCanvas("c1","c1", 600, 600)
c1.SetWindowSize(600 + (600 - c1.GetWw()), 600 + (600 - c1.GetWh()));
c1.SetTicky(0)
c1.SetTickx(0)
c1.SetLeftMargin(0.30)

results = {}
postfix = sys.argv[1] if len(sys.argv) > 1 else ""
if postfix == "":
    results = {
        'ee'   : [ 2.41142, -3.76199, +4.40724 ],
        'em'   : [ 1.9417 , -2.19965, +2.47049 ],
        'mumu' : [ 8.42289, -2.67795, +3.26708 ],
        '2l'   : [ 5.32186, -1.80586, +2.1324  ],
        '3l'   : [ 1.99452, -1.75271, +2.15031 ],
        '4l'   : [ -4.18   , -(6.0-4.18), (0.246+4.18) ],
        'comb' : [ 3.52173, -1.39107, +1.63001 ],
    }
elif postfix == "NJet":
    results = {
        'ee'   : [ -0.861266, -4.46658,+4.64644 ],
        'em'   : [ 1.94726  ,-2.29837,+2.55011 ],
        'mumu' : [ 6.54776  ,-2.81018,+3.29152 ],
        '2l'   : [ 3.72625  ,-1.92177,+2.16402  ],
        '3l'   : [ 4.21621  ,-2.10581,+2.56114 ],
        '4l'   : [ -4.18   , -(6.0-4.18), (0.246+4.18) ],
        'comb' : [ 3.43536 ,-1.48109,+1.70756 ],
    }
elif postfix == "QMVA":
    results = {
        'ee'   : [ 2.41741, -3.7989, +4.38781 ],
        'em'   : [ 1.82389, -2.15234, +2.42362 ],
        'mumu' : [ 8.11871, -2.68611, +3.26181 ],
        '2l'   : [ 5.04319, -1.80378, +2.11682  ],
        '3l'   : [ 2.38814, -1.70408, +2.11605 ],
        '4l'   : [ -4.18   , -(6.0-4.18), (0.246+4.18) ],
        'comb' : [ 3.42641, -1.36123, +1.5986 ],
    }
elif postfix == "QNJet":
    results = {
        'ee'   : [-0.089695, -3.98145, +4.37628 ],
        'em'   : [ 2.00719 ,-2.30918, +2.56645 ],
        'mumu' : [ 6.71137 ,-2.77532, +3.27738  ],
        '2l'   : [ 3.94286 ,-1.9055, +2.17756   ],
        '3l'   : [  4.28502, -2.0529, +2.50972 ],
        '4l'   : [ -4.18   , -(6.0-4.18), (0.246+4.18) ],
        'comb' : [ 3.63018 ,-1.4626, +1.69405 ],
    }

frame = ROOT.TH2F("frame","frame", 100, -7, 9, 4, 0, 4);
frame.GetXaxis().SetTitle("Best fit #mu = #sigma/#sigma_{SM}")
#frame.GetYaxis().SetBinLabel(4, "#splitline{combined}{   #mu = %.2f_{#scale[1.4]{-}%.2f}^{+%.2f}}" % (results['comb'][0],-results['comb'][1],results['comb'][2])  )
#frame.GetYaxis().SetBinLabel(4, "" )
frame.GetYaxis().SetBinLabel(1, "#splitline{dilepton}{   #mu = %.1f_{#scale[1.4]{-}%.1f}^{+%.1f}}" % (results['2l'  ][0],-results['2l'  ][1],results['2l'  ][2])  )
frame.GetYaxis().SetBinLabel(2, "#splitline{trilepton}{  #mu = %.1f_{#scale[1.4]{-}%.1f}^{+%.1f}}" % (results['3l'  ][0],-results['3l'  ][1],results['3l'  ][2])  )
frame.GetYaxis().SetBinLabel(3, "#splitline{four-lepton}{#mu = %.1f_{#scale[1.4]{-}%.1f}^{+%.1f}}" % (results['4l'  ][0],-results['4l'  ][1],results['4l'  ][2])  )
frame.GetYaxis().SetLabelSize(0.070);
frame.Draw()

combBand = ROOT.TBox( results['comb'][0]+results['comb'][1], 0, results['comb'][0]+results['comb'][2], 3);
combLine = ROOT.TLine(results['comb'][0]                   , 0, results['comb'][0]                   , 3);


points = ROOT.TGraphAsymmErrors(3)
for i,l in enumerate(["2l","3l","4l"]):
    points.SetPoint(i, results[l][0], 0.5+i)
    points.SetPointError(i, -results[l][1], results[l][2], 0, 0)

combBand.SetFillColor(80); combBand.SetFillStyle(1001); combBand.SetLineStyle(0);  
combBand.Draw()
combLine.SetLineWidth(4); 
combLine.Draw()

stopLine = ROOT.TLine(results['4l'][0]+results['4l'][1]-0.02,  2.25, results['4l'][0]+results['4l'][1]-0.02, 2.75);
stopLine.SetLineStyle(2);
stopLine.SetLineWidth(2);
stopLine.Draw()

points.SetLineColor(2)
points.SetLineWidth(5)
points.SetMarkerStyle(21)
points.SetMarkerSize(1.7)
points.Draw("P SAME");

combtext = "combined #mu = %.1f_{#scale[1.4]{-}%.1f}^{+%.1f}" % (results['comb'][0],-results['comb'][1],results['comb'][2])
frame.Draw("AXIS SAME")
doSpam("#sqrt{s} = 8 TeV,  L = 19.6 fb^{-1}",.48, .955, .975, .995, align=32, textSize=0.0355)
doSpam("CMS Preliminary",                    .32, .875, .640, .927, align=21, textSize=0.045)
doSpam("m_{H} = 125.7 GeV",                  .66, .875, .950, .927, align=21, textSize=0.045)
doSpam( combtext,                            .32, .765, .950, .847, align=21, textSize=0.045)


c1.Print("muhat%s.pdf" % postfix)
c1.Print("muhat%s.png" % postfix)

frame = ROOT.TH2F("frame","frame", 100, -7, 13, 7, 0, 7);
frame.GetXaxis().SetTitle("Best fit #mu = #sigma/#sigma_{SM}")
#frame.GetYaxis().SetBinLabel(4, "#splitline{combined}{   #mu = %.2f_{#scale[1.4]{-}%.2f}^{+%.2f}}" % (results['comb'][0],-results['comb'][1],results['comb'][2])  )
#frame.GetYaxis().SetBinLabel(4, "" )
frame.GetYaxis().SetBinLabel(1, "#splitline{cross-lepton}{   #mu = %.1f_{#scale[1.4]{-}%.1f}^{+%.1f}}" % (results['em'  ][0],-results['em'  ][1],results['em'  ][2])  )
frame.GetYaxis().SetBinLabel(2, "#splitline{dimuon}{   #mu = %.1f_{#scale[1.4]{-}%.1f}^{+%.1f}}" % (results['mumu'  ][0],-results['mumu'  ][1],results['mumu'  ][2])  )
frame.GetYaxis().SetBinLabel(3, "#splitline{dielectron}{   #mu = %.1f_{#scale[1.4]{-}%.1f}^{+%.1f}}" % (results['ee'  ][0],-results['ee'  ][1],results['ee'  ][2])  )
frame.GetYaxis().SetBinLabel(4, "#splitline{trilepton}{  #mu = %.1f_{#scale[1.4]{-}%.1f}^{+%.1f}}" % (results['3l'  ][0],-results['3l'  ][1],results['3l'  ][2])  )
frame.GetYaxis().SetBinLabel(5, "#splitline{four-lepton}{#mu = %.1f_{#scale[1.4]{-}%.1f}^{+%.1f}}" % (results['4l'  ][0],-results['4l'  ][1],results['4l'  ][2])  )
frame.GetYaxis().SetLabelSize(0.060);
frame.Draw()

combBand = ROOT.TBox( results['comb'][0]+results['comb'][1], 0, results['comb'][0]+results['comb'][2], 5);
combLine = ROOT.TLine(results['comb'][0]                   , 0, results['comb'][0]                   , 5);


points = ROOT.TGraphAsymmErrors(3)
for i,l in enumerate(["em","mumu","ee","3l","4l"]):
    points.SetPoint(i, results[l][0], 0.5+i)
    points.SetPointError(i, -results[l][1], results[l][2], 0, 0)

combBand.SetFillColor(80); combBand.SetFillStyle(1001); combBand.SetLineStyle(0);  
combBand.Draw()
combLine.SetLineWidth(4); 
combLine.Draw()

stopLine = ROOT.TLine(results['4l'][0]+results['4l'][1]-0.02,  4.25, results['4l'][0]+results['4l'][1]-0.02, 4.75);
stopLine.SetLineStyle(2);
stopLine.SetLineWidth(2);
stopLine.Draw()

points.SetLineColor(2)
points.SetLineWidth(5)
points.SetMarkerStyle(21)
points.SetMarkerSize(1.7)
points.Draw("P SAME");

combtext = "combined #mu = %.1f_{#scale[1.4]{-}%.1f}^{+%.1f}" % (results['comb'][0],-results['comb'][1],results['comb'][2])
frame.Draw("AXIS SAME")
doSpam("#sqrt{s} = 8 TeV,  L = 19.6 fb^{-1}",.48, .955, .975, .995, align=32, textSize=0.0355)
doSpam("CMS Preliminary",                    .32, .875, .640, .927, align=21, textSize=0.045)
doSpam("m_{H} = 125.7 GeV",                  .66, .875, .950, .927, align=21, textSize=0.045)
doSpam( combtext,                            .32, .765, .950, .847, align=21, textSize=0.045)


c1.Print("muhatSplit%s.png" % postfix)
c1.Print("muhatSplit%s.pdf" % postfix)
