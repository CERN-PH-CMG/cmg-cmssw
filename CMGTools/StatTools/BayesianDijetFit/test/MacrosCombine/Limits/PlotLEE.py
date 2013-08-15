import os, sys
import array
from ROOT import * 
from os import path
from ROOT import RooStats

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

 channel=[0, 1]
 sigWW=[]
 pvalWW=[]

 sigZZ=[]
 pvalZZ=[]

 sChan=["WW","ZZ"]
 #bin=[0,1,2,3,4,5]
 bin=["2"]

 masses =[1000.0, 1100.0, 1200.0, 1300.0, 1400.0, 1500.0, 1600.0, 1700.0, 1800.0, 1900.0, 2000.0, 2100.0, 2200.0, 2300.0]
 #masses =[2000.0, 2100.0, 2200.0, 2300.0]
 masses =[1200.0, 1300.0, 1400.0, 1500.0, 1600.0, 1700.0, 1800.0, 1900.0, 2000.0, 2100.0, 2200.0, 2300.0]
 name="1200_2300" 

 toysPerJob=200
 jobs=50

 for chan in sChan:

  #bin=[0,1,2,3,4,5]
  bin=["2","012","012345"]

  files=["Xvv_"+chan+"_8TeV_Sig_channel"+str(ibin)+"_toys.txt" for ibin in bin]

  colors=[1,2,4,7,6,8,9,10]
  styles=[1,2,3,4,1,2,3,4]

  for f in files:
    print f
    fWW=eval(file(f).readlines()[0].replace("i","-1").replace("n","").replace("f",""))

    xpoints1=array.array('d')
    ypoints1=array.array('d')
    xerrors1=array.array('d')
    yerrors1=array.array('d')
    
    for i in range(2,20):
       x=i*0.25
       ys=[]
       for toy in range(jobs*toysPerJob):
           ymax=0
           for mass,significances in fWW:
	       #if mass<1200: continue
	       ymax=max(ymax,significances[toy])
	   ys+=[ymax]
       y=RooStats.PValueToSignificance(float(len([s for s in ys if s>=x]))/len(ys))
       if y>10 or y<-10: continue
       xpoints1.append(x)
       ypoints1.append(y)
       xerrors1.append(0)
       if len([s for s in ys if s>=x])>0:
          yerror=2*(y-RooStats.PValueToSignificance(float(sqrt(len([s for s in ys if s>=x]))+len([s for s in ys if s>=x]))/len(ys)))
       else:
          yerror=y
       yerrors1.append(yerror)
       print x,y,yerror
    
    canvas = TCanvas("","",0,0,200,200)
    
    legend=TLegend(0.3,0.7,0.95,0.90,"")

    hist=TH1F("h","h",1,0.5,5)
    hist.SetTitle("")
    graph1=TGraphErrors(len(xpoints1),xpoints1,ypoints1,xerrors1,yerrors1)
    graph1.SetTitle("")
    graph1.SetLineColor(1)
    graph1.SetLineStyle(1)
    graph1.SetLineWidth(2)
    graph1.SetFillStyle(0)
    graph1.SetMarkerStyle(0)
    hist.GetXaxis().SetTitle("local signficance")
    hist.GetYaxis().SetTitle("global signficance")
    hist.GetYaxis().SetRangeUser(0.5,5)
    hist.Draw("")
    graph1.Draw("")
    line=TLine(0.5,0.5,5,5)
    line.SetLineColor(2)
    line.Draw("same")
    legend.AddEntry(graph1,chan,"l")

    legend.SetTextSize(0.04)
    legend.SetFillStyle(0)
    legend.Draw("same")

    canvas.SaveAs(f.replace(".txt","_"+name+".pdf"))
    canvas.SaveAs(f.replace(".txt","_"+name+".root"))
