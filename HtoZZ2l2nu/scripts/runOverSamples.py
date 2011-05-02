#!/usr/bin/env python

import os,sys
import json

if(len(sys.argv)<4):
    print 'runOverSamples.py subToBatch samples.json dirtag filesperjob [parameters]'
    exit(-1)

#open the file which describes the sample
subToBatch=int(sys.argv[1])
samplesDB = sys.argv[2]
jsonFile = open(samplesDB,'r')
procList=json.load(jsonFile,encoding='utf-8').items()
dirtag=sys.argv[3]
fperjob=int(sys.argv[4])
scriptFile=os.path.expandvars('${CMSSW_BASE}/bin/${SCRAM_ARCH}/wrapSubmitLocalRun.sh')

params=''
if(len(sys.argv)>4) :
    for i in xrange(5,len(sys.argv)) :
        params += sys.argv[i] + ' '

from CMGTools.HtoZZ2l2nu.localPatTuples_cff import fillFromCastor

#run over sample
for proc in procList :

    #run over processes
    for desc in proc[1] :
        
        #run over items in process
        data = desc['data']
        for d in data :
            dir = d[dirtag]
            filenames = fillFromCastor(dir)
            nfiles=len(filenames)
            njobs=1
            if(fperjob>0) : njobs=nfiles/fperjob+1

            print "*****"
            for ijob in range(njobs) :
                newparams = params + ' -src=' + dir 
                if(fperjob>0) : newparams += ' -f=' + str(ijob*fperjob) + ' -step=' + str(fperjob)
                if(subToBatch==0) :
                    os.system(scriptFile + ' '  + newparams)
                else :
                    os.system('submit2batch.sh ' + scriptFile + ' ' + newparams)
