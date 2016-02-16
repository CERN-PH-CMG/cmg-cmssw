#!/bin/bash

set -e

# charge=( "Pos", Neg )
# observable=( "Pt", "Mt", "MET" )
# eta="eta"( "0p9" )
# input="Datacard"( "", "ALT" )".txt" 

# Check if it's running on lxplus
if [[ ${HOSTNAME} != *"lxplus"* ]]
then
    echo "YOU NEED TO RUN THIS SCRIPT ON LXPLUS"
    exit 1
fi

basefolder="/afs/cern.ch/work/a/astacchi/CMSSW_5_3_22_patch1_Wmass/src/CMGTools/WMass/analysis/JobOutputs/"
destfolder="${basefolder}/Datacards/"

# sed 's/%CHARGE%/Pos/g; s/%OBSERVABLE%/Pt/g; s/%ETA%/eta0p9/g' "Datacard.txt" > "${destfolder}/Datacard.txt"
sed 's|%JOBFOLDER%|../|g; s/%CHARGE%/Pos/g; s/%OBSERVABLE%/Pt/g; s/%ETA%/eta0p9/g' "DatacardALT.txt" > "${destfolder}/DatacardALT.txt"

cd ${destfolder}

for ((m=91138; m<=91238; m=m+10))
do
    echo "Computing ${m}:"
    text2workspace.py -P "HiggsAnalysis.CombinedLimit.HiggsJPC:twoHypothesisHiggs" -m ${m} "DatacardALT.txt" -o "w${m}.root"
    combine -v3 -M "HybridNew" "w${m}.root" -H "ProfileLikelihood" --singlePoint 1 --onlyTestStat --testStat=TEV --saveHybridResult -m ${m} -n "Wlike" #| tail -n2 | head -n1 > "m${m}.log"
done
