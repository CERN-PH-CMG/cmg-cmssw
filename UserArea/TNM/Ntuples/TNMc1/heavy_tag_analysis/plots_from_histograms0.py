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
  plots = [[#("dijet_mass_0mtag","0x 70<m<100"),
            ("dijet_mass_1mtag_0mdtag","1x 70<m<100, 0x #mu<0.25"),("dijet_mass_1mtag_1mdtag","1x 70<m<100, 1x #mu<0.25")],
           [("dijet_mass_2mtag_0mdtag","2x 70<m<100, 0x #mu<0.25"),("dijet_mass_2mtag_1mdtag","2x 70<m<100, 1x #mu<0.25"),("dijet_mass_2mtag_2mdtag","2x 70<m<100, 2x #mu<0.25")],
           [#("dijet_mass_0mtag","0x 70<m<100"),
	    ("dijet_mass_1mtag_0nstag","1x 70<m<100, 0x #tau_{12}<0.5"),("dijet_mass_1mtag_1nstag","1x 70<m<100, 1x #tau_{12}<0.5")],
	   [("dijet_mass_2mtag_0nstag","2x 70<m<100, 0x #tau_{12}<0.5"),("dijet_mass_2mtag_1nstag","2x 70<m<100, 1x #tau_{12}<0.5"),("dijet_mass_2mtag_2nstag","2x 70<m<100, 2x #tau_{12}<0.5")],
           [#("dijet_mass_0mtag","0x 70<m<100"),
	    ("dijet_mass_1mtag_0nsltag","1x 70<m<100, 0x #tau_{12}<0.55"),("dijet_mass_1mtag_1nsltag","1x 70<m<100, 1x #tau_{12}<0.55")],
	   [("dijet_mass_2mtag_0nsltag","2x 70<m<100, 0x #tau_{12}<0.55"),("dijet_mass_2mtag_1nsltag","2x 70<m<100, 1x #tau_{12}<0.55"),("dijet_mass_2mtag_2nsltag","2x 70<m<100, 2x #tau_{12}<0.55")],
           [("dijet_mass_2mtag_1mdtag","1x / 0x #mu<0.25 (70<m<100)","dijet_mass_2mtag_0mdtag"),
	    ("dijet_mass_2smtag_1mdtag","1x / 0x #mu<0.25 (40<m<70)","dijet_mass_2smtag_0mdtag")],
	   [("dijet_mass_2mtag_2mdtag","2x / 1x #mu<0.25 (70<m<100)","dijet_mass_2mtag_1mdtag"),
	    ("dijet_mass_2smtag_2mdtag","2x / 1x #mu<0.25 (40<m<70)","dijet_mass_2smtag_1mdtag")],
           [("dijet_mass_2mtag_1nstag","1x / 0x #tau_{12}<0.5 (70<m<100)","dijet_mass_2mtag_0nstag"),
	    ("dijet_mass_2smtag_1nstag","1x / 0x #tau_{12}<0.5 (40<m<70)","dijet_mass_2smtag_0nstag")],
	   [("dijet_mass_2mtag_2nstag","2x / 1x #tau_{12}<0.5 (70<m<100)","dijet_mass_2mtag_1nstag"),
	    ("dijet_mass_2smtag_2nstag","2x / 1x #tau_{12}<0.5 (40<m<70)","dijet_mass_2smtag_1nstag")],
           [("dijet_mass_1mtag_1smdtag","1x / 0x 70<m<100 (md>0.8)","dijet_mass_0mtag_1smdtag"),("dijet_mass_2mtag_1smdtag","2x / 1x 70<m<100 (md>0.8)","dijet_mass_1mtag_1smdtag")],
           [("dijet_mass_2mtag_2mdtag","2x 70<m<100, 2x #mu<0.25 (AK5)"),
	    ("dijet_mass7_2mtag_2mdtag","2x 70<m<100, 2x #mu<0.25 (AK7)"),
	    ("dijet_mass8_2mtag_2mdtag","2x 70<m<100, 2x #mu<0.25 (CA8)")],
           [("dijet_mass_2smtag_2mdtag","2x 40<m<70, 2x #mu<0.25 (AK5)"),
	    ("dijet_mass7_2smtag_2mdtag","2x 40<m<70, 2x #mu<0.25 (AK7)"),
	    ("dijet_mass8_2smtag_2mdtag","2x 40<m<70, 2x #mu<0.25 (CA8)")],
           [("dijet_mass_1mtag_1mdtag","#mu<0.25 / #mu<0.4 (1x 70<m<100)","dijet_mass_1mtag_1mdltag"),("dijet_mass_2mtag_2mdtag","#mu<0.25 / #mu<0.4 (2x 70<m<100)","dijet_mass_2mtag_1mdtag_1mdltag")],
           [("dijet_mass_2mtag_2mdtag","2x #mu<0.25 (2x 70<m<100)"),("dijet_mass_2mtag_1mdtag_1mdltag","1x #mu<0.25 1x #mu<0.4 (2x 70<m<100)")],
           [("dijet_mass_2mtag_0mdtag","0x #mu<0.25 (70<m<100)"),("dijet_mass_2mtag_1mdtag","1x #mu<0.25 (70<m<100)"),("dijet_mass_2mtag_2mdtag","2x #mu<0.25 (70<m<100)")],
           [("dijet_mass_2smtag_0mdtag","0x #mu<0.25 (40<m<70)"),("dijet_mass_2smtag_1mdtag","1x #mu<0.25 (40<m<70)"),("dijet_mass_2smtag_2mdtag","2x #mu<0.25 (40<m<70)")],
           [("dijet_mass_0Htag","0x 100<m<150 0x #mu<0.25"),("dijet_mass_1Htag","1x 100<m<150 1x #mu<0.25"),("dijet_mass_2Htag","2x 100<m<150 2x #mu<0.25")],
           [("dijet_mass_0mtag","0x 70<m<100 (CA8)"),
	    ("dijet_mass_0mak7tag","0x 70<m<100 (AK7)")],
           [("dijet_mass8_1mtag_0mdtag","1x 70<m<100, 0x #mu<0.25 (CA8)"),
	    ("dijet_mass_1mak7tag_0mdtag","1x 70<m<100, 0x #mu<0.25 (AK7)")],
           [("dijet_mass8_1mtag_1mdtag","1x 70<m<100, 1x #mu<0.25 (CA8)"),
	    ("dijet_mass_1mak7tag_1mdtag","1x 70<m<100, 1x #mu<0.25 (AK7)")],
           [("dijet_mass8_2mtag_0mdtag","2x 70<m<100, 0x #mu<0.25 (CA8)"),
	    ("dijet_mass_2mak7tag_0mdtag","2x 70<m<100, 0x #mu<0.25 (AK7)")],
           [("dijet_mass8_2mtag_1mdtag","2x 70<m<100, 1x #mu<0.25 (CA8)"),
	    ("dijet_mass_2mak7tag_1mdtag","2x 70<m<100, 1x #mu<0.25 (AK7)")],
           [("dijet_mass8_2mtag_2mdtag","2x 70<m<100, 2x #mu<0.25 (CA8)"),
	    ("dijet_mass_2mak7tag_2mdtag","2x 70<m<100, 2x #mu<0.25 (AK7)")],
           [("dijet_mass8_2smtag_2mdtag","2x 40<m<70, 2x #mu<0.25 (CA8)"),
	    ("dijet_mass_2smak7tag_2mdtag","2x 40<m<70, 2x #mu<0.25 (AK7)")],
           #[("dijet_mass_2ssmtag_2mdtag","2x m<70, 2x #mu<0.25 (CA8)"),
	   # ("dijet_mass_2ssmak7tag_2mdtag","2x m<70, 2x #mu<0.25 (AK7)")],
           ]

  names = ["baseline1","baseline2","baseline1_nsubjettiness","baseline2_nsubjettiness","baseline1_nsubjettinessloose","baseline2_nsubjettinessloose","massside1","massside2","massside1_nsubjettiness","massside2_nsubjettiness","massdropside","baseline_ca8","massside_ca8","loosetight","loosetight2","baseline_compare","massside_compare","higgs","baseline0_ak7","baseline10_ak7","baseline11_ak7","baseline20_ak7","baseline21_ak7","baseline22_ak7","massside_ak7"]

  hists=[]
  
  bins = [890, 944, 1000, 1058, 1118, 1181, 1246, 1313, 1383, 1455, 1530, 1607, 1687,
  1770, 1856, 1945, 2037, 2132, 2231, 2332, 2438, 2546, 2659, 2775, 2895, 3019, 3147, 3279, 3416, 3558, 3704, 3854, 4010, 4171, 4337, 4509,
  4686, 4869, 5058]

  binning=array.array('d')
  for bin in bins:
     binning.append(bin)

  colors=[1,2,4,7,6,8,9,10]
  styles=[1,2,3,4,1,2,3,4]

  for ratio in ["","ratio"]:
   for plot in plots:
    canvas = TCanvas("","",0,0,200,200)
    
    legend=TLegend(0.3,0.7,0.95,0.90,"")

    f=TFile.Open("dijetWtag_Moriond_Mar6.root")
    print plot[0][0]
    hist=f.Get(plot[0][0])
    hist=hist.Rebin(len(bins)-1,hist.GetName()+"_rebin",binning)
    if len(plot[0])>2:
        hist3=f.Get(plot[0][2])
        hist3=hist3.Rebin(len(bins)-1,hist3.GetName()+"_rebin",binning)
        hist.Divide(hist3)
    hist.SetTitle("")
    hist.SetLineColor(1)
    hist.SetLineStyle(1)
    hist.SetLineWidth(2)
    hist.SetFillStyle(0)
    hist.SetMarkerStyle(0)
    hist.GetXaxis().SetTitle("dijet mass")
    hist.GetYaxis().SetTitle("N")
    hist.Draw("histe")
    legend.AddEntry(hist,plot[0][1],"l")
    hist.GetXaxis().SetRangeUser(890,4000)
    hist.GetYaxis().SetRangeUser(0.5,1000000)
    if len(plot[0])>2:
        hist.GetYaxis().SetTitle("ratio")
        hist.GetYaxis().SetRangeUser(0,2)
    if names[plots.index(plot)]=="massdropside":
        hist.GetYaxis().SetRangeUser(0,0.2)
    if "massside2" in names[plots.index(plot)]:
        hist.GetYaxis().SetRangeUser(0,1)
    if "compare" in names[plots.index(plot)]:
       hist.Scale(1./hist.Integral())
       hist.GetYaxis().SetRangeUser(0.0001,0.5)
    hists+=[hist]
    fit=TF1('fit1','[0]*pow(1.0-x/7000.0,[1])/pow(x/7000.0,[2]+[3]*log(x/7000.0))',890,10000)
    fit.SetParameter(0,hist.Integral())
    fit.SetParameter(1,5)
    fit.SetParameter(2,5)
    fit.FixParameter(3,0)
    fit.SetLineWidth(1)
    fit.SetLineColor(hist.GetLineColor())
    fit.SetLineStyle(hist.GetLineStyle())
    hist.Fit(fit,"R0")
    if ratio:
        hist.GetYaxis().SetTitle("(data-fit)/#sigma_{data}")
        hist.GetYaxis().SetRangeUser(-5,5)
	for b in range(hist.GetNbinsX()):
	    if hist.GetBinError(b+1)>0:
                hist.SetBinContent(b+1,(hist.GetBinContent(b+1)-fit.Integral(hist.GetXaxis().GetBinLowEdge(b+1),hist.GetXaxis().GetBinUpEdge(b+1))/hist.GetBinWidth(b+1))/hist.GetBinError(b+1))
                #hist.SetBinContent(b+1,(hist.GetBinContent(b+1)-fit.Eval(hist.GetBinCenter(b+1)))/hist.GetBinError(b+1))
	    else:
                hist.SetBinContent(b+1,0)
	    hist.SetBinError(b+1,0)
    elif not len(plot[0])>2:
        fit.Draw('lsame')
        canvas.SetLogy(True)
	events=0
        for b in range(hist.FindBin(2000)-1,hist.FindBin(2000)+2):
            events+=hist.GetBinContent(b)-fit.Integral(hist.GetXaxis().GetBinLowEdge(b),hist.GetXaxis().GetBinUpEdge(b))/hist.GetBinWidth(b)
        events= '%(number).3f' % {"number":events}
        print "signal events in three bins around 2 TeV: ", events
	bgevents=0
        for b in range(hist.FindBin(2000)-1,hist.FindBin(2000)+2):
            bgevents+=fit.Integral(hist.GetXaxis().GetBinLowEdge(b),hist.GetXaxis().GetBinUpEdge(b))/hist.GetBinWidth(b)
        bgevents= '%(number).3f' % {"number":bgevents}
        print "bg events in three bins around 2 TeV: ", bgevents

    for i in range(1,len(plot)):
        print plot[i][0]
        hist2=f.Get(plot[i][0])
        hist2=hist2.Rebin(len(bins)-1,hist2.GetName()+"_rebin",binning)
        if len(plot[i])>2:
            hist4=f.Get(plot[i][2])
            hist4=hist4.Rebin(len(bins)-1,hist4.GetName()+"_rebin",binning)
            hist2.Divide(hist4)
        hist2.SetLineColor(colors[i])
        hist2.SetLineStyle(styles[i])
        hist2.SetFillStyle(0)
        hist2.SetLineWidth(2)
        hist2.SetMarkerStyle(0)
        hist2.Draw("histesame")
        legend.AddEntry(hist2,plot[i][1],"l")
        hist2.GetYaxis().SetRangeUser(0.5,1000000)
        if len(plot[0])>2:
            hist2.GetYaxis().SetTitle("ratio")
            hist2.GetYaxis().SetRangeUser(0,2)
        if names[plots.index(plot)]=="massdropside":
            hist.GetYaxis().SetRangeUser(0,0.2)
        if "massside2" in names[plots.index(plot)]:
            hist2.GetYaxis().SetRangeUser(0,1)
        if "compare" in names[plots.index(plot)]:
            hist2.Scale(1./hist2.Integral())
            hist2.GetYaxis().SetRangeUser(0.0001,0.5)
        hists+=[hist2]
        fit2=TF1('fit2','[0]*pow(1.0-x/7000.0,[1])/pow(x/7000.0,[2]+[3]*log(x/7000.0))',890,10000)
        fit2.SetParameter(0,hist2.Integral())
        fit2.SetParameter(1,5)
        fit2.SetParameter(2,5)
        fit2.FixParameter(3,0)
        fit2.SetLineWidth(1)
        fit2.SetLineColor(hist2.GetLineColor())
        fit2.SetLineStyle(hist2.GetLineStyle())
        hist2.Fit(fit2,"R0")
        if ratio:
            hist2.GetYaxis().SetTitle("(data-fit)/#sigma_{data}")
            hist2.GetYaxis().SetRangeUser(-5,5)
  	    for b in range(hist2.GetNbinsX()):
  	    	if hist2.GetBinContent(b+1)==0: continue
  	    	if hist2.GetBinError(b+1)>0:
        	    hist2.SetBinContent(b+1,(hist2.GetBinContent(b+1)-fit2.Integral(hist2.GetXaxis().GetBinLowEdge(b+1),hist2.GetXaxis().GetBinUpEdge(b+1))/hist2.GetBinWidth(b+1))/hist2.GetBinError(b+1))
        	    #hist2.SetBinContent(b+1,(hist2.GetBinContent(b+1)-fit2.Eval(hist2.GetBinCenter(b+1)))/hist2.GetBinError(b+1))
  	    	else:
        	    hist2.SetBinContent(b+1,0)
  	    	hist2.SetBinError(b+1,0)
        elif not len(plot[0])>2:
            fit2.Draw('lsame')
	    events=0
  	    for b in range(hist2.FindBin(2000)-1,hist2.FindBin(2000)+2):
                events+=hist2.GetBinContent(b)-fit2.Integral(hist2.GetXaxis().GetBinLowEdge(b),hist2.GetXaxis().GetBinUpEdge(b))/hist2.GetBinWidth(b)
            events= '%(number).3f' % {"number":events}
            print "signal events in three bins around 2 TeV: ", events
	    bgevents=0
  	    for b in range(hist2.FindBin(2000)-1,hist2.FindBin(2000)+2):
                bgevents+=fit2.Integral(hist2.GetXaxis().GetBinLowEdge(b),hist2.GetXaxis().GetBinUpEdge(b))/hist2.GetBinWidth(b)
            bgevents= '%(number).3f' % {"number":bgevents}
            print "bg events in three bins around 2 TeV: ", bgevents

    legend.SetTextSize(0.04)
    legend.SetFillStyle(0)
    legend.Draw("same")

    canvas.SaveAs("data2012_v2_"+ratio+"_"+names[plots.index(plot)]+".pdf")
    canvas.SaveAs("data2012_v2_"+ratio+"_"+names[plots.index(plot)]+".root")
    f.Close()
