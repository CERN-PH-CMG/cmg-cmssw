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


command_out = commands.getstatusoutput("runLocalAnalysisOverSamples.py -e run2011Analysis -j $CMSSW_BASE/src/CMGTools/HtoZZ2l2nu/data/samples.json -o $CMSSW_BASE/src/CMGTools/HtoZZ2l2nu/test/results -d  /store/cmst3/user/querten/12_04_14_HZZ2l2v_ntuples -c $CMSSW_BASE/src/CMGTools/HtoZZ2l2nu/test/runAnalysis_cfg.py.templ -p \"@useMVA=False @saveSummaryTree=True @runSystematics=True\" -s 8nh")
JobList = getListOfJobIds(command_out[1])
#JobList = []
waitUntilJobDone(JobList)
os.system("runPlotter --iLumi 5035 --inDir results/ --outDir $CMSSW_BASE/src/CMGTools/HtoZZ2l2nu/test/results/plots2011/ --outFile $CMSSW_BASE/src/CMGTools/HtoZZ2l2nu/test/plotter2011.root  --json $CMSSW_BASE/src/CMGTools/HtoZZ2l2nu/data/samples.json")
os.system('echo "run2011Analysis done" | mail -s "UNIXMAIL: run2011Analysis done" loic.quertenmont@gmail.com')
