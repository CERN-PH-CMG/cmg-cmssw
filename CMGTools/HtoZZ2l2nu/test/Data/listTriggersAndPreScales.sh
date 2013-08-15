#!/bin/bash

run=$1
dataset=$2

trigs=(`edmConfigFromDB --runNumber ${run} --format streams.list:A.${dataset}`)
echo "${dataset}:" > Dataset_${dataset}.list
echo "${dataset}:" > TriggerInfo_${dataset}.list
for t in ${trigs[@]}; do
    echo ${t} >> Dataset_${dataset}.list
    edmConfigFromDB --runNumber $run --format summary.ascii --paths $t >> TriggerInfo_${dataset}.list 
done

echo "Dataset summary is defined in: Dataset_${dataset}.list"
echo "Trigger info is defined in: TriggerInfo_${dataset}.list"