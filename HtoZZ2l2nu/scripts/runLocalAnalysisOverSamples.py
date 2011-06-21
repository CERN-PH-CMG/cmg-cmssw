#!/usr/bin/env python
import os,sys
import json
import ROOT
import getopt

#print usage
def usage() :
    print ' '
    print 'runLocalAnalysisOverSamples.py [options]'
    print '  -s : submit or not to batch'
    print '  -e : executable name'
    print '  -j : json file containing the samples'
    print '  -d : input dir with the event summaries'
    print '  -o : output directory'
    print '  -c : templated configuration file to steer the job'
    print '  -l : luminosity (pb)'
    print '  -p : extra parameters configure'
    print ' '
    exit(-1)


#parse the options 
try:
     # retrive command line options
     shortopts  = "s:e:j:d:o:c:l:p:h?"
     opts, args = getopt.getopt( sys.argv[1:], shortopts )
except getopt.GetoptError:
     # print help information and exit:
     print "ERROR: unknown options in argument %s" % sys.argv[1:]
     usage()
     sys.exit(1)

subtoBatch=False
samplesDB=''
theExecutable=''
inputdir=''
outdir=''
lumi=1
cfg_file=''
params=''
for o,a in opts:
    if o in("-?", "-h"):
        usage()
        sys.exit(0)
    elif o in('-s'): subtoBatch=True
    elif o in('-j'): samplesDB = a
    elif o in('-e'): theExecutable = a
    elif o in('-d'): inputdir = a
    elif o in('-o'): outdir = a
    elif o in('-l'): lumi=float(a)
    elif o in('-c'): cfg_file = a
    elif o in('-p'): params = a
                                        
#open the file which describes the sample
jsonFile = open(samplesDB,'r')
procList=json.load(jsonFile,encoding='utf-8').items()

#run over sample
for proc in procList :

    #run over processes
    for desc in proc[1] :

        #run over items in process
        isdata=desc['isdata']
        data = desc['data']
        for d in data :
            dtag = d['dtag']
            eventsFile=inputdir + '/' + dtag + '.root'
            sedcmd = 'sed \"s%@input%' + eventsFile +'%;s%@outdir%' + outdir +'%;s%@isMC%' + str(not isdata) + '%;'
            if(len(params)>0) :
                extracfgs=params.split(' ')
                for icfg in extracfgs :
                    varopt=icfg.split('=')
                    sedcmd += 's%' + varopt[0] + '%' + varopt[1] + '%;'
            sedcmd += '\"' 
            cfgfile=outdir +'/'+ dtag + '_cfg.py'
            os.system('cat ' + cfg_file + ' | ' + sedcmd + ' > ' + cfgfile)
            if(not subtoBatch) :
                os.system(theExecutable + ' ' + cfgfile)
            else :
                os.system('submit2batch.sh ${CMSSW_BASE}/bin/${SCRAM_ARCH}/wrapLocalAnalysisRun.sh ' + theExecutable + ' ' + cfgfile)
    
#run plotter over results
if(not subtoBatch) :
    os.system('mkdir -p ' + outdir + '/plots')
    os.system('runPlotterOverSamples.py -j ' + samplesDB + ' -l ' + str(lumi) + ' -i ' + outdir + ' -o ' + outdir + '/plots -d localAnalysis')
