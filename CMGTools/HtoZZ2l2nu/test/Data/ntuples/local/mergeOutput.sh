#!/bin/bash

totalOutputs=`ls ${1}/res/analysis_*.root | wc -l`
echo "Integrating $1 PD"
echo '$totalOutputs found - will be divided in 4'

let "step=${totalOutputs}/4"
for i in `seq 0 4`; do
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


