#!/bin/bash

set -e

# charge=( "Pos", Neg )
# observable=( "Pt", "Mt", "MET" )
# eta="eta"( "0p9" )

debuglevel=0
mass_initial=91138
mass_final=91238
mass_step=10

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

for ((m=mass_initial; m<=mass_final; m=m+mass_step))
do
    text2workspace.py -m ${m} "Datacard2H.txt" -P "HiggsAnalysis.CombinedLimit.HiggsJPC:twoHypothesisHiggs" -o "w2H${m}.root" &
done

wait

echo "Combining..."

for ((m=mass_initial; m<=mass_final; m=m+mass_step))
do
    combine -v${debuglevel} -M "HybridNew" --testStat=TEV --singlePoint 1 --onlyTestStat --saveHybridResult "w2H${m}.root" -m ${m} -n "Wlike2H" &> "m2H${m}.log" &
done

wait

grep "Q_{TEV}" m2H*.log |cut -d' ' -f5 > "2H_likelihoods.txt"
root -l -b -q "fit_2H.cpp(\"2H_likelihoods.txt\")"
