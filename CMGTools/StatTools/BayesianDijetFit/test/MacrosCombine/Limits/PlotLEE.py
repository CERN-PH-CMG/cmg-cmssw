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
 bin=["2","012"]

 masses =[1000.0, 1100.0, 1200.0, 1300.0, 1400.0, 1500.0, 1600.0, 1700.0, 1800.0, 1900.0, 2000.0, 2100.0, 2200.0, 2300.0]
 #masses =[2000.0, 2100.0, 2200.0, 2300.0]
 name="1000_2300" 

 toysPerJob=200
 jobs=50

 if True:
  for ibin in bin:
    sigWW={}
    pvalWW={}
    sigZZ={}
    pvalZZ={}
    for mass in masses:
        print "mass = ",mass
        sigWW[masses.index(mass)]=[]
        sigZZ[masses.index(mass)]=[]
        pvalWW[masses.index(mass)]=[]
        pvalZZ[masses.index(mass)]=[]
        for chan in channel:
	  for job in range(jobs):
            #print "we look inside the fit output for bin "+str(ibin)
            f_fit=file("Xvv.mX"+str(mass)+"_"+sChan[chan]+"_8TeV_channel"+str(ibin)+"_toys"+str(job)+".out")
            for line in f_fit.readlines():
                if "Significance:" in line:
                    if chan == 0:
                        sigWW[masses.index(mass)]+=[float(line.split(":")[1])]
                    if chan == 1:
                        sigZZ[masses.index(mass)]+=[float(line.split(":")[1])] 
                if "(p-value " in line:
                    #print float(line.split("=")[1].split(")")[0])
                    if chan == 0:
                        pvalWW[masses.index(mass)]+=[float(line.split("=")[1].split(")")[0])]
                    if chan == 1:
                        pvalZZ[masses.index(mass)]+=[float(line.split("=")[1].split(")")[0])] 
            f_fit.close()

    #print "sigWW",[(masses[i],sigWW[i]) for i in range(len(masses))]
    #print "sigZZ",[(masses[i],sigZZ[i]) for i in range(len(masses))]

    #print "pvalWW",[(masses[i],pvalWW[i]) for i in range(len(masses))]
    #print "pvalZZ",[(masses[i],pvalZZ[i]) for i in range(len(masses))]

    sSigWW=[(masses[i],sigWW[i]) for i in range(len(masses))]
    sSigZZ=[(masses[i],sigZZ[i]) for i in range(len(masses))]

    sPvalWW=[(masses[i],pvalWW[i]) for i in range(len(masses))]
    sPvalZZ=[(masses[i],pvalZZ[i]) for i in range(len(masses))]

    fWW = open("Xvv_WW_8TeV_Pval_channel"+str(ibin)+"_toys.txt", "w")
    fWW.write(str(sPvalWW))
    fWW.close()

    fZZ = open("Xvv_ZZ_8TeV_Pval_channel"+str(ibin)+"_toys.txt", "w")
    fZZ.write(str(sPvalZZ))
    fZZ.close()

    fWW = open("Xvv_WW_8TeV_Sig_channel"+str(ibin)+"_toys.txt", "w")
    fWW.write(str(sSigWW))
    fWW.close()

    fZZ = open("Xvv_ZZ_8TeV_Sig_channel"+str(ibin)+"_toys.txt", "w")
    fZZ.write(str(sSigZZ))
    fZZ.close()
 
 for chan in sChan:

  #bin=[0,1,2,3,4,5]
  bin=["2","012"]

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
