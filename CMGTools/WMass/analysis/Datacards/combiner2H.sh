#!/bin/bash

set -e

# charge=( "Pos", Neg )
# observable=( "Pt", "Mt", "MET" )
# eta="eta"( "0p9" )

# Check if it's running on lxplus
if [[ ${HOSTNAME} != *"lxplus"* ]]
then
    echo "YOU NEED TO RUN THIS SCRIPT ON LXPLUS"
    exit 1
fi

basefolder="/afs/cern.ch/work/a/astacchi/CMSSW_5_3_22_patch1_Wmass/src/CMGTools/WMass/analysis/JobOutputs/"
destfolder="${basefolder}/Datacards/"

sed 's|%JOBFOLDER%|../|g; s/%CHARGE%/Pos/g; s/%OBSERVABLE%/Pt/g; s/%ETA%/eta0p9/g' "Datacard2H.txt" > "${destfolder}/Datacard2H.txt"

cp "fit_2H.cpp" ${destfolder}

cd ${destfolder}

for ((m=91138; m<=91238; m=m+10))
do
    text2workspace.py -m ${m} "Datacard2H.txt" -P "HiggsAnalysis.CombinedLimit.HiggsJPC:twoHypothesisHiggs" -o "w2H${m}.root" &
done

wait

for ((m=91138; m<=91238; m=m+10))
do
    echo "Computing ${m}"
    #combine -v9 -M "MaxLikelihoodFit" "wALT${m}.root" --setPhysicsModelParameterRanges x=1.0,1.0 --skipBOnlyFit --minimizerStrategy 2 --minimizerStrategyForMinos 2 --minos=all --robustFit 1 -m ${m} -n "WlikeALT${m}" &> "mALT${m}.log" &
    combine -v9 -M "HybridNew" --testStat=TEV --singlePoint 1 --onlyTestStat --saveHybridResult "w2H${m}.root" -m ${m} -n "Wlike2H" &> "m2H${m}.log" &
done

wait

grep "Q_{TEV}" m2H*.log |cut -d' ' -f5 > "2H_likelihoods.txt"
root -l -b -q "fit_2H.cpp(\"2H_likelihoods.txt\")"
