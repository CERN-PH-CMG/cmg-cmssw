#!/usr/bin/env python

import sys 
import os 
import json
import getopt


#call crab from command line
def callCrab(opt,dir='') :
    cmd = 'crab '
    cmd += '-' + opt + ' '
    if( len(dir)>0 ) : cmd += '-c ' + dir
    os.system(cmd)

#print usage
def usage() :
    print ' '
    print 'sub2grid.py [options]'
    print '  -j : json file containing the samples'
    print '  -o : operation=create,submit,status,kill'
    print '  -c : crab file template'
    print '  -w : user directory in storage element'
    print '  -l : lumi mask (aka certified json file)'
    print ' '
    exit(-1)


#parse the options
try:
    # retrive command line options
    shortopts  = "j:o:c:l:w:h?"
    opts, args = getopt.getopt( sys.argv[1:], shortopts )
except getopt.GetoptError:
    # print help information and exit:
    print "ERROR: unknown options in argument %s" % sys.argv[1:]
    usage()
                        
workdir=""
crabtempl="crab.cfg.templ"
samplesDB=''
reqOp='status'
lumimask=''
for o,a in opts:
    if o in("-?", "-h"):
        usage()
        sys.exit(0)
    elif o in('-j'): samplesDB = a
    elif o in('-o'): reqOp = a
    elif o in('-c'): crabtempl = a
    elif o in('-w'): workdir = a + '/'
    elif o in('-l'): lumimask = a
    
jsonFile = open(samplesDB,'r')
procList=json.load(jsonFile,encoding='utf-8').items()

#run over sample
for proc in procList :

    #run over processes
    for desc in proc[1] :

        #run over items in process
        data = desc['data']
        isdata = desc['isdata']
        for d in data :
            dtag = d['dtag']
            dset = d['dset']
            pset=d['pset']
            
            print '\033[0;36m' + reqOp + '-ing \033[0m : ' + dtag + ' @ ' + dset

            #create the crab configuration and submit jobs
            if (reqOp == 'create' ) :
                cmd = 'sed \"'
                cmd += 's%@pset%' + pset +'%;'
                cmd += 's%@user_remote_dir%' + workdir+dtag +'%;'
                cmd += 's%@ui_working_dir%' + dtag +'%;'
                cmd += 's%@datasetpath%' + dset +'%;'
                if(not isdata) :
                    cmd += 's%@jobconfig1%events_per_job = 50000%;'
                    cmd += 's%@jobconfig2%%;'
                    cmd += 's%@jobconfig3%%;'
                else :
                    cmd += 's%@jobconfig1%lumi_mask = ' + lumimask + '%;'
                    cmd += 's%@jobconfig2%total_number_of_lumis   = -1%;'
                    cmd += 's%@jobconfig3%lumis_per_job           = 25%;'
                cmd += '\" '
                print cmd     
                os.system(cmd + '< ' + crabtempl + ' > crab.cfg')
                callCrab(reqOp)
            else :
                callCrab(reqOp,dtag)
