#!/bin/bash

ME=`whoami`
MYCMSSWDIR=/afs/cern.ch/user/p/psilva/scratch0/CMSSW_3_8_7/src
cd ${MYCMSSWDIR}
eval `scram r -sh`
cd ${MYCMSSWDIR}/CMGTools/HtoZZ2l2nu/test

localSrc=$1
ffile=$2
lfile=$3
cmsRun runCMGTuple_cfg.py $localSrc $ffile $lfile
outdir="/castor/cern.ch/user/p/psilva/${localSrc}"
output="/tmp/${ME}/${localSrc}_${ffile}_cmgTuple.root"
outmon="/tmp/${ME}/${localSrc}_${ffile}_histograms.root"
rfmkdir $outdir
rfcp $output $outdir/
rfcp $outmon $outdir/
rm $output
rm $outmon
rfdir $outdir