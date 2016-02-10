#!/bin/bash

#foldername=${1}
echo "Merging chunks (if they exist)"

samples=("DATA"   "WJetsPowPlus"  "WJetsPowNeg"  "WJetsMadSig"  "WJetsMadFake"  "DYJetsPow"  "DYJetsMadSig"  "DYJetsMadFake"   "TTJets"   "ZZJets"   "WWJets"  "WZJets"    "QCD"  "T_s"  "T_t"  "T_tW"  "Tbar_s"  "Tbar_t"  "Tbar_tW")
cmd_params=($@)
analyses=("${cmd_params[@]:1}")
#analyses=("Wanalysis" "Zanalysis" )

for (( id_ana=0; id_ana<${#analyses[@]}; id_ana++ ))
do
  doEWKMerge=1
  for (( id_sample=0; id_sample<${#samples[@]}; id_sample++ ))
  do
    echo ""
    # Check if dir exists
    if [ ! -d "${1}/output_${samples[id_sample]}" ];
    then
      echo "No output_${samples[id_sample]} directory found, continuing..."
      continue
    else
      echo "Merging output_${samples[id_sample]}"
    fi
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
          for i in $(seq 1 ${nchunks_planned})
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
        else
          echo "Found already merged output_${samples[id_sample]}/${analyses[id_ana]}OnDATA.root"
        fi
      else
        hadd -f ${1}/output_${samples[id_sample]}/${analyses[id_ana]}OnDATA.root ${1}/output_${samples[id_sample]}/${analyses[id_ana]}_chunk*.root
      fi
    else
      echo "No ${analyses[id_ana]}_nChuncks.log found in output_${samples[id_sample]}, continuing..."
    fi
  done
  
  echo ""
  if [ ${doEWKMerge} == 0 ]
  then
    echo "NO ${analyses[id_ana]} EWK MERGE DONE, AS THERE WERE MISSING HISTOGRAMS"
    exit 1
  else
    echo ${analyses[id_ana]}" MERGE"
    filename=${analyses[id_ana]}OnDATA
    
    echo ""; echo 'EWK ONLY (EWK)'
    # EWK ONLY
    # Z: W+Jets sig+fake, DY+Jets fake, ZZ, WZ, WW
    # W: W+Jets fake, DY+Jets sig+fake, ZZ, WZ, WW
    mkdir -p ${1}/output_EWK
    hadd -f ${1}/output_EWK/${filename}.root ${1}/output_WJetsMadFake/${filename}.root ${1}/output_DYJetsMadFake/${filename}.root ${1}/output_ZZJets/${filename}.root ${1}/output_WZJets/${filename}.root ${1}/output_WWJets/${filename}.root

    echo ""; echo 'EWK + TT + Single Top (EWKTT)'
    # EWK + TT
    mkdir -p ${1}/output_EWKTT
    hadd -f ${1}/output_EWKTT/${filename}.root ${1}/output_EWK/${filename}.root ${1}/output_TTJets/${filename}.root ${1}/output_T_s/${filename}.root ${1}/output_T_t/${filename}.root ${1}/output_T_tW/${filename}.root ${1}/output_Tbar_s/${filename}.root ${1}/output_Tbar_t/${filename}.root ${1}/output_Tbar_tW/${filename}.root

    echo ""; echo 'EWK + TT + Single Top + SIG POWHEG (MCDATALIKEPOW)'
    # EWK + TT + SIG
    mkdir -p ${1}/output_MCDATALIKEPOW
    hadd -f ${1}/output_MCDATALIKEPOW/${filename}.root ${1}/output_EWKTT/${filename}.root ${1}/output_DYJetsPow/${filename}.root

#    echo ""; echo 'EWK + TT + Single Top + SIG MADGRAPH (MCDATALIKEMAD)'
#    # EWK + TT + SIG
#    mkdir -p ${1}/output_MCDATALIKEMAD
#    hadd -f ${1}/output_MCDATALIKEMAD/${filename}.root ${1}/output_EWKTT/${filename}.root ${1}/output_DYJetsMadSig/${filename}.root
  fi
done
