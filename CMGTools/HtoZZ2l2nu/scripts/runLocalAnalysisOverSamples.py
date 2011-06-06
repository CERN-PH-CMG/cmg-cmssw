#!/usr/bin/env python
import os,sys
import json
import ROOT
                
if(len(sys.argv)<5):
    print 'runLocalAnalysisOverSamples.py executable samples.json inputdir outdir cfg_file [lumi=1] [submitToBatch=0]'
    exit(-1)

theExecutable=sys.argv[1]

#open the file which describes the sample
samplesDB = sys.argv[2]
jsonFile = open(samplesDB,'r')
procList=json.load(jsonFile,encoding='utf-8').items()

#configure
inputdir=sys.argv[3]
outdir=sys.argv[4]
cfg_file=sys.argv[5]
lumi=1
submitToBatch=False
if(len(sys.argv)>6) :
    lumi=int(sys.argv[6])
    if(len(sys.argv)>7) :
        submitToBatch=bool(int(sys.argv[7]))
    

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
            sedcmd = 'sed \"s%@input%' + eventsFile +'%;s%@outdir%' + outdir +'%;s%@isMC%' + str(not isdata) + '%;\"'
            cfgfile=outdir +'/'+ dtag + '_cfg.py'
            os.system('cat ' + cfg_file + ' | ' + sedcmd + ' > ' + cfgfile)
            if(not submitToBatch) :
                os.system(theExecutable + ' ' + cfgfile)
                #                os.system('rm localcfg.py')
            else :
                os.system('submit2batch.sh ${CMSSW_BASE}/bin/${SCRAM_ARCH}/wrapLocalAnalysisRun.sh ' + theExecutable + ' ' + cfgfile)
    
#run plotter over results
if(not submitToBatch) :
    os.system('mkdir -p ' + outdir + '/plots')
    os.system('runPlotterOverSamples.py ' + samplesDB + ' ' + str(lumi) + ' ' + outdir + ' ' + outdir + '/plots localAnalysis')
