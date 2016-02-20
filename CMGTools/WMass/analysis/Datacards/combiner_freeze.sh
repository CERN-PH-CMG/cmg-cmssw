#!/bin/bash

set -e

# charge=( "Pos", Neg )
# observable=( "Pt", "Mt", "MET" )
# eta="eta"( "0p9" )

debuglevel=0
mass_initial=91138
mass_final=91238
mass_step=10

# Nuisance values gathered by floating all and getting the values at MW = 91188
# cat freezeNone_m2H91188.log | head -n-5 | tail -n47 | tr -s ' ' | cut -d' ' -f2,4
nuisancesAtM91188="BkgUncert=-3.8406e-05,KalmanVar1=-5.3786e-02,KalmanVar10=-6.6429e-03,KalmanVar11=-3.6258e-03,KalmanVar12=-1.1924e-02,KalmanVar13=-2.8164e-02,KalmanVar14=-5.8273e-02,KalmanVar15=-1.1065e-02,KalmanVar16=8.5962e-03,KalmanVar17=-6.6637e-02,KalmanVar18=1.1620e-02,KalmanVar19=-3.3417e-04,KalmanVar2=1.0221e-02,KalmanVar20=2.3358e-02,KalmanVar21=-5.3619e-03,KalmanVar22=-4.3355e-02,KalmanVar23=8.8184e-03,KalmanVar24=4.3815e-02,KalmanVar25=-9.6382e-02,KalmanVar26=-4.2929e-02,KalmanVar27=-2.5852e-02,KalmanVar28=-1.5915e-02,KalmanVar29=-1.6504e-02,KalmanVar3=7.7801e-03,KalmanVar30=2.7800e-03,KalmanVar31=3.4584e-02,KalmanVar32=-2.1771e-02,KalmanVar33=-1.3059e-02,KalmanVar34=-5.2465e-02,KalmanVar35=-6.2209e-03,KalmanVar36=5.8398e-02,KalmanVar37=2.1289e-03,KalmanVar38=7.2898e-02,KalmanVar39=4.0143e-03,KalmanVar4=-2.2333e-02,KalmanVar40=-2.4858e-02,KalmanVar41=-3.7107e-02,KalmanVar42=-6.2846e-02,KalmanVar43=-2.8481e-02,KalmanVar44=-7.0899e-02,KalmanVar45=-3.3085e-12,KalmanVar5=4.4581e-03,KalmanVar6=2.5758e-03,KalmanVar7=3.7686e-03,KalmanVar8=5.9778e-03,KalmanVar9=-1.9123e-02,MuonScale=5.8688e-02"

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
    combine -v2 -M "HybridNew" --testStat=TEV --singlePoint 1 --onlyTestStat "w2H${m}.root" -m ${m} -n "Wlike2H_freezeNone" &> "freezeNone_m2H${m}.log" &
done

wait

echo "Freezing all"

for ((m=mass_initial; m<=mass_final; m=m+mass_step))
do
    #Profiling with all groups frozen to best-fit values:
    combine -v${debuglevel} -M "HybridNew" --testStat=TEV --singlePoint 1 --onlyTestStat "w2H${m}.root" -m ${m} -n "Wlike2H_freeze${group}" --setPhysicsModelParameters ${nuisancesAtM91188} --freezeNuisanceGroups MuonScales,KalmanVars &> "freezeAll_m2H${m}.log" &
done

wait

echo "Freezing groups"

for ((m=mass_initial; m<=mass_final; m=m+mass_step))
do
    #Profiling with groups frozen to best-fit values:
    for group in MuonScales KalmanVars; do
        combine -v${debuglevel} -M "HybridNew" --testStat=TEV --singlePoint 1 --onlyTestStat "w2H${m}.root" -m ${m} -n "Wlike2H_freeze${group}" --setPhysicsModelParameters ${nuisancesAtM91188} --freezeNuisanceGroups ${group} &> "freeze${group}_m2H${m}.log" &
    done
done

wait

grep "Q_{TEV}" freezeNone_m2H91*.log |cut -d' ' -f5 > "likelihoods_bestfit.txt"
root -l -b -q "fit_2H.cpp(\"likelihoods_bestfit.txt\")"
echo ""
grep "Q_{TEV}" freezeAll_m2H91*.log |cut -d' ' -f5 > "likelihoods_freezeAll.txt"
root -l -b -q "fit_2H.cpp(\"likelihoods_freezeAll.txt\")"
echo ""
for group in MuonScales KalmanVars; do
    grep "Q_{TEV}" freeze${group}_m2H91*.log |cut -d' ' -f5 > "likelihoods_freeze${group}.txt"
    root -l -b -q "fit_2H.cpp(\"likelihoods_freeze${group}.txt\")"
    echo ""
done

