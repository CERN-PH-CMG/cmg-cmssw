#!/bin/bash

#determine CMSSW config
SCRIPT=$(readlink -f $0)
SCRIPTPATH=`dirname $SCRIPT`
ARCH=${SCRIPTPATH##/*/}
CMSSW=${SCRIPTPATH}/../../src

#configure environment
cd $CMSSW
export SCRAM_ARCH=$ARCH
eval `scram r -sh`
cd $CMSSW_BASE/src/UserCode/EWKV

#run with the arguments passed
localRun.sh $*
