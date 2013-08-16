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
  plots = [[("dijet_mass_2mtag_0mdtag","2x 70<m<100, 0x #mu<0.25"),("dijet_mass_2mtag_1mdtag","2x 70<m<100, 1x #mu<0.25"),("dijet_mass_2mtag_2mdtag","2x 70<m<100, 2x #mu<0.25")],
           [("dijet_mass_2mtag_0nstag","2x 70<m<100, 0x #tau_{12}<0.5"),("dijet_mass_2mtag_1nstag","2x 70<m<100, 1x #tau_{12}<0.5"),("dijet_mass_2mtag_2nstag","2x 70<m<100, 2x #tau_{12}<0.5")],
           [("dijet_mass_2mtag_0nsltag","2x 70<m<100, 0x #tau_{12}<0.55"),("dijet_mass_2mtag_1nsltag","2x 70<m<100, 1x #tau_{12}<0.55"),("dijet_mass_2mtag_2nsltag","2x 70<m<100, 2x #tau_{12}<0.55")],
           [("dijet_mass_2mtag_2mdtag","2x #mu<0.25 (2x 70<m<100)"),("dijet_mass_2mtag_1mdtag_1mdltag","1x #mu<0.25 1x #mu<0.4 (2x 70<m<100)")],
           [("dijet_mass_2mak7tag_0mdtag","AK7 2x 70<m<100, 0x #mu<0.25"),("dijet_mass_2mak7tag_1mdtag","AK7 2x 70<m<100, 1x #mu<0.25"),("dijet_mass_2mak7tag_2mdtag","AK7 2x 70<m<100, 2x #mu<0.25")],
           [("dijet_mass_2mak7tag_0nstag","AK7 2x 70<m<100, 0x #tau_{12}<0.5"),("dijet_mass_2mak7tag_1nstag","AK7 2x 70<m<100, 1x #tau_{12}<0.5"),("dijet_mass_2mak7tag_2nstag","AK7 2x 70<m<100, 2x #tau_{12}<0.5")],
           ]

  names = ["signal","signal_nsubjettiness","signal_nsubjettinessloose","signal_loosetight","signal_ak7","signal_ak7_nsubjettiness",]

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
    
    legend=TLegend(0.2,0.7,0.95,0.90,"")

    f=TFile.Open("dijetWtag_Moriond_WWHpp2000.root")
    print plot[0][0]
    hist=f.Get(plot[0][0])
    hist=hist.Rebin(len(bins)-1,hist.GetName()+"_rebin",binning)
    if len(plot[0])>2:
        hist3=f.Get(plot[0][2])
        hist3=hist3.Rebin(len(bins)-1,hist3.GetName()+"_rebin",binning)
        hist.Divide(hist3)
    hist.Scale(1./30000.)
    hist.SetTitle("")
    hist.SetLineColor(1)
    hist.SetLineStyle(1)
    hist.SetLineWidth(2)
    hist.SetFillStyle(0)
    hist.SetMarkerStyle(0)
    hist.GetXaxis().SetTitle("dijet mass")
    hist.GetYaxis().SetTitle("N")
    hist.Draw("histe")
    efficiency= 'eff=%(number).3f' % {"number":hist.Integral(hist.FindBin(600),hist.FindBin(2600))}
    legend.AddEntry(hist,plot[0][1]+" "+efficiency,"l")
    hist.GetXaxis().SetRangeUser(890,4000)
    hist.GetYaxis().SetRangeUser(0,0.03)
    if len(plot[0])>2:
        hist.GetYaxis().SetTitle("ratio")
        hist.GetYaxis().SetRangeUser(0,2)
    if names[plots.index(plot)]=="massdropside":
        hist.GetYaxis().SetRangeUser(0,0.2)
    if "massside2" in names[plots.index(plot)]:
        hist.GetYaxis().SetRangeUser(0,1)
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
        #fit.Draw('lsame')
        #canvas.SetLogy(True)
	pass

    for i in range(1,len(plot)):
        print plot[i][0]
        hist2=f.Get(plot[i][0])
        hist2=hist2.Rebin(len(bins)-1,hist2.GetName()+"_rebin",binning)
        if len(plot[i])>2:
            hist4=f.Get(plot[i][2])
            hist4=hist4.Rebin(len(bins)-1,hist4.GetName()+"_rebin",binning)
            hist2.Divide(hist4)
	hist2.Scale(1./30000.)
        print hist2.Integral(hist2.FindBin(600),hist2.FindBin(2600))
        hist2.SetLineColor(colors[i])
        hist2.SetLineStyle(styles[i])
        hist2.SetFillStyle(0)
        hist2.SetLineWidth(2)
        hist2.SetMarkerStyle(0)
        hist2.Draw("histesame")
        efficiency= 'eff=%(number).3f' % {"number":hist2.Integral(hist2.FindBin(600),hist2.FindBin(2600))}
        legend.AddEntry(hist2,plot[i][1]+" "+efficiency,"l")
        hist2.GetYaxis().SetRangeUser(0,0.03)
        if len(plot[0])>2:
            hist2.GetYaxis().SetTitle("ratio")
            hist2.GetYaxis().SetRangeUser(0,2)
        if names[plots.index(plot)]=="massdropside":
            hist.GetYaxis().SetRangeUser(0,0.2)
        if "massside2" in names[plots.index(plot)]:
            hist2.GetYaxis().SetRangeUser(0,1)
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
            #fit2.Draw('lsame')
	    pass

    legend.SetTextSize(0.04)
    legend.SetFillStyle(0)
    legend.Draw("same")

    canvas.SaveAs("data2012_v2_"+ratio+"_"+names[plots.index(plot)]+".pdf")
    canvas.SaveAs("data2012_v2_"+ratio+"_"+names[plots.index(plot)]+".root")
    f.Close()
