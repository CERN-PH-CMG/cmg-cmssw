import os, sys
import array
from ROOT import * 

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

    colors=[2,3,4,6,7,8,9,10]
    styles=[2,3,4,5,6,7,8,9]
    
    prefix="CMS_2012_5_fb_forHCP_"
    systematics=["NODR",
                 "NODR_iso06",
		 "NODR_iso07",
		 "DR3",
		 "DR4"]
    categories=["BOOSTED"]#,"VBF"
    variables=["svfitMass"]#,"l1Pt"

    for category in categories:
     for variable in variables:
      canvas = TCanvas("","",0,0,200,200)
      legend=TLegend(0.2,0.65,0.95,0.90,"")
    
      files=[]
      hists=[]
    
      index=0
      for systematic in systematics:
        print category,systematic
	f=TFile.Open(prefix+systematic+"_"+category+"/"+prefix+systematic+"_"+category+"_QCD_TauTau_mH125_PLOT.root")
	files+=[f]
        hist=f.Get(variable+"/QCDlooseOS").Clone("QCDlooseOS"+systematic)
	hist.SetLineColor(colors[index])
        hist.SetLineStyle(styles[index])
	hist.SetFillStyle(0)
        hist.SetMarkerStyle(0)
        binning=array.array('d')
	if variable=="svfitMass":
            #bins=[20,40,60,80,100,120,140,160,200,300]
            bins=[20,40,60,80,100,120,140,160,180,200,220,240,260,280,300]
	else:
            bins=[40,60,80,100,120,160,200]
	for bin in bins:
           binning.append(bin)
        hist=hist.Rebin(len(bins)-1,hist.GetName()+"_rebin",binning)
        hists+=[hist]
	if index==0:
	    hist1=hist
        hist.Scale(hist1.Integral()/hist.Integral())

        ratio = hist.Clone("QCDlooseOSratio"+systematic)
        ratio.Divide(hist,hist1,1,1,"B")
        ratio.Add(TF1("offset","-1",bins[0],bins[-1]))
	if category=="BOOSTED":
            ratio.GetYaxis().SetRangeUser(-0.8,0.8)
	else:
            ratio.GetYaxis().SetRangeUser(-2,2)
        hists+=[ratio]
	if index==0:
	    for b in range(ratio.GetNbinsX()):
	      if hist.GetBinContent(b+1)>0 and ratio.GetBinContent(b+1)>-0.9:
	        ratio.SetBinError(b+1,hist.GetBinError(b+1)/hist.GetBinContent(b+1))
	      elif hist.GetBinContent(b)>0:
	        ratio.SetBinError(b+1,ratio.GetBinError(b))
	      elif hist.GetBinContent(b-1)>0:
	        ratio.SetBinError(b+1,ratio.GetBinError(b-1))
	      elif hist.GetBinContent(b-2)>0:
	        ratio.SetBinError(b+1,ratio.GetBinError(b-2))
	      else:
	        ratio.SetBinError(b+1,0)
        for b in range(ratio.GetNbinsX()):
	      if hist.GetBinContent(b+1)>0 and ratio.GetBinContent(b+1)>-0.9:
	        pass
	      elif hist.GetBinContent(b)>0:
	        ratio.SetBinContent(b+1,ratio.GetBinContent(b))
	      elif hist.GetBinContent(b-1)>0:
	        ratio.SetBinContent(b+1,ratio.GetBinContent(b-1))
	      elif hist.GetBinContent(b-2)>0:
	        ratio.SetBinContent(b+1,ratio.GetBinContent(b-2))
	if index==0:
	    unc=ratio
	if index==0:
            ratio.Draw("he")
	else:	
            ratio.Draw("histsame")
	if systematic=="NODR":
            legend.AddEntry(ratio,"Nominal #pm stat unc.","l")
	elif "DR3" in systematic:
            legend.AddEntry(ratio,"#Delta R < 3","l")
	elif "DR4" in systematic:
            legend.AddEntry(ratio,"#Delta R < 4","l")
	elif "iso06" in systematic:
            legend.AddEntry(ratio,"#tau iso < 0.6","l")
	elif "iso07" in systematic:
            legend.AddEntry(ratio,"#tau iso < 0.7","l")
	else:
            legend.AddEntry(ratio,systematic.strip("_"),"l")
	index+=1

      if variable=="svfitMass":
        unc_up=unc.Clone("UncUp")
        for b in range(unc_up.GetNbinsX()):
          unc_up.SetBinContent(b+1,0.5*unc_up.GetBinError(b+1))
        unc_up.SetLineColor(1)
        unc_up.SetLineStyle(1)
        unc_up.Draw("histsame")
        unc_down=unc.Clone("UncDown")
        for b in range(unc_down.GetNbinsX()):
          unc_down.SetBinContent(b+1,-0.5*unc_down.GetBinError(b+1))
        unc_down.SetLineColor(1)
        unc_down.SetLineStyle(1)
        unc_down.Draw("histsame")
        legend.AddEntry(unc_up,"QCD shape unc.","l")

      legend.SetTextSize(0.04)
      legend.SetFillStyle(0)
      legend.Draw("same")
      canvas.SaveAs("QCDShapeUncertainty_plot_"+category+"_"+variable+".pdf")
      unc_down.SaveAs("QCDShapeUncertainty_plot_"+category+"_"+variable+".root")

