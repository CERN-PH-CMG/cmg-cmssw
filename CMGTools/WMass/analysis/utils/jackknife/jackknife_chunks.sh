#!/bin/bash

foldername=${1}

sample="DYJetsPow"
#sample="DYJetsMadSig"
analysis="Zanalysis"
#analysis="Wanalysis"
filename=${analysis}OnDATA.root

# Check if dir exists
if [ ! -d "${1}/output_${sample}" ];
then
  echo "No output_${sample} directory found, aborting..."
  exit 1
else
  echo ""
  echo "Building output_${sample} distributions"
fi
# Count chunks
nchunks=$(ls ${1}/output_${sample}/${analysis}_chunk*.root |wc -l)
nchunks_planned=$(cat ${1}/output_${sample}/${analysis}_nChuncks.log)
if [ $nchunks -ne $nchunks_planned ]
then
  echo "Found only ${nchunks} out of the planned ${nchunks_planned}, aborting..."
  exit 2
fi

# Start the real work
mkdir -p ${foldername}/output_${sample}_jackknife
for ((exclude=1; exclude<nchunks; exclude++))
do
  if [ -f ${1}/output_${sample}_jackknife/${analysis}_${exclude}.root ]
  then
    echo "${analysis}_${exclude}.root already exists"
  else
    chunklist=$(ls ${1}/output_${sample}/${analysis}_chunk*.root|grep -v ${analysis}_chunk${exclude}.root)
    hadd ${1}/output_${sample}_jackknife/${analysis}_${exclude}.root ${chunklist}
  fi
done

