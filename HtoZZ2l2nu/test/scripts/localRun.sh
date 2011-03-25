#!/bin/bash

ME=`whoami`
MYLETTER=${ME:0:1}
MYCMSSWDIR=/afs/cern.ch/user/${MYLETTER}/${ME}/scratch0/CMSSW_3_9_9/src
cd ${MYCMSSWDIR}
eval `scram r -sh`
cd ${MYCMSSWDIR}/CMGTools/HtoZZ2l2nu/test

localSrc=$1
cmsRun zzllvvCleanEvent_cfg.py $localSrc $2
outdir="/castor/cern.ch/user/${MYLETTER}/${ME}/HtoZZ/${localSrc}"
output="/tmp/evHyp.root"
rfmkdir $outdir
rfcp $output $outdir/
rm $output
rm $outmon
rfdir $outdir