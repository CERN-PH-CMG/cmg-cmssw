#!/bin/bash

set -e

# charge=( "Pos" "Neg" )
observables=( "Pt" "Mt" "MET" )
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

startfolder="/afs/cern.ch/work/a/astacchi/CMSSW_5_3_22_patch1_Wmass/src/CMGTools/WMass/analysis/Datacards"
destfolder="/afs/cern.ch/work/a/astacchi/CMSSW_5_3_22_patch1_Wmass/src/CMGTools/WMass/analysis/JobOutputs/Datacards"

cp "fit_2H.cpp" ${destfolder}

cd ${destfolder}

worker()
{
    observable=$1
    
    # Build datacard from template
    sed "s|%JOBFOLDER%|../|g; s/%CHARGE%/Pos/g; s/%OBSERVABLE%/${observable}/g; s/%ETA%/eta0p9/g" "${startfolder}/Datacard2H.txt" > "${destfolder}/Datacard_${observable}.txt"
    
    echo "Building workspaces [${observable}]"

    for ((m=mass_initial; m<=mass_final; m=m+mass_step))
    do
        if [ ! -f "wspace_${observable}_m${m}.root" ]; then
            text2workspace.py -m ${m} "Datacard_${observable}.txt" -P "HiggsAnalysis.CombinedLimit.HiggsJPC:twoHypothesisHiggs" -o "wspace_${observable}_m${m}.root" &
        fi
    done

    wait

    echo "Freezing none (best fit) [${observable}]"

    for ((m=mass_initial; m<=mass_final; m=m+mass_step))
    do
        #Running and saving the best-fit snapshot:
        combine -v2 -M "HybridNew" --testStat=TEV --singlePoint 1 --onlyTestStat "wspace_${observable}_m${m}.root" -m ${m} -n "Wlike_${observable}_freezeNone" --minimizerTolerance=0.00001 &> "freezeNone_${observable}_m${m}.log" &
    done

    wait

    echo "Getting best fit nuisance values (MW = ${mass_bestfit}) [${observable}]"
    bestNuisances=$(head -n-5 freezeNone_${observable}_m${mass_bestfit}.log | tail -n${nuisanceNumber} | tr -s ' ' | cut -d' ' -f2,4 | tr ' ' '=' | tr '\n' ',')
    bestNuisances=${bestNuisances%?}  # Remove last ','

    echo "Freezing all [${observable}]"

    for ((m=mass_initial; m<=mass_final; m=m+mass_step))
    do
        #Profiling with all groups frozen to best-fit values:
        combine -v${debuglevel} -M "HybridNew" --testStat=TEV --singlePoint 1 --onlyTestStat "wspace_${observable}_m${m}.root" -m ${m} -n "Wlike_${observable}_freezeAll" --setPhysicsModelParameters ${bestNuisances} --freezeNuisanceGroups ${groupscomma} --minimizerTolerance=0.00001 &> "freezeAll_${observable}_m${m}.log" &
    done

    wait

    echo "Freezing groups [${observable}]"

    for ((m=mass_initial; m<=mass_final; m=m+mass_step))
    do
        #Profiling with groups frozen to best-fit values:
        for group in "${groups[@]}"; do
            combine -v${debuglevel} -M "HybridNew" --testStat=TEV --singlePoint 1 --onlyTestStat "wspace_${observable}_m${m}.root" -m ${m} -n "Wlike_${observable}_freeze${group}" --setPhysicsModelParameters ${bestNuisances} --freezeNuisanceGroups ${group} --minimizerTolerance=0.00001 &> "freeze${group}_${observable}_m${m}.log" &
        done
    done

    wait

    grep "Q_{TEV}" freezeNone_${observable}_m91???.log |cut -d' ' -f5 > "likelihoods_freezeNone_${observable}.txt"
    root -l -b -q "fit_2H.cpp(\"likelihoods_freezeNone_${observable}.txt\", \"results_${observable}.txt\", true)"

    grep "Q_{TEV}" freezeAll_${observable}_m91???.log |cut -d' ' -f5 > "likelihoods_freezeAll_${observable}.txt"
    root -l -b -q "fit_2H.cpp(\"likelihoods_freezeAll_${observable}.txt\", \"results_${observable}.txt\", false)"

    for group in "${groups[@]}"; do
        grep "Q_{TEV}" freeze${group}_${observable}_m91???.log |cut -d' ' -f5 > "likelihoods_freeze${group}_${observable}.txt"
        root -l -b -q "fit_2H.cpp(\"likelihoods_freeze${group}_${observable}.txt\", \"results_${observable}.txt\", false)"
    done
    
    wait
}

for observable in "${observables[@]}"; do
    worker ${observable} &
done

wait

echo "Done"
