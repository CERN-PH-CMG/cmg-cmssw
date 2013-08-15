#! /usr/bin/env python
import os
import sys
# set parameters to use cmst3 batch 
#######################################
### usage  cmst3_submit_manyfilesperjob.py dataset njobs applicationName queue 
#######################################
if len(sys.argv) != 4:
    print sys.argv
    print "usage ./submit.py file outFile queue"
    sys.exit(1)
inFile = sys.argv[1]
outLabel = sys.argv[2]
queue = sys.argv[3]

# write on cmst3
os.system("export STAGE_HOST=castorcms; export STAGE_SVCCLASS=cmst3; rfmkdir out/res")
################################################
os.system("mkdir -p out/")
os.system("mkdir -p out/log/")
os.system("mkdir -p out/src/")
#######################################
pwd = os.environ['PWD']
#######################################
for ijob in range(1):
    outputname = "out/src/submit_"+str(ijob)+"_" + outLabel + ".src"
    outputfile = open(outputname,'w')
    outputfile.write('#!/bin/bash\n')
    outputfile.write('export STAGE_HOST=castorcms\n')
    outputfile.write('export STAGE_SVCCLASS=cmst3\n')
    outputfile.write("cd /afs/cern.ch/user/m/mgouzevi/scratch0/CMGAnalysis/TEST/CMGTools/CMSSW_4_1_4/src/CMGTools/DiJetHighMass/prod/macros_KK/ControlPlots_Chiyoung/Plots_HT_CaloFat_740_380pb; eval `scramv1 run -sh`\n")  # CHANGE WITH YOUR DIR TO DO CMSENV. LEAVE UNCHANGED THE REST!!!!
    outputfile.write('cd '+pwd+'\n')
    outputfile.write("root -b -q \'ReadTree_any.C(\"" + inFile + "\", \"" + outLabel + "\")\' \n")
    outputfile.write("rfcp histograms_" + outLabel + ".root /afs/cern.ch/user/m/mgouzevi/scratch0/CMGAnalysis/TEST/CMGTools/CMSSW_4_1_4/src/CMGTools/DiJetHighMass/prod/macros_KK/ControlPlots_Chiyoung/Plots_HT_CaloFat_740_380pb/out/res/histograms_" + outLabel + ".root \n")
    outputfile.close
    os.system("echo bsub -q "+queue+" -o out/log/log_"+str(ijob)+"_"+ outLabel + ".log source "+pwd+"/"+outputname)
    os.system("bsub -q "+queue+" -o out/log/log_"+str(ijob)+".log source "+pwd+"/"+outputname)
    ijob = ijob+1
    continue
