#!/bin/bash

totalOutputs=`ls ${1}/res/analysis_*.root | wc -l`
nsplit=$2
echo "Integrating $1 PD"
echo '$totalOutputs found - will be divided in ${nsplit}'

let "step=${totalOutputs}/${nsplit}"
for i in `seq 0 ${nsplit}`; do
    let "startOutput=${i}*step"
    let "endOutput=(${i}+1)*step-1"
    echo "${startOutput}-${endOutput}"
    outputList=""
    for j in `seq ${startOutput} ${endOutput}`; do
	newFile="${1}/res/analysis_${j}_*.root"
	if [ -e ${newFile} ]; then
	    outputList="${outputList} ${newFile}"
	fi
    done
    hadd -f /tmp/psilva/${1}_${i}.root ${outputList}
done


