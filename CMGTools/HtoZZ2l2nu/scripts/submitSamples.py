#!/usr/bin/env python

import os,sys
import json

if(len(sys.argv)<2):
    print 'submitSamples.py samples.json scriptToRunOnBatch.sh'

from CMGTools.HtoZZ2l2nu.localPatTuples_cff import *

#open the file which describes the sample
samplesDB = sys.argv[1]
jsonFile = open(samplesDB,'r')
procList=json.load(jsonFile,encoding='utf-8').items()
scriptToRun=sys.argv[2]

#run over sample
fperjob=5
for proc in procList :

    #run over processes
    for desc in proc[1] :
        
        #run over items in process
        data = desc['data']
        for d in data :
            tag = d['dtag']
            files=getLocalSourceFor( tag )
            print tag
            print files
            if(files is None):continue
            nfiles=len(getLocalSourceFor( tag ) )
            njobs=nfiles/fperjob+1
            for ijob in range(njobs) :
                os.system('submit2batch.sh ' + scriptToRun + ' ' + tag + ' ' + str(ijob*fperjob) + ' ' + str(ijob*fperjob+fperjob))
