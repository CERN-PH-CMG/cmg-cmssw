#! /usr/bin/env python
import os
import sys

#run default
#os.system("root -b -q \'ReadTree_any.C(\"rfio:/castor/cern.ch/user/m/mgouzevi/cmst3/2011_Analyses/DATA/HT/ProcessedTree_Combined_HT.root\", \"data\")\' ");


#os.system("root -b -q \'ReadTree_any.C(\"rfio:/castor/cern.ch/user/m/mgouzevi/cmst3/2011_Analyses/MC/120to170/ProcessedTree_mc.root\", \"mc_120to170\")\' ");
#os.system("root -b -q \'ReadTree_any.C(\"rfio:/castor/cern.ch/user/m/mgouzevi/cmst3/2011_Analyses/MC/170to300/ProcessedTree_mc.root\", \"mc_170to300\")\' ");
#os.system("root -b -q \'ReadTree_any.C(\"rfio:/castor/cern.ch/user/m/mgouzevi/cmst3/2011_Analyses/MC/300to470/ProcessedTree_mc.root\", \"mc_300to470\")\' ");
#os.system("root -b -q \'ReadTree_any.C(\"rfio:/castor/cern.ch/user/m/mgouzevi/cmst3/2011_Analyses/MC/470to600/ProcessedTree_mc.root\", \"mc_470to600\")\' ");
#os.system("root -b -q \'ReadTree_any.C(\"rfio:/castor/cern.ch/user/m/mgouzevi/cmst3/2011_Analyses/MC/600to800/ProcessedTree_mc.root\", \"mc_600to800\")\' ");
#os.system("root -b -q \'ReadTree_any.C(\"rfio:/castor/cern.ch/user/m/mgouzevi/cmst3/2011_Analyses/MC/800to1000/ProcessedTree_mc.root\", \"mc_800to1000\")\'");
#os.system("root -b -q \'ReadTree_any.C(\"rfio:/castor/cern.ch/user/m/mgouzevi/cmst3/2011_Analyses/MC/1000to1400/ProcessedTree_mc.root\", \"mc_1000to1400\")\' ");
#os.system("root -b -q \'ReadTree_any.C(\"rfio:/castor/cern.ch/user/m/mgouzevi/cmst3/2011_Analyses/MC/1400to1800/ProcessedTree_mc.root\", \"mc_1400to1800\")\' ");
#os.system("root -b -q \'ReadTree_any.C(\"rfio:/castor/cern.ch/user/m/mgouzevi/cmst3/2011_Analyses/MC/1800toinf/ProcessedTree_mc.root\", \"mc_1800toinf\")\' ");

os.system("root -b -q \'ReadTree_JES.C(\"rfio:/castor/cern.ch/user/m/mgouzevi/cmst3/2011_Analyses/MC/170to300/ProcessedTree_mc.root\", \"mc_170to300\")\' ");
