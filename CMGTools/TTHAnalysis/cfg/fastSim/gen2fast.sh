#!/bin/bash
TMPDIR=$PWD
#### ENV
cd /afs/cern.ch/user/g/gpetrucc/ttH/CMGTools/CMSSW_5_3_5/src/CMGTools/TTHAnalysis/cfg/fastSim
export SCRAM_ARCH=slc5_amd64_gcc462
eval $(scramv1 runtime -sh)
#### CREATE CFG
INFILE=$1
OUTFILE=$2
OUTBASE=$(basename $OUTFILE .root)
echo "Will read from $INFILE and write to $OUTFILE";
if echo $INFILE | grep -q -v ^/store/; then
    if echo $INFILE | grep -q lhe.gz; then  
        echo "Unzipping $INFILE in $TMPDIR/events.lhe"
        zcat $INFILE > $TMPDIR/events.lhe
        INFILE="file:$TMPDIR/events.lhe";
    else
        INFILE="file:$INFILE";
    fi
fi

cat FASTSIM_HLT_PU.py  > jobs/FASTSIM_HLT_PU_$OUTBASE.cfg.py
echo "process.maxEvents.input = -1"                              >> jobs/FASTSIM_HLT_PU_$OUTBASE.cfg.py
echo "process.source.fileNames = [ '$INFILE' ]"                  >> jobs/FASTSIM_HLT_PU_$OUTBASE.cfg.py
echo "process.AODSIMoutput.fileName = '$TMPDIR/$OUTBASE.root'"   >> jobs/FASTSIM_HLT_PU_$OUTBASE.cfg.py
cmsRun jobs/FASTSIM_HLT_PU_$OUTBASE.cfg.py 2>&1 | tee jobs/FASTSIM_HLT_PU_$OUTBASE.log
test -f $TMPDIR/$OUTBASE.root && cmsStageIn $TMPDIR/$OUTBASE.root $OUTFILE
