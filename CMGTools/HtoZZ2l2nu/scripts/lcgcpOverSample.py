#!/usr/bin/env python

import os,sys
import json
import getopt
import commands

#print usage
def usage() :
    print ' '
    print 'haddOverSamples.py [options]'
    print '  -j : json file containing the samples'
    print '  -i : sample input dir'
    print '  -o : output directory'
    print ' - t : process only this tag'
    print ' '
    print 'typical example: "lcgcpOverSample.py -j data/samples.json -i /store/cmst3/user/psilva/grid_12_03_27_HZZ2l2v_ntuples -o /tmp/"'
    exit(-1)

#parse the options 
try:
     # retrive command line options
     shortopts  = "j:i:t:o:"
     opts, args = getopt.getopt( sys.argv[1:], shortopts )
except getopt.GetoptError:
     # print help information and exit:
     print "ERROR: unknown options in argument %s" % sys.argv[1:]
     usage()
     sys.exit(1)

samplesDB=''
inputdir=''
outdir='.'
onlytag='all'
for o,a in opts:
    if o in("-?", "-h"):
        usage()
        sys.exit(0)
    elif o in('-o'): outdir = a
    elif o in('-j'): samplesDB = a
    elif o in('-i'): inputdir = a
    elif o in('-t'): onlytag = a


jsonFile = open(samplesDB,'r')
procList=json.load(jsonFile,encoding='utf-8').items()

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

            split = 1
            try:	  split = d['split']
            except:  	  split = 1

            for s in range(0,split) :
		splitLine = '_'+str(s);
		if(split==1):splitLine='';
		if(inputdir.find('/store/cmst3')==0) : command = 'lcg-cp --verbose -D srmv2 "srm://srm-eoscms.cern.ch:8443/srm/v2/server?SFN=/eos/cms/' + inputdir + '/' + d['dtag']+splitLine+'.root" ' + '"file:'+ outdir+'/' + d['dtag']+'_'+str(s)+'.root"'
		else                                 : command = 'lcg-cp --verbose -D srmv2 "srm://srm-cms.cern.ch:8443/srm/managerv2?SFN=' + inputdir + '/' + d['dtag']+splitLine+'.root" ' + '"file:'+ outdir+'/' + d['dtag']+'_'+str(s)+'.root"'
		print command 
	        os.system(command)

