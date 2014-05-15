#!/usr/local/bin/python

import urllib, urlparse, string, time, os, shutil, sys, subprocess
from subprocess import Popen, PIPE

# scale_variations=[(0,0),(1,0),(-1,0),(0,1),(0,-1)]
scale_variations=[(0,0)]
# scale_variations=[(1,0)]


# infile_run='launch_analysis.py'
infile_run='launch_analysis_testSplit.py'


for (resol,scale) in scale_variations:
    
    os.system("cp launch_analysis_testSplit_bkp.py "+infile_run);
    ###################################
    print('## RUN W AND Z ANALYSES ##')
    ###################################

    os.system("sed -i 's/.*RecoilCorrResolutionNSigma =.*/RecoilCorrResolutionNSigma = \""+str(resol)+"\";/' "+infile_run)
    os.system("sed -i 's/.*RecoilCorrScaleNSigma =.*/RecoilCorrScaleNSigma = \""+str(scale)+"\";/' "+infile_run)
    os.system("grep RecoilCorrResolutionNSigma\ = "+infile_run)
    os.system("grep RecoilCorrScaleNSigma\ = "+infile_run)
    # sys.exit()
    os.system("sed -i 's/runWanalysis = 0;/runWanalysis = 1;/g' "+infile_run)
    os.system("sed -i 's/runZanalysis = 0;/runZanalysis = 1;/g' "+infile_run)
    os.system("sed -i 's/mergeSigEWKbkg = 1;/mergeSigEWKbkg = 0;/g' "+infile_run)
    os.system("sed -i 's/runR_WdivZ= 1;/runR_WdivZ= 0;/g' "+infile_run)
    os.system("sed -i 's/run_BuildSimpleTemplates= 1;/run_BuildSimpleTemplates= 0;/g' "+infile_run)
    os.system("sed -i 's/runPrepareDataCards = 1;/runPrepareDataCards = 0;/g' "+infile_run)
    os.system("sed -i 's/runClosureTestLikeLihoodRatioAnsMergeResults = 1;/runClosureTestLikeLihoodRatioAnsMergeResults = 0;/g' "+infile_run)
    execfile(infile_run)

    ##############################################################
    print('## CHECK IF RUN W AND Z ANALYSES ARE STILL RUNNING ##')
    ##############################################################
    still_running=1
    while still_running==1:
      line = os.popen('ps aux | grep perrozzi | grep analysis |grep root').read()
      if ("runWanalysis" not in line) and ("runZanalysis" not in line):
        still_running=0
      # if still_running==1: print 'runWanalysis or runZanalysis still running'
      # else: print 'runWanalysis and runZanalysis not running anymore'
      else: 
        os.system("sleep 10");
        # print 'runWanalysis or runZanalysis still running',line

    ###################################
    print('## MERGE EWK AND TT     ##')
    ###################################
    os.system("sed -i 's/runWanalysis = 1;/runWanalysis = 0;/g' "+infile_run)
    os.system("sed -i 's/runZanalysis = 1;/runZanalysis = 0;/g' "+infile_run)
    os.system("sed -i 's/mergeSigEWKbkg = 0;/mergeSigEWKbkg = 1;/g' "+infile_run)
    os.system("sed -i 's/runR_WdivZ= 1;/runR_WdivZ= 0;/g' "+infile_run)
    os.system("sed -i 's/run_BuildSimpleTemplates= 1;/run_BuildSimpleTemplates= 0;/g' "+infile_run)
    os.system("sed -i 's/runPrepareDataCards = 1;/runPrepareDataCards = 0;/g' "+infile_run)
    os.system("sed -i 's/runClosureTestLikeLihoodRatioAnsMergeResults = 1;/runClosureTestLikeLihoodRatioAnsMergeResults = 0;/g' "+infile_run)
    execfile(infile_run)
      
    ###################################
    print('## RUN R = W div Z      ##')
    ###################################
    os.system("sed -i 's/runWanalysis = 1;/runWanalysis = 0;/g' "+infile_run)
    os.system("sed -i 's/runZanalysis = 1;/runZanalysis = 0;/g' "+infile_run)
    os.system("sed -i 's/mergeSigEWKbkg = 1;/mergeSigEWKbkg = 0;/g' "+infile_run)
    os.system("sed -i 's/runR_WdivZ= 0;/runR_WdivZ= 1;/g' "+infile_run)
    os.system("sed -i 's/run_BuildSimpleTemplates= 1;/run_BuildSimpleTemplates= 0;/g' "+infile_run)
    os.system("sed -i 's/runPrepareDataCards = 1;/runPrepareDataCards = 0;/g' "+infile_run)
    os.system("sed -i 's/runClosureTestLikeLihoodRatioAnsMergeResults = 1;/runClosureTestLikeLihoodRatioAnsMergeResults = 0;/g' "+infile_run)
    execfile(infile_run)
      
    #########################################
    print('## RUN BUILD SIMPLE TEMPLATES ##')
    #########################################
    os.system("sed -i 's/runWanalysis = 1;/runWanalysis = 0;/g' "+infile_run)
    os.system("sed -i 's/runZanalysis = 1;/runZanalysis = 0;/g' "+infile_run)
    os.system("sed -i 's/mergeSigEWKbkg = 1;/mergeSigEWKbkg = 0;/g' "+infile_run)
    os.system("sed -i 's/runR_WdivZ= 1;/runR_WdivZ= 0;/g' "+infile_run)
    os.system("sed -i 's/run_BuildSimpleTemplates= 0;/run_BuildSimpleTemplates= 1;/g' "+infile_run)
    os.system("sed -i 's/runPrepareDataCards = 1;/runPrepareDataCards = 0;/g' "+infile_run)
    os.system("sed -i 's/runClosureTestLikeLihoodRatioAnsMergeResults = 1;/runClosureTestLikeLihoodRatioAnsMergeResults = 0;/g' "+infile_run)
    execfile(infile_run)

    ####################################
    print('## RUN PREPARE DATACARDS ##')
    ####################################
    os.system("sed -i 's/runWanalysis = 1;/runWanalysis = 0;/g' "+infile_run)
    os.system("sed -i 's/runZanalysis = 1;/runZanalysis = 0;/g' "+infile_run)
    os.system("sed -i 's/mergeSigEWKbkg = 1;/mergeSigEWKbkg = 0;/g' "+infile_run)
    os.system("sed -i 's/runR_WdivZ= 1;/runR_WdivZ= 0;/g' "+infile_run)
    os.system("sed -i 's/run_BuildSimpleTemplates= 1;/run_BuildSimpleTemplates= 0;/g' "+infile_run)
    os.system("sed -i 's/runPrepareDataCards = 0;/runPrepareDataCards = 1;/g' "+infile_run)
    os.system("sed -i 's/runClosureTestLikeLihoodRatioAnsMergeResults = 1;/runClosureTestLikeLihoodRatioAnsMergeResults = 0;/g' "+infile_run)
    execfile(infile_run)

    ###################################################
    print('## RUN LIKELIHOOD FIT AND MERGE RESULTS ##')
    ###################################################
    os.system("sed -i 's/runWanalysis = 1;/runWanalysis = 0;/g' "+infile_run)
    os.system("sed -i 's/runZanalysis = 1;/runZanalysis = 0;/g' "+infile_run)
    os.system("sed -i 's/mergeSigEWKbkg = 1;/mergeSigEWKbkg = 0;/g' "+infile_run)
    os.system("sed -i 's/runR_WdivZ= 1;/runR_WdivZ= 0;/g' "+infile_run)
    os.system("sed -i 's/run_BuildSimpleTemplates= 1;/run_BuildSimpleTemplates= 0;/g' "+infile_run)
    os.system("sed -i 's/runPrepareDataCards = 1;/runPrepareDataCards = 0;/g' "+infile_run)
    os.system("sed -i 's/runClosureTestLikeLihoodRatioAnsMergeResults = 0;/runClosureTestLikeLihoodRatioAnsMergeResults = 1;/g' "+infile_run)
    execfile(infile_run)
