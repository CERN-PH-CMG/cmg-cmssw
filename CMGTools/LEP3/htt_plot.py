#!/usr/bin/env python
import os,string,sys,commands,time,ConfigParser
from ROOT import *
from array import array

mclist=[
    "htt/Hig125_12/htttreeproducer_httanalyzer/htttreeproducer_httanalyzer_tree.root"
#    "zz.root",
#    "ww.root",
#    "qq.root"
    ]

# xsection of the processes in fb
xsec=[
    200.
#    1446.,
#    14080.,
#   16750.
    ]

# luminosity to normalize (in fb-1)
lumi=500

# step at which the plot should be made
stepplot=5

# define histograms
step_h=[]
for index in range(0,len(mclist)):
    step_h.append(TH1F("step","step",10,0,10))

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

a=raw_input("hit a key to exit...")
