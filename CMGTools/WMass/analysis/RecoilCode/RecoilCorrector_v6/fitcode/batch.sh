#!/bin/bash

myJobN=$1
rap=1

myDIR=/afs/cern.ch/work/d/dalfonso/CMSSW_5_3_22_patch1_Wmass_aug5/src/CMGTools/WMass/analysis/RecoilCode/RecoilCorrector_v6/fitcode
cd $myDIR
echo $PWD

eval `scramv1 runtime -sh`

source /afs/cern.ch/work/d/dalfonso/CMSSW_5_3_22_patch1_Wmass_aug5/src/CMGTools/WMass/analysis/RecoilCode/RecoilCorrector_v6/source.csh

mkdir keysrecoilfits

##cd $TOP

root.exe -b -x -q runRecoilFit3G.C+\(1,$rap,-1,false,$myJobN\)
#root.exe -b -x -q runRecoilFit3G.C+\(1,2,-1,false,$myJobN\)


