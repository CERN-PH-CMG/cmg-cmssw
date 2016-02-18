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
    :
    text2workspace.py -m ${m} "DatacardALT.txt" -P "HiggsAnalysis.CombinedLimit.HiggsJPC:twoHypothesisHiggs" -o "wALT${m}.root" &
done

wait

for ((m=91138; m<=91238; m=m+10))
do
    echo "Computing ${m}:"
    combine -v9 -M "MaxLikelihoodFit" "wALT${m}.root" --setPhysicsModelParameterRanges x=1.0,1.0 --minimizerStrategy 2 --minimizerStrategyForMinos 2 --minos=all --robustFit 1 -m ${m} -n "WlikeALT${m}" &> "mALT${m}.log" &
done

wait

echo "Done"
