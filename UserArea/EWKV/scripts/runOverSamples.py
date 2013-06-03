#!/usr/bin/env python

import os,sys
import json
import optparse
import commands
from UserCode.EWKV.storeTools_cff import fillFromStore

usage = 'usage: %prog [options]'
parser = optparse.OptionParser(usage)
parser.add_option('-s', '--sub'        ,    dest='queue'              , help='batch queue'                           , default='')
parser.add_option('-R', '--R'          ,    dest='requirementtoBatch' , help='requirement for batch queue'           , default='pool>30000')
parser.add_option('-j', '--json'       ,    dest='samplesDB'          , help='samples json file'                     , default='')
parser.add_option('-d', '--dir'        ,    dest='dirtag'             , help='input directory tag in json file'      , default='aoddir')
parser.add_option('-t', '--tag'        ,    dest='onlytag'            , help='process only samples matching this tag', default='all')
parser.add_option('-n', '--n'          ,    dest='fperjob'            , help='input files per job'                   , default=-1,  type=int)
parser.add_option('-p', '--pars'       ,    dest='params'             , help='extra parameters for the job'          , default='')
(opt, args) = parser.parse_args()


scriptFile=os.path.expandvars('${CMSSW_BASE}/bin/${SCRAM_ARCH}/wrapLocalAnalysisRun.sh')

jsonFile = open(opt.samplesDB,'r')
procList=json.load(jsonFile,encoding='utf-8').items()
jobParamsList = opt.params.split(' ')

#run over sample
for proc in procList :

    #run over processes
    for desc in proc[1] :
                
        #run over items in process
        data = desc['data']
        for d in data :

            alldirs=[]
            try :
                alldirs = d[opt.dirtag]
            except:
                continue

            #tag veto
            if(opt.onlytag!='all') :
                itag=d['dtag']
                if(itag.find(opt.onlytag)<0) : continue

            idir=0
            for dir in alldirs:
                idir=idir+1
                filenames=fillFromStore(dir,0,-1,False)
                nfiles=len(filenames)
                
                njobs=1
		sleep=0;
                if(opt.fperjob>0) : 
                    njobs=nfiles//opt.fperjob
                    if(nfiles%opt.fperjob>0):njobs = njobs+1
                    sleep=2*opt.fperjob/6
                
                #substitute some job parameters by json file parameters
                newParams=''
                for ipar in jobParamsList :
                    jobopt=ipar.split('=')[0]
                    jobarg=ipar.split('=')[1]
                    if(jobopt.find('-castor')<0):
                        newParams += ipar + ' '
                    else :
                        if(jobarg.find('/')>=0):
                            newParams += ipar + ' '
                        else :
                            newParams += '-castor=' + d[arg][0] + ' '

                #submit the jobs
                for ijob in range(njobs) :
                    localParams = '%s -src=%s -tag=%s'%(newParams,dir,d['dtag'])
		    if(idir>1) :        localParams += '_%d'%(idir)
                    if(opt.fperjob>0) : localParams += " -f=%d -step=%d"%(opt.fperjob*ijob,opt.fperjob)
                    print "**** Starting new job with the following parameters ****"
                    print localParams
                    if(len(opt.queue)>0) :
                        batchCommand='submit2batch.sh -q%s -R\"%s\" -J%s%d %s %s'%(opt.queue,opt.requirementtoBatch,d['dtag'],ijob,scriptFile,localParams)
                        print batchCommand
                        os.system(batchCommand)
                    else :
                        os.system(scriptFile + ' '  + localParams)
                        
