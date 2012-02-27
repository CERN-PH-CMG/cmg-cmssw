#!/usr/bin/env python

import os,sys
import json
import getopt
import commands
import ROOT
from ROOT import TFile

#get input file List
def checkCastorDirectory(dir, FilePrefix):
    if(dir.endswith('/')!=True):
       dir+='/'
    rfdir_cmd = "rfdir " + dir
    isEOS=False
    if(dir.find('/store/cmst3')==0) :
        isEOS=True
        splitOnString=','
        rfdir_cmd='cmsLs ' + dir + ' | grep root | awk \'{print $5}\''
    rfdir_cmd = rfdir_cmd + '| grep ' + dir+FilePrefix+"_"
#    print rfdir_cmd + "########"
    outCastorDir_out = commands.getstatusoutput(rfdir_cmd)
    return outCastorDir_out[1].split('\n')

#check rootFile exist and can be read
def checkInputFile(url):
    command_out = commands.getstatusoutput("root -l -b -q " + url)
    if(command_out[1].find("Error")>=0 or command_out[1].find("Warning")>=0):return False
    return True


#print usage
def usage() :
    print ' '
    print 'haddOverSamples.py [options]'
    print '  -j : json file containing the samples'
    print '  -d : sample input dir as described in the json'
    print ' - t : process only this tag'
    print '  -o : output directory'
    print '  -c : check input file validity'

    print ' '
    exit(-1)

#parse the options 
try:
     # retrive command line options
     shortopts  = "j:d:t:o:c:"
     opts, args = getopt.getopt( sys.argv[1:], shortopts )
except getopt.GetoptError:
     # print help information and exit:
     print "ERROR: unknown options in argument %s" % sys.argv[1:]
     usage()
     sys.exit(1)

subtoBatch=''
samplesDB=''
dirtag=''
outdir='.'
fperjob=-1
params=''
onlytag='all'
checkFile='True'
for o,a in opts:
    if o in("-?", "-h"):
        usage()
        sys.exit(0)
    elif o in('-o'): outdir = a
    elif o in('-j'): samplesDB = a
    elif o in('-d'): dirtag = a
    elif o in('-t'): onlytag = a
    elif o in('-c'): checkFile = a


jsonFile = open(samplesDB,'r')
procList=json.load(jsonFile,encoding='utf-8').items()
scriptFile=os.path.expandvars('${CMSSW_BASE}/bin/${SCRAM_ARCH}/wrapSubmitLocalRun.sh')
jobParamsList = params.split(' ')

from CMGTools.HtoZZ2l2nu.localPatTuples_cff import fillFromCastor

isOutputOnEOS = False
if(outdir.startswith('/store')==True):
	isOutputOnEOS = True

if(outdir.endswith('/')!=True):
	outdir+='/'

#run over sample
for proc in procList :
    #run over processes
    for desc in proc[1] :

        #run over items in process
        data = desc['data']
        for d in data :
            #tag veto
            if(onlytag!='all') :
                itag=d['dtag']
                if(itag.find(onlytag)<0) : continue

            print "-------------------------"
            allfiles = checkCastorDirectory(dirtag, d['dtag'] )
            if(len(allfiles)==0) : 
		print "no file found for sample: " + d['dtag']
		continue

            split = 1
            try:
                split = d['split']
            except:
		split = 1

            if(split>len(allfiles)):split = len(allfiles)

            NFilesToMerge = len(allfiles)//split
	    NFilesToMergeRemains = len(allfiles)%split
 
            startFile = 0
	    endFile = 0	    
            for s in range(0,split) :
		    outputFile = d['dtag']
		    if(split>1):
			outputFile += "_" + str(s)
                    outputFile += '.root '

	            haddCommand = 'hadd -f '
		    if(isOutputOnEOS==True):	haddCommand += '/tmp/' + outputFile
		    else:			haddCommand += outdir+outputFile
	            ifile=0
		
		    startFile = endFile
		    endFile   = endFile + NFilesToMerge
		    if(NFilesToMergeRemains>0):
		    	endFile+=1
			NFilesToMergeRemains-=1

	            for i in range(startFile,endFile):
			f = allfiles[i]
	                ifile=ifile+1
			if(f.startswith('/store')==True):
				f= 'root://eoscms//eos/cms'+f
			if(checkFile=='True' and checkInputFile(f)==False):
				print "missing file skipped " + f
				continue
                	haddCommand += f + " "
	
	            print haddCommand
		    commands.getstatusoutput(haddCommand)
		    if(isOutputOnEOS==True): 
			print 'cmsStageOut -f /tmp/'+outputFile + outdir+outputFile
			commands.getstatusoutput('cmsStageOut -f /tmp/'+outputFile + outdir+outputFile)


