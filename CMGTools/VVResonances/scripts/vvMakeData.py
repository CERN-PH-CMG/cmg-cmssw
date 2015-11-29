#!/usr/bin/env python

import ROOT
from array import array
from CMGTools.VVResonances.plotting.TreePlotter import TreePlotter
from CMGTools.VVResonances.plotting.MergedPlotter import MergedPlotter
from math import log
import os, sys, re, optparse,pickle,shutil,json




parser = optparse.OptionParser()
parser.add_option("-s","--samples",dest="samples",default='',help="Type of sample")
parser.add_option("-c","--cut",dest="cut",help="Cut to apply for yield",default='')
parser.add_option("-o","--output",dest="output",help="Output ROOT",default='')
parser.add_option("-v","--vars",dest="vars",help="variables seprataed by comma",default='')
parser.add_option("-b","--bins",dest="bins",help="bins per dimension separated by comma",default='')
parser.add_option("-m","--min",dest="mins",help="minimum separated by comma",default='')
parser.add_option("-M","--max",dest="maxes",help="maximum separated by comma",default='')
parser.add_option("-d","--isData",dest="data",type=int,help="isData",default=1)
parser.add_option("-f","--factor",dest="factor",type=float,help="factor",default=1.0)


(options,args) = parser.parse_args()
#define output dictionary

samples={}



sampleTypes=options.samples.split(',')

dataPlotters=[]

for filename in os.listdir(args[0]):
    for sampleType in sampleTypes:
        if filename.find(sampleType)!=-1:
            fnameParts=filename.split('.')
            fname=fnameParts[0]
            ext=fnameParts[1]
            if ext.find("root") ==-1:
                continue
            dataPlotters.append(TreePlotter(args[0]+'/'+fname+'.root','tree'))
            if options.data==0:
                dataPlotters[-1].setupFromFile(args[0]+'/'+fname+'.pck')
                dataPlotters[-1].addCorrectionFactor('xsec','tree')
                dataPlotters[-1].addCorrectionFactor('genWeight','tree')
                dataPlotters[-1].addCorrectionFactor('puWeight','tree')
    

data=MergedPlotter(dataPlotters)

pvars=options.vars.split(',')
pmins=options.mins.split(',')
pmaxes=options.maxes.split(',')
pbins=options.bins.split(',')

if len(pvars)==1:
    histo=data.drawTH1(pvars[0],options.cut,"1",int(pbins[0]),float(pmins[0]),float(pmaxes[0]))

if len(pvars)==2:
    histo=data.drawTH2(pvars[1]+":"+pvars[0],options.cut,"1",int(pbins[0]),float(pmins[0]),float(pmaxes[0]),int(pbins[1]),float(pmins[1]),float(pmaxes[1]))

if len(pvars)==3:
    histo=data.drawTH3(pvars[2]+":"+pvars[1]+":"+pvars[0],options.cut,"1",int(pbins[0]),float(pmins[0]),float(pmaxes[0]),int(pbins[1]),float(pmins[1]),float(pmaxes[1]),int(pbins[2]),float(pmins[2]),float(pmaxes[2]))


histo.Scale(options.factor)


F=ROOT.TFile(options.output,"RECREATE")
F.cd()
histo.Write("histo")
F.Close()



