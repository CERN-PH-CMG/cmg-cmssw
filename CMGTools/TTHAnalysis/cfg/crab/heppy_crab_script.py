#!/usr/bin/env python
import os

import ROOT
from DataFormats.FWLite import *
import sys
import re


dataset = ""
total = 0  # total number of jobs for given dataset, not used at the moment
nevents = None # this means run all events
nprint  = 0 # quiet printout, change if you want to print the first nprint events
useAAA = True # use xrootd by default
cfgfile=""
_filestounpack=""
filestounpack=[]

def XrootdRedirector():
    americas     = ["CO", "MX","US"]
    oldcontinent = ["AT", "BE", "CH", "DE", "EE", "ES", "FR", "GR", "HU", "IT", "RU", "UK"]
    region = os.environ["GLIDEIN_CMSSite"].split("_")[1] if "GLIDEIN_CMSSite" in os.environ else ""
    return  "xrootd-cms.infn.it/" if region in oldcontinent else "cmsxrootd.fnal.gov/" if region in americas else "cms-xrd-global.cern.ch/" 

# arguments of scriptExe
print "ARGV:",sys.argv
JobNumber=sys.argv[1] # 1st crab argument is jobID
job = int(JobNumber)
# if one wants to include more options to be passed to the crab scriptExe add a corresponding argument below
# crab only allows arguments of the type 'arg=value'
for arg in sys.argv[2:]:
    if arg.split("=")[0] == "dataset":  # this argument is strictly necessary
        dataset = arg.split("=")[1]
    elif arg.split("=")[0] == "total":
        total = int(arg.split("=")[1])
    elif arg.split("=")[0] == "nevents":
        nevents = int(arg.split("=")[1])
        print "selected to run over", nevents, "events"
    elif arg.split("=")[0] == "useAAA":
        useAAA = not (arg.split("=")[1] == 'False') # 'True' by default
        if useAAA: print "chosen to run via xrootd"
    elif arg.split("=")[0] == "cfgfile":
        cfgfile = arg.split("=")[1]
    elif arg.split("=")[0] == "filestounpack":
        _filestounpack = arg.split("=")[1]

print "dataset:", dataset
print "job", job , " out of", total

# fetch config file
import imp
import json
from PhysicsTools.HeppyCore.framework.heppy_loop import _heppyGlobalOptions
jfile = open ('options.json', 'r')
opts=json.loads(jfile.readline())
for k,v in opts.iteritems():
    _heppyGlobalOptions[k]=v
jfile.close()
handle = open(cfgfile, 'r')
cfo = imp.load_source(cfgfile.rstrip('py'), cfgfile, handle)
config = cfo.config
handle.close()

from PhysicsTools.HeppyCore.framework.heppy_loop import split
# pick right component from dataset and file from jobID
selectedComponents = []
for comp in config.components:
    if comp.name == dataset:
        # this selects the files and events and changes the name to _ChunkX according to fineSplitFactor and splitFactor
        newComp = split([comp])[job-1] # first job number is 1
        if useAAA:
            newComp.files = [x.replace("root://eoscms.cern.ch//eos/cms","root://" + XrootdRedirector()) for x in newComp.files]
        selectedComponents.append(newComp)

# check selectedComponents
if len(selectedComponents) == 0:
    print "No selected components found!!"
    print "   - dataset:", dataset
    print "   - components:", config.components
if len(selectedComponents)>1:
    print "More than one selected component:"
    from PhysicsTools.HeppyCore.framework.config import printComps
    printComps(selectedComponents)
else:
    print "Selected component:"
    print selectedComponents[0]
    print "files: ", selectedComponents[0].files

# set component to run
config.components = selectedComponents

# run!!!
from PhysicsTools.HeppyCore.framework.looper import Looper
looper = Looper( 'Output', config, nPrint = nprint, nEvents = nevents)
looper.loop()
looper.write()

#os.system("ls -lR") # for debugging

# assign the right name
if _filestounpack!="": filestounpack=_filestounpack.split(',')
for mytree in filestounpack:
    os.rename("Output/"+mytree, './'+mytree.replace('/','_'))
os.system("tar czf heppyOutput.tgz Output/")
os.system("touch dummyOutput.root")

# print in crab log file the content of the job log files, so one can see it from 'crab getlog'
print "-"*25
print "printing output txt files"
os.system('for i in Output/*.txt; do echo $i; cat $i; echo "---------"; done')

print "---> job successful <---"
