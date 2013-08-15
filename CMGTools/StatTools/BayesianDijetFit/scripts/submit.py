#! /usr/bin/env python
import os
import sys
# set parameters to use cmst3 batch 
#######################################
### usage  cmst3_submit_manyfilesperjob.py dataset njobs applicationName queue 
#######################################
print len(sys.argv)

if len(sys.argv) != 7:
    print sys.argv
    print "usage ./submit.py mass statlevel nPEs Model queue 1"
    sys.exit(1)
mass = sys.argv[1]
statlevel = sys.argv[2]
nPEs = sys.argv[3]
Model = sys.argv[4]
verbose = sys.argv[5]
queue = sys.argv[6]

print 'mass = ',mass,' statlevel = ',statlevel,' nPEs = ',nPEs,' Model = ',Model,'; verbose = ',verbose,'; queue = ',queue

#######################################
pwd = os.environ['PWD']
#######################################
out=str(mass)+"_"+str(statlevel)+ "_" + str(nPEs) + "_" +str(Model)

outputname = "out/src/submit_"+out+".src"
print outputname
outputfile = open(outputname,'w')
outputfile.write('#!/bin/bash\n')
outputfile.write("cd ${CMSSW_BASE}/src/StatTools/BayesianDijetFit/test; eval `scramv1 run -sh`\n")  # CHANGE WITH YOUR DIR TO DO CMSENV. LEAVE UNCHANGED THE REST!!!!
outputfile.write("dijetStatsNewBackground " + str(mass) + " " + str(statlevel) + " " + str(nPEs) + " " + str(Model) + " " + str(verbose) + " -b -q &>out/res/masslimit_" + out + "_full.txt\n")
outputfile.write("sed '/INFO/d' out/res/masslimit_" + out + "_full.txt | sed '/RooDataHist/d' &>out/res/masslimit_" + out + ".txt\n")
outputfile.write("rm out/res/masslimit_" + out + "_full.txt\n")
outputfile.write("tar -cjf  out/res/masslimit_" + out + ".txt.tar.gz out/res/masslimit_" + out + ".txt\n")
outputfile.write("rm out/res/masslimit_" + out + ".txt\n")
outputfile.close
os.system("echo bsub -q "+queue+" -o out/log/log_"+ out  + ".log source "+pwd+"/"+outputname)
os.system("bsub -q "+queue+" -o out/log/log_"+ out  + ".log source "+pwd+"/"+outputname)

