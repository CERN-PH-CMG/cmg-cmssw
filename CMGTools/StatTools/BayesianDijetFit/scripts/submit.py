#! /usr/bin/env python
import os
import sys
# set parameters to use cmst3 batch 
#######################################
### usage  cmst3_submit_manyfilesperjob.py dataset njobs applicationName queue 
#######################################
if len(sys.argv) != 6:
    print sys.argv
    print "usage ./submit.py mass statlevel nPEs Model queue"
    sys.exit(1)
mass = sys.argv[1]
statlevel = sys.argv[2]
nPEs = sys.argv[3]
Model = sys.argv[4]
queue = sys.argv[5]

#######################################
pwd = os.environ['PWD']
#######################################
out=str(mass)+"_"+str(statlevel)+ "_" + str(nPEs) + "_" +str(Model)

outputname = "out/src/submit_"+out+".src"
print outputname
outputfile = open(outputname,'w')
outputfile.write('#!/bin/bash\n')
outputfile.write("cd /afs/cern.ch/user/m/mgouzevi/scratch0/CMGTools/CMSSW_4_2_8/src/Analysis/Statistics/test; eval `scramv1 run -sh`\n")  # CHANGE WITH YOUR DIR TO DO CMSENV. LEAVE UNCHANGED THE REST!!!!
outputfile.write("dijetStatsNewBackground " + str(mass) + " " + str(statlevel) + " " + str(nPEs) + " " + str(Model) + " -b -q &>out/res/masslimit_" + out + ".txt")
outputfile.close
os.system("echo bsub -q "+queue+" -o out/log/log_"+ out  + ".log source "+pwd+"/"+outputname)
os.system("bsub -q "+queue+" -o out/log/log_"+ out  + ".log source "+pwd+"/"+outputname)

