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
sed 's|%JOBFOLDER%|../|g; s/%CHARGE%/Pos/g; s/%OBSERVABLE%/Pt/g; s/%ETA%/eta0p9/g' "Datacard.txt" > "${destfolder}/Datacard.txt"

cd ${destfolder}

for ((m=91138; m<=91238; m=m+10))
do
    :
    #text2workspace.py -m ${m} "Datacard.txt" -o "wtoy${m}.root" &
    #text2workspace.py -m ${m} "Datacard.txt" --X-allow-no-background -o "wpull${m}.root" &
    #text2workspace.py -m ${m} "Datacard.txt" -o "wcentral${m}.root" &
    #text2workspace.py -m ${m} "Datacard.txt" -o "wdlike${m}.root" &
done

wait

for ((m=91138; m<=91238; m=m+10))
do
    echo "Computing ${m}"
    #combine -v9 -M "MaxLikelihoodFit" "wtoy${m}.root" --robustFit 1 --skipBOnlyFit -m ${m} --setPhysicsModelParameters MuonScale5=1.0 -t -1 -n "Wlike" > "mtoy${m}.log" &
    #combine -v9 -M "MaxLikelihoodFit" "wpull${m}.root" --robustFit 1 --skipBOnlyFit -m ${m} --setPhysicsModelParameters bkgUncert=-7 --expectSignal=1 -n "Wlike" > "mpull${m}.log" &
    #combine -v9 -M "MaxLikelihoodFit" "wcentral${m}.root" --robustFit 1 --skipBOnlyFit -m ${m} -n "Wlike" > "mcentral${m}.log" &
    combine -v9 -M "MaxLikelihoodFit" "wdlike${m}.root" --saveNLL --minimizerStrategy 2 --minimizerStrategyForMinos 2 --minos=all --robustFit 1 -m ${m} -n "Wlike${m}" &> "mdlike${m}.log" &
done

wait

echo "Done"
