#!/bin/bash

#foldername=${1}
#resubmit=${2}
#queue=${3}
useBatch=${4}

echo "merging chunks (if they exist)"

samples=("DATA"   "WJetsPowPlus"  "WJetsPowNeg"  "WJetsMadSig"  "WJetsMadFake"  "DYJetsPow"  "DYJetsMadSig"  "DYJetsMadFake"   "TTJets"   "ZZJets"   "WWJets"  "WZJets"    "QCD"  "T_s"  "T_t"  "T_tW"  "Tbar_s"  "Tbar_t"  "Tbar_tW")
# analyses=("Wanalysis" "Zanalysis" "PhiStarEtaAnalysis")
analyses=("Wanalysis" "Zanalysis" )

for (( id_sample=0; id_sample<${#samples[@]}; id_sample++ ))
  do
  for (( id_ana=0; id_ana<${#analyses[@]}; id_ana++ ))
    do
    
    nchunks=$(ls ${1}/output_${samples[id_sample]}/${analyses[id_ana]}_chunk*.root |wc -l)
    nchunks_planned=$(more ${1}/output_${samples[id_sample]}/${analyses[id_ana]}_nChuncks.log)
    if [ $nchunks -gt 0 ]
    then
      if [ $nchunks -ne $nchunks_planned ]
      then
        echo "MISSING "${analyses[id_ana]}" chunk for "${samples[id_sample]}": nchunks="${nchunks}" nchunks_planned="${nchunks_planned}"!!! EXITING"
        nchunk=$(ls ${1}/output_${samples[id_sample]}/${analyses[id_ana]}_chunk*.root)
        ARRAY=()
        for i in `seq 1 ${nchunks_planned}`;
        do
          # echo $i
          if [[ $nchunk != *""${analyses[id_ana]}"_chunk"${i}".root"* ]]
          then
            # echo "${i} It's not there!";
            ARRAY+=(""${i}"")
            if [[ ${2} == 1 ]]
            then
              cd ${1}/output_${samples[id_sample]}/
              # echo "RESUBMITTING "${i}
              if [[ $useBatch == 1 ]]
              then
                echo "bsub -q ${3} -J run${analyses[id_ana]} run${analyses[id_ana]}_${samples[id_sample]}_${i}.sh"
                bsub -q ${3} -u pippo123 -J run${analyses[id_ana]} run${analyses[id_ana]}_${samples[id_sample]}_${i}.sh
              else
                sh run${analyses[id_ana]}_${samples[id_sample]}_${i}.sh
              fi
              cd -
            fi
            # echo $ARRAY
          fi
        done
        printf "%s "  "${ARRAY[@]}"
        echo ""
        # echo ${ARRAY}
        echo "List of the chunks rebuilt (the eventual batch jobs may or may not be finished)"
        read -p "PRESS 1 TO EXIT, 0 TO CONTINUE THE MERGE: " n1
        if [[ ${n1} == 1 ]]
        then
          exit 1
          # continue
        fi
      fi
      hadd -f ${1}/output_${samples[id_sample]}/${analyses[id_ana]}OnDATA.root ${1}/output_${samples[id_sample]}/${analyses[id_ana]}_chunk*.root 
      rm ${1}/output_${samples[id_sample]}/${analyses[id_ana]}_chunk*.root 
      rm -rf ${1}/output_${samples[id_sample]}/LSFJOB_*
      rm -rf ${1}/output_${samples[id_sample]}/*.out
    fi
    
  done
done

# exit 1

echo 'EWK ONLY (EWK)'
# EWK ONLY
mkdir ${1}/output_EWK
echo 'Z analysis (W+Jets sig+fake, DY+Jets fake, ZZ, WZ, WW)'
hadd -f ${1}/output_EWK/ZanalysisOnDATA.root ${1}/output_WJetsMadFake/ZanalysisOnDATA.root ${1}/output_DYJetsMadFake/ZanalysisOnDATA.root ${1}/output_ZZJets/ZanalysisOnDATA.root ${1}/output_WZJets/ZanalysisOnDATA.root ${1}/output_WWJets/ZanalysisOnDATA.root 
echo 'W analysis (W+Jets fake, DY+Jets sig+fake, ZZ, WZ, WW)'
hadd -f ${1}/output_EWK/WanalysisOnDATA.root ${1}/output_WJetsMadFake/WanalysisOnDATA.root ${1}/output_DYJetsMadFake/WanalysisOnDATA.root ${1}/output_ZZJets/WanalysisOnDATA.root ${1}/output_WZJets/WanalysisOnDATA.root ${1}/output_WWJets/WanalysisOnDATA.root 

echo 'EWK + TT + Single Top (EWKTT)'
# EWK + TT
mkdir ${1}/output_EWKTT
echo 'Z analysis'
hadd -f ${1}/output_EWKTT/ZanalysisOnDATA.root ${1}/output_EWK/ZanalysisOnDATA.root ${1}/output_TTJets/ZanalysisOnDATA.root ${1}/output_T_s/ZanalysisOnDATA.root ${1}/output_T_t/ZanalysisOnDATA.root ${1}/output_T_tW/ZanalysisOnDATA.root ${1}/output_Tbar_s/ZanalysisOnDATA.root ${1}/output_Tbar_t/ZanalysisOnDATA.root ${1}/output_Tbar_tW/ZanalysisOnDATA.root
echo 'W analysis'
hadd -f ${1}/output_EWKTT/WanalysisOnDATA.root ${1}/output_EWK/WanalysisOnDATA.root ${1}/output_TTJets/WanalysisOnDATA.root ${1}/output_T_s/WanalysisOnDATA.root ${1}/output_T_t/WanalysisOnDATA.root ${1}/output_T_tW/WanalysisOnDATA.root ${1}/output_Tbar_s/WanalysisOnDATA.root ${1}/output_Tbar_t/WanalysisOnDATA.root ${1}/output_Tbar_tW/WanalysisOnDATA.root

echo 'EWK + TT + Single Top + SIG POWHEG (MCDATALIKEPOW)'
# EWK + TT + SIG
mkdir ${1}/output_MCDATALIKEPOW
echo 'Z analysis'
hadd -f ${1}/output_MCDATALIKEPOW/ZanalysisOnDATA.root ${1}/output_EWKTT/ZanalysisOnDATA.root ${1}/output_DYJetsPow/ZanalysisOnDATA.root
echo 'W analysis'
hadd -f ${1}/output_MCDATALIKEPOW/WanalysisOnDATA.root ${1}/output_EWKTT/WanalysisOnDATA.root ${1}/output_WJetsPowPlus/WanalysisOnDATA.root ${1}/output_WJetsPowNeg/WanalysisOnDATA.root

echo 'EWK + TT + Single Top + SIG MADGRAPH (MCDATALIKEMAD)'
# EWK + TT + SIG
mkdir ${1}/output_MCDATALIKEMAD
echo 'Z analysis'
hadd -f ${1}/output_MCDATALIKEMAD/ZanalysisOnDATA.root ${1}/output_EWKTT/ZanalysisOnDATA.root ${1}/output_DYJetsMadSig/ZanalysisOnDATA.root
echo 'W analysis'
hadd -f ${1}/output_MCDATALIKEMAD/WanalysisOnDATA.root ${1}/output_EWKTT/WanalysisOnDATA.root ${1}/output_WJetsMadSig/WanalysisOnDATA.root
