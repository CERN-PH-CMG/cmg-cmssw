#!/usr/bin/env python

import os,sys
#import json
import getopt
import commands
import ROOT

dirs  = []
dirs += ["cards_CC7TeV", "cards_CC7TeV_cp0.10_brn0.00", "cards_CC7TeV_cp0.20_brn0.00", "cards_CC7TeV_cp0.30_brn0.00", "cards_CC7TeV_cp0.40_brn0.00", "cards_CC7TeV_cp0.50_brn0.00", "cards_CC7TeV_cp0.60_brn0.00", "cards_CC7TeV_cp0.80_brn0.00", "cards_CC7TeV_cp1.00_brn0.00"]
dirs += ["cards_SB7TeV", "cards_SB7TeV_cp0.10_brn0.00", "cards_SB7TeV_cp0.20_brn0.00", "cards_SB7TeV_cp0.30_brn0.00", "cards_SB7TeV_cp0.40_brn0.00", "cards_SB7TeV_cp0.50_brn0.00", "cards_SB7TeV_cp0.60_brn0.00", "cards_SB7TeV_cp0.80_brn0.00", "cards_SB7TeV_cp1.00_brn0.00"]


#parse the options
try:
   # retrive command line options
   shortopts  = "p:f:m:i:s:j:o:h?"
   opts, args = getopt.getopt( sys.argv[1:], shortopts )
except getopt.GetoptError:
   # print help information and exit:
   print "ERROR: unknown options in argument %s" % sys.argv[1:]
   help()
   sys.exit(1)

phase = 4
for o,a in opts:
   if o in("-?", "-h"):
      sys.exit(1)
   elif o in('-p'): phase = int(a)

if( phase == 3 ):
   for dir in dirs:
      DataCardsDir7TeV=dir
      DataCardsDir8TeV=dir.replace("7TeV", "8TeV")
      DataCardsDirComb=dir.replace("7TeV", "Comb")

      #SUBMASS = [200, 202, 204, 206, 208, 210, 212, 214, 216, 218, 220, 222, 224, 226, 228, 230, 232, 234, 236, 238, 240, 242, 244, 246, 248, 250, 252, 254, 256, 258, 260, 262, 264, 266, 268, 270, 272, 274, 276, 278, 280, 282, 284, 286, 288, 290, 295, 300, 305, 310, 315, 320, 325, 330, 335, 340, 345, 350, 360, 370, 380, 390, 400, 420, 440, 460, 480, 500, 520, 540, 560, 580, 600, 650, 700, 750, 800, 850, 900, 950, 1000]
      SUBMASS = [200,250, 300,350,400,450,500,550,600, 650, 700, 750, 800, 850, 900, 950, 1000]


      CWD = os.getcwd()
      os.system('mkdir -p ' + DataCardsDirComb)
      for m in SUBMASS:
         os.system('mkdir -p ' + DataCardsDirComb+"/"+('%.0f' % float(m)))
         os.system("cp " + DataCardsDir7TeV+"/"+('%04.0f' % float(m))+"/hzz2l2v_* " + DataCardsDirComb+"/"+('%.0f' % float(m))+"/.")
         os.system("cp " + DataCardsDir8TeV+"/"+('%04.0f' % float(m))+"/hzz2l2v_* " + DataCardsDirComb+"/"+('%.0f' % float(m))+"/.")
         os.chdir(CWD+"/"+DataCardsDirComb+"/"+('%.0f' % float(m)) )
         os.system("combineCards.py hzz*.dat > card_combined.dat")

         SCRIPT = open('script.sh',"w")
         SCRIPT.writelines('cd ' + os.environ.get('CMSSW_BASE') + ';\n')
         SCRIPT.writelines("export SCRAM_ARCH="+os.getenv("SCRAM_ARCH","slc5_amd64_gcc434")+";\n")
         SCRIPT.writelines("eval `scram r -sh`;\n")
         SCRIPT.writelines('cd ' + os.getcwd() + ';\n')
         #SCRIPT.writelines("combine -M Asymptotic -m " +  str(m) + " -t -1 card_combined.dat > COMB.log;\n")
         SCRIPT.writelines("combine -M Asymptotic -m " +  str(m) + " card_combined.dat > COMB.log;\n")
         SCRIPT.close()
         os.system("bsub -G u_zh -q 8nh -J "+str(m)+"limit 'sh " + os.getcwd()+"/script.sh'")
         os.chdir(CWD)

elif( phase == 4 ):
   for dir in dirs:
      DataCardsDirComb=dir.replace("7TeV", "Comb")
      os.system("hadd -f "+DataCardsDirComb+"/LimitTree.root "+DataCardsDirComb+"/???/higgsCombineTest.Asymptotic.*.root " + DataCardsDirComb+"/????/higgsCombineTest.Asymptotic.*.root > /dev/null")
      os.system("root -l -b -q plotLimit.C+'(\""+DataCardsDirComb+"/Stength_\",\""+DataCardsDirComb+"/LimitTree.root\",\"\",  true, false, 9 , 24,735 )'")


