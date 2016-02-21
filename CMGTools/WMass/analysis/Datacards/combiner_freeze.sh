#!/bin/bash

set -e

# charge=( "Pos", Neg )
# observable=( "Pt", "Mt", "MET" )
# eta="eta"( "0p9" )

debuglevel=0
mass_initial=91138
mass_final=91238
mass_step=10

mass_bestfit=91188

groups=(MuonScales KalmanVars RecoilEigens)
groupscomma=$(printf ",%s" "${groups[@]}")
groupscomma=${groupscomma:1}
# Total nuisance number: 1+1+45+144 = 191
nuisanceNumber=191

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
    if [ ! -f "w2H${m}.root" ]; then
        text2workspace.py -m ${m} "Datacard2H.txt" -P "HiggsAnalysis.CombinedLimit.HiggsJPC:twoHypothesisHiggs" -o "w2H${m}.root" &
    fi
done

wait

echo "Freezing none (best fit)"

for ((m=mass_initial; m<=mass_final; m=m+mass_step))
do
    #Running and saving the best-fit snapshot:
    combine -v2 -M "HybridNew" --testStat=TEV --singlePoint 1 --onlyTestStat "w2H${m}.root" -m ${m} -n "Wlike2H_freezeNone" &> "freezeNone_m2H${m}.log" &
done

wait

echo "Getting best fit nuisance values (MW = ${mass_bestfit})"
bestNuisances=$(head -n-5 freezeNone_m2H${mass_bestfit}.log | tail -n${nuisanceNumber} | tr -s ' ' | cut -d' ' -f2,4 | tr ' ' '=' | tr '\n' ',')
bestNuisances=${bestNuisances%?}  # Remove last ','

echo "Freezing all"

for ((m=mass_initial; m<=mass_final; m=m+mass_step))
do
    #Profiling with all groups frozen to best-fit values:
    combine -v${debuglevel} -M "HybridNew" --testStat=TEV --singlePoint 1 --onlyTestStat "w2H${m}.root" -m ${m} -n "Wlike2H_freezeAll" --setPhysicsModelParameters ${bestNuisances} --freezeNuisanceGroups ${groupscomma} &> "freezeAll_m2H${m}.log" &
done

wait

echo "Freezing groups"

for ((m=mass_initial; m<=mass_final; m=m+mass_step))
do
    #Profiling with groups frozen to best-fit values:
    for group in "${groups[@]}"; do
        combine -v${debuglevel} -M "HybridNew" --testStat=TEV --singlePoint 1 --onlyTestStat "w2H${m}.root" -m ${m} -n "Wlike2H_freeze${group}" --setPhysicsModelParameters ${bestNuisances} --freezeNuisanceGroups ${group} &> "freeze${group}_m2H${m}.log" &
    done
done

wait

grep "Q_{TEV}" freezeNone_m2H91*.log |cut -d' ' -f5 > "likelihoods_freezeNone.txt"
root -l -b -q "fit_2H.cpp(\"likelihoods_freezeNone.txt\")"
echo ""
grep "Q_{TEV}" freezeAll_m2H91*.log |cut -d' ' -f5 > "likelihoods_freezeAll.txt"
root -l -b -q "fit_2H.cpp(\"likelihoods_freezeAll.txt\")"
echo ""
for group in "${groups[@]}"; do
    grep "Q_{TEV}" freeze${group}_m2H91*.log |cut -d' ' -f5 > "likelihoods_freeze${group}.txt"
    root -l -b -q "fit_2H.cpp(\"likelihoods_freeze${group}.txt\")"
    echo ""
done

