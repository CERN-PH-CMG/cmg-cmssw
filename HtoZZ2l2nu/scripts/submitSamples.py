#!/usr/bin/env python

import os,sys
import json

if(len(sys.argv)<3):
    print 'submitSamples.py samples.json runOnBatch cfgFile.py/script.sh <parameters>'
    exit(-1)

#open the file which describes the sample
samplesDB = sys.argv[1]
jsonFile = open(samplesDB,'r')
procList=json.load(jsonFile,encoding='utf-8').items()
runOnBatch=int(sys.argv[2])
scriptFile=sys.argv[3]
params=''
if(len(sys.argv)>3) :
    for i in xrange(4,len(sys.argv)) :
        params += sys.argv[i] + ' '

#run over sample
for proc in procList :

    #run over processes
    for desc in proc[1] :
        
        #run over items in process
        data = desc['data']
        for d in data :
            tag = d['dtag']
            if(runOnBatch):
                os.system('submit2batch.sh ' + scriptFile + ' ' + tag + ' ' + params)
            else :2
                os.system('cmsRun ' + scriptFile + ' ' + tag + ' ' + params)
