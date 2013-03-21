import os, sys
import array
from ROOT import * 
from os import path

gROOT.Reset()
gROOT.SetStyle("Plain")
gStyle.SetOptStat(0)
gStyle.SetOptFit(0)
gStyle.SetTitleOffset(1.2,"Y")
gStyle.SetPadLeftMargin(0.18)
gStyle.SetPadBottomMargin(0.15)
gStyle.SetPadTopMargin(0.08)
gStyle.SetPadRightMargin(0.05)
gStyle.SetMarkerSize(0.5)
gStyle.SetHistLineWidth(1)
gStyle.SetStatFontSize(0.020)
gStyle.SetTitleSize(0.06, "XYZ")
gStyle.SetLabelSize(0.05, "XYZ")
gStyle.SetNdivisions(510, "XYZ")
gStyle.SetLegendBorderSize(0)

if __name__ == '__main__':
  plots = [[("deta_0mtag","0x 70<m<100"),
            ("deta_1mtag_0mdtag","1x 70<m<100, 0x #mu<0.25"),("deta_1mtag_1mdtag","1x 70<m<100, 1x #mu<0.25"),
            ("deta_2mtag_0mdtag","2x 70<m<100, 0x #mu<0.25"),("deta_2mtag_1mdtag","2x 70<m<100, 1x #mu<0.25"),("deta_2mtag_2mdtag","2x 70<m<100, 2x #mu<0.25")],
           [("njets_0mtag","0x 70<m<100"),
            ("njets_1mtag_0mdtag","1x 70<m<100, 0x #mu<0.25"),("njets_1mtag_1mdtag","1x 70<m<100, 1x #mu<0.25"),
            ("njets_2mtag_0mdtag","2x 70<m<100, 0x #mu<0.25"),("njets_2mtag_1mdtag","2x 70<m<100, 1x #mu<0.25"),("njets_2mtag_2mdtag","2x 70<m<100, 2x #mu<0.25")],
           [("njetsCA8_0mtag","0x 70<m<100"),
            ("njetsCA8_1mtag_0mdtag","1x 70<m<100, 0x #mu<0.25"),("njetsCA8_1mtag_1mdtag","1x 70<m<100, 1x #mu<0.25"),
            ("njetsCA8_2mtag_0mdtag","2x 70<m<100, 0x #mu<0.25"),("njetsCA8_2mtag_1mdtag","2x 70<m<100, 1x #mu<0.25"),("njetsCA8_2mtag_2mdtag","2x 70<m<100, 2x #mu<0.25")],
           ]

  names = ["deta","njets","njetsCA8"]

  hists=[]
  
  colors=[1,2,4,7,6,8,9,10]
  styles=[1,2,3,4,1,2,3,4]

  samples=[("dijetWtag_Moriond_Mar6","data"),
           ("dijetWtag_Moriond_QCD1000","QCD"),
	   ("dijetWtag_Moriond_WWHpp2000","WW 2 TeV"),
	   ]

  for plot in plots:
   for i in range(0,len(plot)):
    canvas = TCanvas("","",0,0,200,200)
    
    legend=TLegend(0.3,0.7,0.95,0.90,"")

    f=TFile.Open(samples[0][0]+".root")
    if "00" in samples[0][0] and "deta" in plot[i][0]:
        plot[i]=("deta_0mtag","0x 70<m<100")
    print plot[i][0]
    hist=f.Get(plot[i][0])
    hist.SetTitle("")
    hist.SetLineColor(1)
    hist.SetLineStyle(1)
    hist.SetLineWidth(2)
    hist.SetFillStyle(0)
    hist.SetMarkerStyle(0)
    if "deta" in plot[i][0]:
        hist.Rebin(20)
        hist.GetXaxis().SetTitle("#Delta #eta")
        hist.GetXaxis().SetRangeUser(0,1.299)
    if "njets" in plot[i][0]:
        hist.GetXaxis().SetTitle("n_{jets}")
        hist.GetXaxis().SetRangeUser(2,9.99)
    hist.GetYaxis().SetRangeUser(0,hist.GetMaximum()*1.5)
    hist.GetYaxis().SetTitle("N")
    hist.Draw("histe")
    legend.AddEntry(hist,samples[0][1],"l")
    hists+=[hist]

    for j in range(1,len(samples)):
        f=TFile.Open(samples[j][0]+".root")
        if "00" in samples[j][0] and "deta" in plot[i][0]:
            plot[i]=("deta_0mtag","0x 70<m<100")
        print plot[i][0]
        hist2=f.Get(plot[i][0])
        hist2.SetLineColor(colors[j])
        hist2.SetLineStyle(styles[j])
        hist2.SetFillStyle(0)
        hist2.SetLineWidth(2)
        hist2.SetMarkerStyle(0)
        if "deta" in plot[i][0]:
            hist2.Rebin(20)
	#if "njets" in plot[i][0] and "Hpp" in samples[j][0]:
	#    continue
	hist2.Scale(hist.Integral()/hist2.Integral())
        hist2.Draw("histesame")
        legend.AddEntry(hist2,samples[j][1],"l")
        #hist2.GetYaxis().SetRangeUser(0.5,1000000)
        hists+=[hist2]

    legend.SetTextSize(0.04)
    legend.SetFillStyle(0)
    legend.Draw("same")

    canvas.SaveAs("data2012_v3_"+plot[i][0]+"_"+names[plots.index(plot)]+".pdf")
    canvas.SaveAs("data2012_v3_"+plot[i][0]+"_"+names[plots.index(plot)]+".root")
    f.Close()
