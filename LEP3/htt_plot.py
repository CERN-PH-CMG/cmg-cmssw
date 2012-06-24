#!/usr/bin/env python
import os,string,sys,commands,time,ConfigParser
from ROOT import *
from array import array

mclist=[
    "htt/Hig125/htttreeproducer_httanalyzer/htttreeproducer_httanalyzer_tree.root",
    "htt/ZZ/htttreeproducer_httanalyzer/htttreeproducer_httanalyzer_tree.root",
    "htt/WW/htttreeproducer_httanalyzer/htttreeproducer_httanalyzer_tree.root",
    "htt/QQ/htttreeproducer_httanalyzer/htttreeproducer_httanalyzer_tree.root"
#    "zz.root",
#    "ww.root",
#    "qq.root"
    ]

# xsection of the processes in fb
xsec=[
    200.,
    1446.,
    14080.,
   16750.
    ]

# luminosity to normalize (in fb-1)
lumi=500

# step at which the plot should be made
stepplot=5

# define histograms
step_h=[]
massh_h=[]
massz_h=[]
for index in range(0,len(mclist)):
    step_h.append(TH1F("step"+str(index),"step"+str(index),10,0,10))
    step_h[index].SetLineColor(index+2)
    step_h[index].SetMarkerColor(index+2)
    step_h[index].SetFillColor(index+2)
    massh_h.append(TH1F("Hmass"+str(index),"Hmass"+str(index),40,100,140))
    massh_h[index].SetLineColor(index+2)
    massh_h[index].SetMarkerColor(index+2)
    massh_h[index].SetFillColor(index+2)
    massz_h.append(TH1F("Zmass"+str(index),"Zmass"+str(index),100,50,150))
    massz_h[index].SetLineColor(index+2)
    massz_h[index].SetMarkerColor(index+2)
    massz_h[index].SetFillColor(index+2)


# now loop on tree and project
for index,rootfile in enumerate(mclist):
    treefile=TFile.Open(rootfile)
    tree=treefile.Get("htttreeproducer_httanalyzer")
    nevents=tree.GetEntries()
    # loop on tree entries
    weight=xsec[index]*lumi/nevents
    print weight
    for event  in tree:
      for bin in range(0,int(event.step)+1):
        step_h[index].Fill(bin)
      if event.step==stepplot:
        # here we can put all plots after selection
        massh_h[index].Fill(event.recHMass)   
        massz_h[index].Fill(event.recZMass)   
          
        continue
    treefile.Close()

# now we can plot them
c1=TCanvas("c1","c1",800,600)
first=True
for index in range(0,len(mclist)):
    opt="same"
    if (first):
        first=False
        opt=""
    step_h[index].Draw(opt)

c2=TCanvas("c2","c2",800,600)
stackh_h = THStack("HmassS", "HmassS")
for index in range(0,len(mclist)):
    stackh_h.Add(massh_h[index])
stackh_h.Draw("nostack")

c3=TCanvas("c3","c3",800,600)
stackz_h = THStack("ZmassS", "ZmassS")
for index in range(0,len(mclist)):
    stackz_h.Add(massz_h[index])
stackz_h.Draw()

a=raw_input("hit a key to exit...")
