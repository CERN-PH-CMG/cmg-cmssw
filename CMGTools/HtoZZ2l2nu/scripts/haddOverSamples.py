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

    print ' '
    exit(-1)

#parse the options 
try:
     # retrive command line options
     shortopts  = "j:d:t:o:"
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
for o,a in opts:
    if o in("-?", "-h"):
        usage()
        sys.exit(0)
    elif o in('-o'): outdir = a
    elif o in('-j'): samplesDB = a
    elif o in('-d'): dirtag = a
    elif o in('-t'): onlytag = a


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

#            haddCommand = 'hadd -f ' + outdir+d['dtag']+'.root '
            haddCommand = 'hadd -f '
	    if(isOutputOnEOS==True):	haddCommand += '/tmp/HADD_MERGE.root '
	    else:			haddCommand += outdir+d['dtag']+'.root '
            ifile=0
            for f in allfiles:
                ifile=ifile+1
		if(f.startswith('/store')==True):
			f= 'root://eoscms//eos/cms'+f
		if(checkInputFile(f)==False):
			print "missing file skipped " + f
			continue
                haddCommand += f + " "
	
            print haddCommand
	    commands.getstatusoutput(haddCommand)
	    if(isOutputOnEOS==True): 
		print 'cmsStageOut -f /tmp/HADD_MERGE.root ' + outdir+d['dtag']+'.root '
		commands.getstatusoutput('cmsStageOut -f /tmp/HADD_MERGE.root ' + outdir+d['dtag']+'.root ')


