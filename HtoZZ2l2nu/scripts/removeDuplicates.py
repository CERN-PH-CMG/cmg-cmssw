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
                        
    #print '   - Found ' + str(len(duplicatedJobs)) + ' job id duplicates @ ' + outdir
    #print '   - Removing ' + str(len(duplicatedFiles)) + ' files'

    #for f in duplicatedFiles :
        #if(isEOS) : commands.getstatusoutput('cmsRm ' + f)
        #else : commands.getstatusoutput('rfrm ' +outdir + '/' + f)
    return duplicatedFiles



#print usage
def usage() :
    print ' '
    print 'runOverSamples.py [options]'
    print '  -j : json file containing the samples'
    print '  -d : sample input dir as described in the json'
    print ' '
    exit(-1)

#parse the options 
try:
     # retrive command line options
     shortopts  = "j:d:h?"
     opts, args = getopt.getopt( sys.argv[1:], shortopts )
except getopt.GetoptError:
     # print help information and exit:
     print "ERROR: unknown options in argument %s" % sys.argv[1:]
     usage()
     sys.exit(1)

samplesDB=''
dirtag='aoddir'
for o,a in opts:
    if o in("-?", "-h"):
        usage()
        sys.exit(0)
    elif o in('-j'): samplesDB = a
    elif o in('-d'): dirtag = a


jsonFile = open(samplesDB,'r')
procList=json.load(jsonFile,encoding='utf-8').items()

from CMGTools.HtoZZ2l2nu.localPatTuples_cff import fillFromCastor

#run over sample
for proc in procList :

    #run over processes
    for desc in proc[1] :
                
        #run over items in process
        data = desc['data']
        for d in data :
	    dtag = d["dtag"]
            alldirs=[]
            try :
                alldirs = d[dirtag]
            except:
                continue
            
            idir=0
            for dir in alldirs:
	        print('#' + dtag + ' --> ' + dir + ' : ' )
                idir=idir+1
                duplicatedFiles=checkCastorDirectory(dir)
		for f in duplicatedFiles :
		    	if(f.find('/store/cmst3')==0) : print('cmsRm ' + f)
			else      		      : print('rfrm ' +outdir + '/' + f)
