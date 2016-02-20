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

echo "Building workspaces"

for ((m=mass_initial; m<=mass_final; m=m+mass_step))
do
    text2workspace.py -m ${m} "Datacard2H.txt" -P "HiggsAnalysis.CombinedLimit.HiggsJPC:twoHypothesisHiggs" -o "w2H${m}.root" &
done

wait

echo "Float all (best fit)"

for ((m=mass_initial; m<=mass_final; m=m+mass_step))
do
    #Running and saving the best-fit snapshot:
    combine -v${debuglevel} -M "HybridNew" --testStat=TEV --singlePoint 1 --onlyTestStat "w2H${m}.root" -m ${m} -n "Wlike2H_freezeNone" &> "freezeNone_m2H${m}.log" &
done

wait

echo "Freezing all"

for ((m=mass_initial; m<=mass_final; m=m+mass_step))
do
    #Profiling with all groups frozen to best-fit values:
    combine -v${debuglevel} -M "HybridNew" --testStat=TEV --singlePoint 1 --onlyTestStat "w2H${m}.root" -m ${m} -n "Wlike2H_freeze${group}"  --freezeNuisanceGroups MuonScales,KalmanVars &> "freezeAll_m2H${m}.log" &
done

wait

echo "Freezing groups"

for ((m=mass_initial; m<=mass_final; m=m+mass_step))
do
    #Profiling with groups frozen to best-fit values:
    for group in MuonScales KalmanVars; do
        combine -v${debuglevel} -M "HybridNew" --testStat=TEV --singlePoint 1 --onlyTestStat "w2H${m}.root" -m ${m} -n "Wlike2H_freeze${group}" --freezeNuisanceGroups ${group} &> "freeze${group}_m2H${m}.log" &
    done
done

wait

grep "Q_{TEV}" m2H*_bestfit.log |cut -d' ' -f5 > "likelihoods_bestfit.txt"
root -l -b -q "fit_2H.cpp(\"likelihoods_bestfit.txt\")"
echo ""
grep "Q_{TEV}" m2H*_freezeAll.log |cut -d' ' -f5 > "likelihoods_freezeAll.txt"
root -l -b -q "fit_2H.cpp(\"likelihoods_freezeAll.txt\")"
echo ""
for group in MuonScales KalmanVars; do
    grep "Q_{TEV}" m2H*_freeze${group}.log |cut -d' ' -f5 > "likelihoods_freeze${group}.txt"
    root -l -b -q "fit_2H.cpp(\"likelihoods_freeze${group}.txt\")"
    echo ""
done

