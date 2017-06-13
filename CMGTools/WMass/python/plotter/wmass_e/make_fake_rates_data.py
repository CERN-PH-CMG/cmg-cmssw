#!/usr/bin/env python
from shutil import copyfile
import re, sys, os, os.path, subprocess

T='/data1/emanuele/wmass/TREES_1LEP_53X_V3_FRELSKIM_V2'
if 'pccmsrm29' in os.environ['HOSTNAME']: T = T.replace('/data1/emanuele/wmass','/u2/emanuele')
elif 'lxplus' in os.environ['HOSTNAME']: T = T.replace('/data1/emanuele/wmass','/afs/cern.ch/work/e/emanuele/TREES/')
elif 'cmsrm-an' in os.environ['HOSTNAME']: T = T.replace('/data1/emanuele/wmass','/t3/users/dimarcoe/')
print "used trees from: ",T
J=4
BASECONFIG="wmass_e"
MCA=BASECONFIG+'/mca-qcd1l.txt'
CUTFILE=BASECONFIG+'/qcd1l.txt'
XVAR="pt_coarse"
FITVAR="mt"
NUM="FullSel"

BARREL="00_15"; ENDCAP="15_25"; ETA="1.479";

from optparse import OptionParser
parser = OptionParser(usage="%prog testname ")
(options, args) = parser.parse_args()

OPTIONS = MCA+" "+CUTFILE+" -f -P "+T+" --s2v -j "+str(J)+" -l 19.7 "
OPTIONS += ' -F mjvars/t "'+T+'/friends/evVarFriend_{cname}.root" '

PBASE = "plots/fake-rate/el/"
EWKSPLIT="-p 'W,Z,data'"
MCEFF="  python wmass_e/dataFakeRate.py "+ OPTIONS + " " + EWKSPLIT + " --groupBy cut wmass_e/make_fake_rates_sels.txt wmass_e/make_fake_rates_xvars.txt  "
MCEFF += "--sP "+NUM+" --sP "+XVAR+"  --sP "+FITVAR+" "+FITVAR+"  --ytitle 'Fake rate' "
MCEFF += " --fixRatioRange --maxRatioRange 0.7 1.29 " # ratio for other plots
LEGEND=" --legend=TL --fontsize 0.05 --legendWidth 0.4"
RANGES=" --showRatio  --ratioRange 0.00 2.99 "
RANGES+=" --yrange 0 0.25  --xcut 30 100 "
STACK="python wmass_e/stack_fake_rates_data.py "
MCEFF += (LEGEND+RANGES)

print MCEFF+" -o "+PBASE+"/fr_sub_eta_"+BARREL+".root --bare -A onelep eta 'abs(LepGood_eta)<"+ETA+"'"
print MCEFF+" -o "+PBASE+"/fr_sub_eta_"+ENDCAP+".root --bare -A onelep eta 'abs(LepGood_eta)>"+ETA+"'"

MCGO=MCEFF + " --algo=fQCD "
print MCGO + "-i " + PBASE + "/fr_sub_eta_"+BARREL+".root -o "+PBASE+"/fr_sub_eta_"+BARREL+"_fQCD.root" 
print MCGO + "-i " + PBASE + "/fr_sub_eta_"+ENDCAP+".root -o "+PBASE+"/fr_sub_eta_"+ENDCAP+"_fQCD.root" 
