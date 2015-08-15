#!/bin/bash

#foldername=${1}
doEWKMerge=1

echo "merging chunks (if they exist)"

samples=("DATA"   "WJetsPowPlus"  "WJetsPowNeg"  "WJetsMadSig"  "WJetsMadFake"  "DYJetsPow"  "DYJetsMadSig"  "DYJetsMadFake"   "TTJets"   "ZZJets"   "WWJets"  "WZJets"    "QCD"  "T_s"  "T_t"  "T_tW"  "Tbar_s"  "Tbar_t"  "Tbar_tW")
analyses=("Wanalysis" "Zanalysis" )

for (( id_sample=0; id_sample<${#samples[@]}; id_sample++ ))
do
  # Check if dir exists
  if [ ! -d "${1}/output_${samples[id_sample]}" ];
  then
    echo "No output_${samples[id_sample]} directory found, continuing..."
    continue
  fi
  for (( id_ana=0; id_ana<${#analyses[@]}; id_ana++ ))
  do
    # Delete empty chunks before counting
    find ${1}/output_${samples[id_sample]} -size 0 -type f -name ${analyses[id_ana]}_chunk*.root -delete
    if [ -f "${1}/output_${samples[id_sample]}/${analyses[id_ana]}_nChuncks.log" ]
    then
      nchunks=$(ls ${1}/output_${samples[id_sample]}/${analyses[id_ana]}_chunk*.root |wc -l)
      nchunks_planned=$(cat ${1}/output_${samples[id_sample]}/${analyses[id_ana]}_nChuncks.log)
      if [ $nchunks -ne $nchunks_planned ]
      then
        # Check if full file exists before complaining
        if [ ! -f "${1}/output_${samples[id_sample]}/${analyses[id_ana]}OnDATA.root" ]
        then
          echo "MISSING "${analyses[id_ana]}" chunk for "${samples[id_sample]}": nchunks="${nchunks}" nchunks_planned="${nchunks_planned}"!!! EXITING"
          # Building array of missing pieces
          chunklist=$(ls ${1}/output_${samples[id_sample]}/${analyses[id_ana]}_chunk*.root)
          ARRAY=()
          for i in `seq 1 ${nchunks_planned}`;
          do
            if [[ $chunklist != *""${analyses[id_ana]}"_chunk"${i}".root"* ]]
            then
              ARRAY+=(""${i}"")
            fi
          done
          echo "List of the missing chunks:"
          printf "%s "  "${ARRAY[@]}"
          echo ""
          doEWKMerge=0
        fi
      else
        hadd -f ${1}/output_${samples[id_sample]}/${analyses[id_ana]}OnDATA.root ${1}/output_${samples[id_sample]}/${analyses[id_ana]}_chunk*.root
      fi
    else
      echo "No ${analyses[id_ana]}_nChuncks.log found in output_${samples[id_sample]}, continuing..."
    fi
  done
done

echo ""
if [ ${doEWKMerge} == 0 ]
then
  echo "NO EWK MERGE DONE, AS THERE WERE MISSING HISTOGRAMS"
else
  echo 'EWK ONLY (EWK)'
  # EWK ONLY
  mkdir -p ${1}/output_EWK
  echo 'Z analysis (W+Jets sig+fake, DY+Jets fake, ZZ, WZ, WW)'
  hadd -f ${1}/output_EWK/ZanalysisOnDATA.root ${1}/output_WJetsMadFake/ZanalysisOnDATA.root ${1}/output_DYJetsMadFake/ZanalysisOnDATA.root ${1}/output_ZZJets/ZanalysisOnDATA.root ${1}/output_WZJets/ZanalysisOnDATA.root ${1}/output_WWJets/ZanalysisOnDATA.root
  echo 'W analysis (W+Jets fake, DY+Jets sig+fake, ZZ, WZ, WW)'
  hadd -f ${1}/output_EWK/WanalysisOnDATA.root ${1}/output_WJetsMadFake/WanalysisOnDATA.root ${1}/output_DYJetsMadFake/WanalysisOnDATA.root ${1}/output_ZZJets/WanalysisOnDATA.root ${1}/output_WZJets/WanalysisOnDATA.root ${1}/output_WWJets/WanalysisOnDATA.root

  echo 'EWK + TT + Single Top (EWKTT)'
  # EWK + TT
  mkdir -p ${1}/output_EWKTT
  echo 'Z analysis'
  hadd -f ${1}/output_EWKTT/ZanalysisOnDATA.root ${1}/output_EWK/ZanalysisOnDATA.root ${1}/output_TTJets/ZanalysisOnDATA.root ${1}/output_T_s/ZanalysisOnDATA.root ${1}/output_T_t/ZanalysisOnDATA.root ${1}/output_T_tW/ZanalysisOnDATA.root ${1}/output_Tbar_s/ZanalysisOnDATA.root ${1}/output_Tbar_t/ZanalysisOnDATA.root ${1}/output_Tbar_tW/ZanalysisOnDATA.root
  echo 'W analysis'
  hadd -f ${1}/output_EWKTT/WanalysisOnDATA.root ${1}/output_EWK/WanalysisOnDATA.root ${1}/output_TTJets/WanalysisOnDATA.root ${1}/output_T_s/WanalysisOnDATA.root ${1}/output_T_t/WanalysisOnDATA.root ${1}/output_T_tW/WanalysisOnDATA.root ${1}/output_Tbar_s/WanalysisOnDATA.root ${1}/output_Tbar_t/WanalysisOnDATA.root ${1}/output_Tbar_tW/WanalysisOnDATA.root

  echo 'EWK + TT + Single Top + SIG POWHEG (MCDATALIKEPOW)'
  # EWK + TT + SIG
  mkdir -p ${1}/output_MCDATALIKEPOW
  echo 'Z analysis'
  hadd -f ${1}/output_MCDATALIKEPOW/ZanalysisOnDATA.root ${1}/output_EWKTT/ZanalysisOnDATA.root ${1}/output_DYJetsPow/ZanalysisOnDATA.root
  echo 'W analysis'
  hadd -f ${1}/output_MCDATALIKEPOW/WanalysisOnDATA.root ${1}/output_EWKTT/WanalysisOnDATA.root ${1}/output_WJetsPowPlus/WanalysisOnDATA.root ${1}/output_WJetsPowNeg/WanalysisOnDATA.root

  echo 'EWK + TT + Single Top + SIG MADGRAPH (MCDATALIKEMAD)'
  # EWK + TT + SIG
  mkdir -p ${1}/output_MCDATALIKEMAD
  echo 'Z analysis'
  hadd -f ${1}/output_MCDATALIKEMAD/ZanalysisOnDATA.root ${1}/output_EWKTT/ZanalysisOnDATA.root ${1}/output_DYJetsMadSig/ZanalysisOnDATA.root
  echo 'W analysis'
  hadd -f ${1}/output_MCDATALIKEMAD/WanalysisOnDATA.root ${1}/output_EWKTT/WanalysisOnDATA.root ${1}/output_WJetsMadSig/WanalysisOnDATA.root
fi
