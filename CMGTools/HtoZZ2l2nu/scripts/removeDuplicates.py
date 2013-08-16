#!/usr/bin/env python

import os,sys
import json
import getopt
import commands

def checkInputFile(url):
    if(url.startswith('/store')==True):
       url= 'root://eoscms//eos/cms'+url
    command_out = commands.getstatusoutput("root -l -b -q " + url)
    if(command_out[1].find("Error")>=0 or command_out[1].find("probably not closed")>=0 or command_out[1].find("Corrupted")>=0):return False
    return True


#check castor directory for duplicates
def checkCastorDirectory(outdir):
    rfdir_cmd = "rfdir " + outdir
    isEOS=False
    if(outdir.find('/store/cmst3')==0) :
        isEOS=True
        splitOnString=','
        rfdir_cmd='cmsLs ' + outdir + ' | grep root | awk \'{print $5}\''	
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

		    if(checkInputFile(fileName)==True):

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
		    else:
			    print("   #corrupted file found : " + fileName)
			    duplicatedFiles.append(fileName)
    return duplicatedFiles
                        
#print usage
def usage() :
    print ' '
    print 'removeDuplicates.py [options]'
    print '  -d : sample input directory'
    print ' '
    exit(-1)

#parse the options 
try:
     # retrive command line options
     shortopts  = "d:h?"
     opts, args = getopt.getopt( sys.argv[1:], shortopts )
except getopt.GetoptError:
     # print help information and exit:
     print "ERROR: unknown options in argument %s" % sys.argv[1:]
     usage()
     sys.exit(1)

dirtag=''
for o,a in opts:
    if o in("-?", "-h"):
        usage()
        sys.exit(0)
    elif o in('-d'): dirtag = a

if(len(dirtag)==0):
    usage()
    sys.exit(0)

from CMGTools.HtoZZ2l2nu.localPatTuples_cff import fillFromCastor

duplicatedFiles=checkCastorDirectory(dirtag)
print 'Removing ' + str(len(duplicatedFiles)) + ' duplicated files in ' + dirtag

isEOS=False
if(dirtag.find('/store/cmst3')==0) : isEOS=True
for f in duplicatedFiles :
    print f
    if(isEOS) : commands.getstatusoutput('cmsRm ' + f)
    else : commands.getstatusoutput('rfrm ' +outdir + '/' + f)
        

