#!/usr/bin/env python

import os,sys
import json

if(len(sys.argv)<2):
    print 'submitSamples.py samples.json cfgFile.py/script.sh <sampleTag=all> <runOnBatch=0>'
    exit(-1)

from CMGTools.HtoZZ2l2nu.localPatTuples_cff import *

#open the file which describes the sample
samplesDB = sys.argv[1]
jsonFile = open(samplesDB,'r')
procList=json.load(jsonFile,encoding='utf-8').items()
cfgFile=sys.argv[2]
sampleTag='all'
if(len(sys.argv)>3) :sampleTag=sys.argv[3]
runOnBatch=False
if(len(sys.argv)>4) : runOnBatch=int(sys.argv[4])

#run over sample
for proc in procList :

    #run over processes
    for desc in proc[1] :
        
        #run over items in process
        data = desc['data']
        for d in data :
            tag = d['dtag']
            if(sampleTag!="all" and tag.find(sampleTag)<0 ): continue
            files=getLocalSourceFor( tag )

            if(runOnBatch):
                fperjob=5
                if(files is None):continue
                nfiles=len(getLocalSourceFor( tag ) )
                njobs=nfiles/fperjob+1
                for ijob in range(njobs) :
                    os.system('submit2batch.sh ' + cfgFile + ' ' + tag + ' ' + str(ijob*fperjob) + ' ' + str(fperjob))
            else :
                os.system('cmsRun ' + cfgFile + ' ' + tag)
