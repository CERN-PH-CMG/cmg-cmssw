#!/bin/bash

#read arguments
localSrc=$1
ffile=$2
step=$3

#configuration
ME=`whoami`
MYLETTER=${ME:0:1}
MYCMSSWDIR=/afs/cern.ch/user/${MYLETTER}/${ME}/scratch0/CMSSW_4_1_3_patch2/src
MYOUTDIR="/castor/cern.ch/user/${MYLETTER}/${ME}/Dileptons/${localSrc}"
output="/tmp/evHyp.root"
if [ -n "$ffile" ]
then
    output="/tmp/evHyp_${ffile}.root"
fi
if [ -n "$step" ]
then

    output="/tmp/evHyp_${ffile}_${step}.root"
fi

#call cmsRun
cd ${MYCMSSWDIR}
export SCRAM_ARCH=slc5_amd64_gcc434
eval `scram r -sh`
cd ${MYCMSSWDIR}/CMGTools/HtoZZ2l2nu/test
cmsRun zzllvvCleanEvent_cfg.py $localSrc $ffile $step

#move to output directory
if [ -e "$output" ]
then
    rfmkdir $MYOUTDIR
    rfcp $output $MYOUTDIR/
    rm $output
    rfdir $MYOUTDIR
else
    echo "*** Nothing done: output $output was not found ***"
fi
