#!/usr/bin/env python

import os,sys
import json
import getopt
import commands
import time

def getListOfJobIds(inputString):
	JobIds = []
	lines = inputString.split('\n')
	for line in lines:
		if(line.find("is submitted to queue")<0):continue
		JobId = line[line.find("<")+1:line.find(">")]
		JobIds.append(JobId)	
	return JobIds

def waitUntilJobDone(JobList):
        JobListStr = ""
        for job in JobList:
                JobListStr += job + " "
        os.system('echo ' + JobListStr + ' > JOBLIST')

        while True:
                NJobsRunning = commands.getstatusoutput("bjobs "+ JobListStr + "| grep 'RUN\|PEND' | wc")[1].split()[0]
                if(NJobsRunning.isdigit()==False or int(NJobsRunning)<=0):
                        break
                else:
                        print "They are some still " + NJobsRunning + " jobs running... wait"
                        time.sleep(30)


command_out11 = commands.getstatusoutput("runLocalAnalysisOverSamples.py -e run2011Analysis -j $CMSSW_BASE/src/CMGTools/HtoZZ2l2nu/data/samples_2011.json -o $CMSSW_BASE/src/CMGTools/HtoZZ2l2nu/test/results -d  /store/cmst3/user/psilva/28May2012_CMSSW444_HZZ2l2v_ntuples/ -c $CMSSW_BASE/src/CMGTools/HtoZZ2l2nu/test/runAnalysis_cfg.py.templ -p \"@useMVA=False @saveSummaryTree=True @runSystematics=True @automaticSwitch=False @is2011=True \" -s 8nh")
#command_out12 = commands.getstatusoutput("runLocalAnalysisOverSamples.py -e run2011Analysis -j $CMSSW_BASE/src/CMGTools/HtoZZ2l2nu/data/samples_2012Rescaled.json -o $CMSSW_BASE/src/CMGTools/HtoZZ2l2nu/test/resultsRescaled -d  /store/cmst3/user/psilva/28May2012_CMSSW444_HZZ2l2v_ntuples/ -c $CMSSW_BASE/src/CMGTools/HtoZZ2l2nu/test/runAnalysis_cfg.py.templ -p \"@useMVA=False @saveSummaryTree=True @runSystematics=True @automaticSwitch=False @is2011=False \" -s 8nh")
#JobList = getListOfJobIds(command_out11[1]+command_out12[1])
JobList = getListOfJobIds(command_out11[1])
waitUntilJobDone(JobList)
os.system("runPlotter --iEcm 7 --iLumi 5035 --inDir results/ --outDir $CMSSW_BASE/src/CMGTools/HtoZZ2l2nu/test/results/plots2011/ --outFile $CMSSW_BASE/src/CMGTools/HtoZZ2l2nu/test/plotter2011.root  --json $CMSSW_BASE/src/CMGTools/HtoZZ2l2nu/data/samples_2011.json")
#os.system("runPlotter --iEcm 8 --iLumi 1620 --inDir resultsRescaled/ --outDir $CMSSW_BASE/src/CMGTools/HtoZZ2l2nu/test/resultsRescaled/plots2012/ --outFile $CMSSW_BASE/src/CMGTools/HtoZZ2l2nu/test/plotter2012Rescaled.root  --json $CMSSW_BASE/src/CMGTools/HtoZZ2l2nu/data/samples_2012Rescaled.json")
#os.system("runPlotter --iEcm 8 --iLumi 5000 --inDir resultsRescaled/ --outDir $CMSSW_BASE/src/CMGTools/HtoZZ2l2nu/test/resultsRescaled/plots2012/ --outFile $CMSSW_BASE/src/CMGTools/HtoZZ2l2nu/test/plotter2012Rescaled.root  --json $CMSSW_BASE/src/CMGTools/HtoZZ2l2nu/data/samples_2012Rescaled.json --noPlot")
os.system('echo "run2011/12Analysis done" | mail -s "UNIXMAIL: run2011/12Analysis done" loic.quertenmont@gmail.com')

