#! /usr/bin/env python
import os
import sys
import time


sequence = xrange(0,9)

for i in sequence :
#     #os.system("python submit.py rfio:/castor/cern.ch/user/m/mgouzevi/cmst3/2011_Analyses/DATA/HT/ProcessedTree_Combined_HT_%i.root data_HT   1nd")
     fcn="python submit.py rfio:/castor/cern.ch/cms/store/cmst3/user/mgouzevi/CMG/HT/Run2011A-May10ReReco-v1/AOD/ProcessedTree/ProcessedTree_Combined_%i.root data_HT_ReReco_%i   1nh" %(i,i)
#     fcn="python submitEventsMass.py rfio:/castor/cern.ch/cms/store/cmst3/user/mgouzevi/CMG/HT/Run2011A-May10ReReco-v1/AOD/ProcessedTree/ProcessedTree_Combined_%i.root data_HT_ReReco_%i   1nh" %(i,i)
     print fcn
     os.system(fcn)
     time.sleep(1)


sequence = xrange(0,5)
for i in sequence :
#     #os.system("python submit.py rfio:/castor/cern.ch/user/m/mgouzevi/cmst3/2011_Analyses/DATA/HT/ProcessedTree_Combined_HT_%i.root data_HT   1nd")
     fcn="python submit.py rfio:/castor/cern.ch/cms/store/cmst3/user/mgouzevi/CMG/HT/Run2011A-PromptReco-v4/AOD/ProcessedTree/ProcessedTree_Combined_%i.root data_HT_PromptReco_%i   1nh" %(i,i)
#     fcn="python submitEventsMass.py rfio:/castor/cern.ch/cms/store/cmst3/user/mgouzevi/CMG/HT/Run2011A-PromptReco-v4/AOD/ProcessedTree/ProcessedTree_Combined_%i.root data_HT_PromptReco_%i   1nh" %(i,i)
     print fcn
     os.system(fcn)
     time.sleep(1)
#os.system("python submit.py rfio:/castor/cern.ch/user/m/mgouzevi/cmst3/2011_Analyses/MC/120to170/ProcessedTree_mc.root    mc_120to170   1nd")
#time.sleep(3)

#os.system("python submit.py rfio:/castor/cern.ch/user/m/mgouzevi/cmst3/2011_Analyses/MC/170to300/ProcessedTree_mc.root    mc_170to300   1nd")
#time.sleep(3)

#os.system("python submit.py rfio:/castor/cern.ch/user/m/mgouzevi/cmst3/2011_Analyses/MC/300to470/ProcessedTree_mc.root    mc_300to470   1nd")
#time.sleep(3)

#os.system("python submit.py rfio:/castor/cern.ch/user/m/mgouzevi/cmst3/2011_Analyses/MC/470to600/ProcessedTree_mc.root    mc_470to600   1nd")
#time.sleep(3)

#os.system("python submit.py rfio:/castor/cern.ch/user/m/mgouzevi/cmst3/2011_Analyses/MC/600to800/ProcessedTree_mc.root    mc_600to800   1nd")
#time.sleep(3)

#os.system("python submit.py rfio:/castor/cern.ch/user/m/mgouzevi/cmst3/2011_Analyses/MC/800to1000/ProcessedTree_mc.root    mc_800to1000   1nd")
#time.sleep(3)

#os.system("python submit.py rfio:/castor/cern.ch/user/m/mgouzevi/cmst3/2011_Analyses/MC/1000to1400/ProcessedTree_mc.root    mc_1000to1400   1nd")
#time.sleep(3)

#os.system("python submit.py rfio:/castor/cern.ch/user/m/mgouzevi/cmst3/2011_Analyses/MC/1400to1800/ProcessedTree_mc.root    mc_1400to1800   1nd")
#time.sleep(3)

#os.system("python submit.py rfio:/castor/cern.ch/user/m/mgouzevi/cmst3/2011_Analyses/MC/1800toinf/ProcessedTree_mc.root    mc_1800toinf   1nd")




#os.system("python submit_JES.py rfio:/castor/cern.ch/user/m/mgouzevi/cmst3/2011_Analyses/DATA/HT/ProcessedTree_Combined_HT.root data_HT   1nd")
#time.sleep(3)

#os.system("python submit_JES.py rfio:/castor/cern.ch/user/m/mgouzevi/cmst3/2011_Analyses/MC/120to170/ProcessedTree_mc.root    mc_120to170   1nd")
#time.sleep(3)

#os.system("python submit_JES.py rfio:/castor/cern.ch/user/m/mgouzevi/cmst3/2011_Analyses/MC/170to300/ProcessedTree_mc.root    mc_170to300   1nd")
#time.sleep(3)

#os.system("python submit_JES.py rfio:/castor/cern.ch/user/m/mgouzevi/cmst3/2011_Analyses/MC/300to470/ProcessedTree_mc.root    mc_300to470   1nd")
#time.sleep(3)

#os.system("python submit_JES.py rfio:/castor/cern.ch/user/m/mgouzevi/cmst3/2011_Analyses/MC/470to600/ProcessedTree_mc.root    mc_470to600   1nd")
#time.sleep(3)

#os.system("python submit_JES.py rfio:/castor/cern.ch/user/m/mgouzevi/cmst3/2011_Analyses/MC/600to800/ProcessedTree_mc.root    mc_600to800   1nd")
#time.sleep(3)

#os.system("python submit_JES.py rfio:/castor/cern.ch/user/m/mgouzevi/cmst3/2011_Analyses/MC/800to1000/ProcessedTree_mc.root    mc_800to1000   1nd")
#time.sleep(3)

#os.system("python submit_JES.py rfio:/castor/cern.ch/user/m/mgouzevi/cmst3/2011_Analyses/MC/1000to1400/ProcessedTree_mc.root    mc_1000to1400   1nd")
#time.sleep(3)

#os.system("python submit_JES.py rfio:/castor/cern.ch/user/m/mgouzevi/cmst3/2011_Analyses/MC/1400to1800/ProcessedTree_mc.root    mc_1400to1800   1nd")
#time.sleep(3)

#os.system("python submit_JES.py rfio:/castor/cern.ch/user/m/mgouzevi/cmst3/2011_Analyses/MC/1800toinf/ProcessedTree_mc.root    mc_1800toinf   1nd")


