#!/usr/bin/env python

import os,sys
import json
import getopt
import commands

#check castor directory for duplicates
def checkCastorDirectory(outdir):
    rfdir_cmd = "rfdir " + outdir
    isEOS=False
    if(outdir.find('/store/cmst3')==0) :
        isEOS=True
        splitOnString=','
        rfdir_cmd='cmsLs ' + dir + ' | grep root | awk \'{print $5}\''
    nOutFile = 0

    outCastorDir_out = commands.getstatusoutput(rfdir_cmd)
    jobNumbers = []
    duplicatedJobs = []
    origFiles=[]
    duplicatedFiles=[]
    if outCastorDir_out[0] == 0:
        castorLines = outCastorDir_out[1].split("\n")
        if len(castorLines) != 0:
            for castorFileLine in castorLines:
                fileName=castorFileLine
                if "root" in castorFileLine:

                    if(not isEOS) : fileName = castorFileLine.split()[8]

                    jobNumber=-1
                    try:
                        fileBaseName=os.path.basename(fileName)
                        jobNumber=int(fileBaseName.split("_")[1])
                    except:
                        continue

                    if jobNumber in jobNumbers:
                        if not jobNumber in duplicatedJobs:  duplicatedJobs.append(jobNumber)
                        duplicatedFiles.append(fileName)
                    else :
                        jobNumbers.append(jobNumber)
                        origFiles.append(fileName)
                        nOutFile += 1
                        
    print '   - Found ' + str(len(duplicatedJobs)) + ' job id duplicates @ ' + outdir
    print '   - Removing ' + str(len(duplicatedFiles)) + ' files'

    for f in duplicatedFiles :
        if(isEOS) : commands.getstatusoutput('cmsRm ' + f)
        else : commands.getstatusoutput('rfrm ' +outdir + '/' + f)



#print usage
def usage() :
    print ' '
    print 'runOverSamples.py [options]'
    print '  -s : submit or not to batch'
    print '  -R : host requirement when submitting to LSG'
    print '  -j : json file containing the samples'
    print '  -d : sample input dir as described in the json'
    print '  -n : files to process per job'
    print '  -p : parameters to pass to the job'
    print ' - t : process only this tag'
    print ' '
    exit(-1)

#parse the options 
try:
     # retrive command line options
     shortopts  = "s:j:d:n:p:t:R:h?"
     opts, args = getopt.getopt( sys.argv[1:], shortopts )
except getopt.GetoptError:
     # print help information and exit:
     print "ERROR: unknown options in argument %s" % sys.argv[1:]
     usage()
     sys.exit(1)

subtoBatch=''
requirementtoBatch='type==SLC5_64&&pool>30000'
samplesDB=''
dirtag=''
fperjob=-1
params=''
onlytag='all'
for o,a in opts:
    if o in("-?", "-h"):
        usage()
        sys.exit(0)
    elif o in('-s'): subtoBatch=a
    elif o in('-R'): requirementtoBatch=requirementtoBatch + '&&' + a.replace("'","").replace('"','')
    elif o in('-j'): samplesDB = a
    elif o in('-d'): dirtag = a
    elif o in('-t'): onlytag = a
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

            alldirs=[]
            try :
                alldirs = d[dirtag]
            except:
                continue
            
            #tag veto
            if(onlytag!='all') :
                itag=d['dtag']
                if(itag.find(onlytag)<0) : continue
                
            idir=0
            for dir in alldirs:
                idir=idir+1
                #checkCastorDirectory(dir)
                filenames=fillFromCastor(dir,0,-1,False)
                nfiles=len(filenames)

                njobs=1
		sleep=0;
                if(fperjob>0) : 
			njobs=nfiles/fperjob+1
			sleep=2*fperjob/6;
                
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
                            newParams += '-castor=' + d[arg][0] + ' '

                #submit the jobs
                for ijob in range(njobs) :
                    localParams = newParams + ' -src=' + dir + ' -tag=' + d['dtag']
		    if(idir>1) : localParams += '_' + str(idir)
                    if(fperjob>0) : localParams += ' -f=' + str(ijob*fperjob) + ' -step=' + str(fperjob)
                    print "**** Starting new job with the following parameters ****"
                    print localParams
                    if(len(subtoBatch)>0) :
                        print('submit2batch.sh -q' + subtoBatch + ' -R"' + requirementtoBatch + '" -J' + d['dtag']+str(ijob) + ' ' + scriptFile + ' ' + localParams)
                        os.system('submit2batch.sh -q' + subtoBatch + ' -R"' + requirementtoBatch + '" -J' + d['dtag']+str(ijob) + ' ' + scriptFile + ' ' + localParams)
			#os.system('sleep ' + str(sleep) + 's')   ##THIS WAS NEEDED ONLY FOR FILE ON CASTOR
                    else :
                        os.system(scriptFile + ' '  + localParams)

