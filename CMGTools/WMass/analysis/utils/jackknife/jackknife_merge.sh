#!/bin/bash

foldername=${1}

sample="DYJetsPow"
#sample="DYJetsMadSig"
analysis="Zanalysis"
#analysis="Wanalysis"
filename=${analysis}OnDATA.root

set -v

# Check if dir exists
if [ ! -d "${1}/output_${sample}" ];
then
  echo "No output_${sample} directory found, aborting..."
  exit 1
fi
# Check if BKG distr exists
if [ ! -f "${1}/output_EWKTT/${filename}" ]
then
  echo "Cannot find original background distribution, aborting..."
  exit 1
fi
# Check if rescaled jk distr exists
if [ ! -d "${1}/output_jackknife_rescaled" ]
then
  echo "Cannot find distribution directory, aborting..."
  exit 1
fi
# Start the real work
nchunks=$(ls ${1}/output_${sample}/${analysis}_chunk*.root |wc -l)
for ((exclude=1; exclude<nchunks; exclude++))
do
  if [ ! -f ${1}/output_jackknife_rescaled/${analysis}_${exclude}.root ]
  then
    echo "${analysis}_${exclude}.root doesn't exist"
  else
    bfolder="JobOutputs/jackknife_${exclude}"
    mkdir ${bfolder}
    mkdir ${bfolder}/output_MCDATALIKEPOW
    hadd ${bfolder}/output_MCDATALIKEPOW/${filename} ${1}/output_${sample}_jackknife/${analysis}_${exclude}.root ${1}/output_EWKTT/${filename}
    cp ${1}/common.h ${bfolder}
  fi
done

# for ((i=2; i<=2092; ++i)); do ./launch_analysis_jackknife.py $i; done
