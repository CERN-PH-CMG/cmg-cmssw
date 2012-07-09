#!/usr/bin/env python

import os,sys
#import json
import getopt
import commands
import ROOT

DataCardsDir7TeV='cards'
DataCardsDir8TeV='cards8TeV'
DataCardsDirComb='cardsComb'

SUBMASS = [200,225,250, 275,300,325,350,375, 400,425,450,475, 500,525,550,575, 600]

CWD = os.getcwd()
os.system('mkdir -p ' + DataCardsDirComb)
for m in SUBMASS:
   os.system('mkdir -p ' + DataCardsDirComb+"/"+str(m))
   os.system("cp " + DataCardsDir7TeV+"/"+str(m)+"/hzz2l2v_* " + DataCardsDirComb+"/"+str(m)+"/.")
   os.system("cp " + DataCardsDir8TeV+"/"+str(m)+"/hzz2l2v_* " + DataCardsDirComb+"/"+str(m)+"/.")
   os.chdir(CWD+"/"+DataCardsDirComb+"/"+str(m) )
   os.system("combineCards.py hzz*.dat > card_combined.dat")

   SCRIPT = open('script.sh',"w")
   SCRIPT.writelines('cd ' + os.environ.get('CMSSW_BASE') + ';\n')
   SCRIPT.writelines("export SCRAM_ARCH=slc5_amd64_gcc434;\n")
   SCRIPT.writelines("eval `scram r -sh`;\n")
   SCRIPT.writelines('cd ' + os.getcwd() + ';\n')
   #SCRIPT.writelines("combine -M Asymptotic -m " +  str(m) + " -t -1 card_combined.dat > COMB.log;\n")
   SCRIPT.writelines("combine -M Asymptotic -m " +  str(m) + " card_combined.dat > COMB.log;\n")
   SCRIPT.close()
   os.system("bsub -G u_zh -q 2nd -J "+str(m)+"limit 'sh " + os.getcwd()+"/script.sh'")
   os.chdir(CWD)
