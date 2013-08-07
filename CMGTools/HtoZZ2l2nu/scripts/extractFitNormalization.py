#!/usr/bin/env python

import os,sys
import ROOT
from ROOT import TFile 

print sys.argv
file = ROOT.TFile(sys.argv[1])
if(file is None): sys.exit(1)
if(file.IsZombie()) : sys.exit(1) 

file2=None
if(len(sys.argv)==3):
   file2=ROOT.TFile(sys.argv[2])
   print "%-30s & %-30s & %7s & %7s & %7s \\\\\\hline" % ("proc", "channel", "pred", "fit b", "fit b+s" )   
else:
   print "%-30s & %-30s & %7s & %7s \\\\\\hline" % ("proc", "channel", "fit b", "fit b+s" )  

nfb = file.Get("norm_fit_b"); #RooArgSet
nfs = file.Get("norm_fit_s");
itb = nfb.createIterator() #TIterator

list = []
for i in range(nfb.getSize()):
   valb = itb.Next()
   vals = nfs.find(valb.GetName())
   name = valb.GetName();
   channel = name.split("_")[3]
   channel = channel[3:len(channel)]
   proc = name.split("_")[5]
   if(len(name.split("_"))>6): proc=proc+"_"+name.split("_")[6]
   
   if(file2 is None):
      list.append( "%-30s & %-30s & $%7.3f$ & $%7.3f$ \\\\" % (proc, channel, valb.getVal(), vals.getVal() ) )
   else:
      h = file2.Get(channel+"/"+proc)
      hstat = file2.Get(channel+"/"+proc+"_CMS_hzz2l2v_stat_"+channel+"_"+proc+"_7TeVUp");
      if(hstat==None):hstat = file2.Get(channel+"/"+proc+"_CMS_hzz2l2v_stat_"+channel+"_"+proc+"_8TeVUp");
      statErr = hstat.Integral();
      statErr = statErr -h.Integral();
      systErr = 0.0;
      if(proc=="topwwwjetsdata"):systErr=0.25;
      if(proc=="zlldata"):systErr=1.00;
      systErr=systErr*h.Integral();

      channel = channel.replace("mumu","\\mu\\mu")
      channel = channel.replace("eq0jets","- 0jet")
      channel = channel.replace("eq1jets","- 1jet")
      channel = channel.replace("geq2jets","- \\geq 2jets")
      channel = channel.replace("vbf","- vbf")

      proc = proc.replace("topwwwjetsdata","NonResonant");
      proc = proc.replace("zlldata","Z+Jets");

      if(systErr>0):
         list.append( "%-30s & $%-30s$ & $%7.3f\pm%7.3f\pm%7.3f$ & $%7.3f$ & $%7.3f$ \\\\" % (proc, channel, h.Integral(), statErr, systErr, valb.getVal(), vals.getVal() ) )
      else:
         list.append( "%-30s & $%-30s$ & $%7.3f\pm%7.3f   %7.0s$ & $%7.3f$ & $%7.3f$ \\\\" % (proc, channel, h.Integral(), statErr, "", valb.getVal(), vals.getVal() ) )

list.sort()
for l in list:
   print(l)
