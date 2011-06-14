#!/usr/bin/env python

import os,sys
import json
import getopt


#print usage
def usage() :
    print ' '
    print 'runOverSamples.py [options]'
    print '  -s : submit or not to batch'
    print '  -j : json file containing the samples'
    print '  -d : sample input dir as described in the json'
    print '  -n : files to process per job'
    print '  -p : parameters to pass to the job'
    print ' '
    exit(-1)

#parse the options 
try:
     # retrive command line options
     shortopts  = "s:j:d:n:p:h?"
     opts, args = getopt.getopt( sys.argv[1:], shortopts )
except getopt.GetoptError:
     # print help information and exit:
     print "ERROR: unknown options in argument %s" % sys.argv[1:]
     usage()
     sys.exit(1)

subtoBatch=False
samplesDB=''
dirtag=''
fperjob=-1
params=''
for o,a in opts:
    if o in("-?", "-h"):
        usage()
        sys.exit(0)
    elif o in('-s'): subtoBatch=True
    elif o in('-j'): samplesDB = a
    elif o in('-d'): dirtag = a
    elif o in('-n'): fperjob=int(a)
    elif o in('-p'): params = a

jsonFile = open(samplesDB,'r')
procList=json.load(jsonFile,encoding='utf-8').items()
scriptFile=os.path.expandvars('${CMSSW_BASE}/bin/${SCRAM_ARCH}/wrapSubmitLocalRun.sh')
jobParamsList = params.split(' ')

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

            #substitute some job parameters by json file parameters
            newParams=''
            for ipar in jobParamsList :
                opt=ipar.split('=')[0]
                arg=ipar.split('=')[1]
                if(opt.find('-castor')<0):
                    newParams += ipar + ' '
                else :
                    if(arg.find('/')>=0):
                        newParams += ipar + ' '
                    else :
                        newParams += '-castor=' + d[arg] + ' '
            
            #submit the jobs
            for ijob in range(njobs) :
                localParams = newParams + ' -src=' + dir + ' -tag=' + d['dtag']
                if(fperjob>0) : localParams += ' -f=' + str(ijob*fperjob) + ' -step=' + str(fperjob)
                if(subtoBatch) :
                    os.system('submit2batch.sh ' + scriptFile + ' ' + localParams)                   
                else :
                    os.system(scriptFile + ' '  + localParams)

