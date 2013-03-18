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
cat Hadronizer_MgmMatchTuneZ2star_8TeV_madgraph_tauola_cff_py_noCMS_GEN.py > jobs/$OUTBASE.cfg.py
echo "process.source.fileNames = [ '$INFILE' ]"                  >> jobs/$OUTBASE.cfg.py
echo "process.AODSIMoutput.fileName = '$TMPDIR/$OUTBASE.root'"   >> jobs/$OUTBASE.cfg.py
if [[ "$3" == "--up" ]]; then
    echo "Matching scale UP"
    echo "process.generator.PythiaParameters.processParameters += [ 'PARP(64)=4.', 'PARP(72)=0.125' ]"   >> jobs/$OUTBASE.cfg.py
elif [[ "$3" == "--down" ]]; then
    echo "Matching scale DOWN"
    echo "process.generator.PythiaParameters.processParameters += [ 'PARP(64)=0.25', 'PARP(72)=0.5' ]"   >> jobs/$OUTBASE.cfg.py
elif [[ "$3" == "--nojets" ]]; then
    echo "No jet matching"
    echo "del process.generator.jetMatching"   >> jobs/$OUTBASE.cfg.py
fi
cmsRun jobs/$OUTBASE.cfg.py 2>&1 | tee jobs/$OUTBASE.log
test -f $TMPDIR/$OUTBASE.root && cmsStageIn $TMPDIR/$OUTBASE.root $OUTFILE
