#!/bin/bash
TMPDIR=$PWD
#### ENV
cd /afs/cern.ch/user/g/gpetrucc/w/GENS/CMSSW_5_3_5/src
export SCRAM_ARCH=slc5_amd64_gcc462
eval $(scramv1 runtime -sh)
cd /afs/cern.ch/user/g/gpetrucc/ttH/CMGTools/CMSSW_5_3_5/src/CMGTools/TTHAnalysis/cfg/fastSim
#### CREATE CFG
M="default"; if [[ "$1" == "--up" || "$1" == "--down" || "$1" == "--nojets" || "$1" == "--xqtup" || "$1" == "--xqtdown"  ]]; then M="$1"; shift; fi;
OUTFILE=$1
OUTBASE=$(basename $OUTFILE .root)
echo "Will  write to $OUTFILE";
shift;

INFILES=""; COUNTER=0
while [[ "$1" != "" ]]; do
    INFILE=$1; shift
    echo "Will read from $INFILE";
    if echo $INFILE | grep -q -v ^/store/; then
        if echo $INFILE | grep -q lhe.gz; then  
            echo "Unzipping $INFILE in $TMPDIR/events.$COUNTER.lhe"
            zcat $INFILE > $TMPDIR/events.$COUNTER.lhe
            INFILE="file:$TMPDIR/events.$COUNTER.lhe";
            COUNTER=$(($COUNTER+1))
        else
            INFILE="file:$INFILE";
        fi
    fi
    if [[ "$INFILES" != "" ]]; then
        INFILES="$INFILES, '$INFILE'"; 
    else
        INFILES="'$INFILE'"; 
    fi
done

cat Hadronizer_MgmMatchTuneZ2star_8TeV_madgraph_tauola_cff_py_noCMS_GEN.py > jobs/$OUTBASE.cfg.py
echo "process.source.fileNames = [ $INFILES ]"                   >> jobs/$OUTBASE.cfg.py
echo "process.AODSIMoutput.fileName = '$TMPDIR/$OUTBASE.root'"   >> jobs/$OUTBASE.cfg.py
if [[ "$M" == "--up" ]]; then
    echo "Matching scale UP"
    echo "process.generator.PythiaParameters.processParameters += [ 'PARP(64)=4.', 'PARP(72)=0.125' ]"   >> jobs/$OUTBASE.cfg.py
elif [[ "$M" == "--down" ]]; then
    echo "Matching scale DOWN"
    echo "process.generator.PythiaParameters.processParameters += [ 'PARP(64)=0.25', 'PARP(72)=0.5' ]"   >> jobs/$OUTBASE.cfg.py
elif [[ "$M" == "--xqtup" ]]; then
    echo "Matching cut DOWN"
    echo "process.generator.jetMatching.MEMAIN_qcut = 20"   >> jobs/$OUTBASE.cfg.py
elif [[ "$M" == "--xqtdown" ]]; then
    echo "Matching cut DOWN"
    echo "process.generator.jetMatching.MEMAIN_qcut = 5"   >> jobs/$OUTBASE.cfg.py
elif [[ "$M" == "--nojets" ]]; then
    echo "No jet matching"
    echo "del process.generator.jetMatching"   >> jobs/$OUTBASE.cfg.py
fi
cat >> jobs/$OUTBASE.cfg.py <<_EOF_
## Scramble
import random
rnd = random.SystemRandom()
for X in process.RandomNumberGeneratorService.parameterNames_(): 
   if X != 'saveFileName': getattr(process.RandomNumberGeneratorService,X).initialSeed = rnd.randint(1,99999999)
_EOF_
cmsRun jobs/$OUTBASE.cfg.py 2>&1 | tee jobs/$OUTBASE.log
test -f $TMPDIR/$OUTBASE.root && cmsStageIn $TMPDIR/$OUTBASE.root $OUTFILE
