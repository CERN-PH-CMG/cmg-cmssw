#!/bin/bash
TMPDIR=$PWD
#### ENV
AREA=/afs/cern.ch/user/g/gpetrucc/scratch0/cmgprod/CMSSW_5_3_22/src/CMGTools/TTHAnalysis/cfg/fastSim
cd $AREA
export SCRAM_ARCH=slc5_amd64_gcc462
eval $(scramv1 runtime -sh)
#### CREATE CFG
EVENTS=""
if [[ "$1" == "--events" ]]; then
    EVENTS="process.source.skipEvents = cms.untracked.uint32($2); process.maxEvents.input = cms.untracked.int32($3);"
    shift; shift; shift;
fi;

########################################### FIXED ORDER NLO (aMC@NLO) ###########################################
if [[ "$1" == "--nlo" ]]; then
    NCOL="$2"; 
    if [[ "$NCOL" != "" ]] && seq 1 20 | grep -q $NCOL; then
        echo "Will configure for aMCatNLO with TimeShower:nPartonsInBorn = $NCOL";
    else
        echo "ERROR: syntax is --nlo NCOL  (number of coloured particles in born matrix element, before resonance decays)";
        exit 1;
    fi
    EVENTS="$EVENTS
from Configuration.Generator.Pythia8CommonSettings_cfi import *
from Configuration.Generator.Pythia8CUEP8M1Settings_cfi import *
from Configuration.Generator.Pythia8aMCatNLOSettings_cfi import *
process.generator.PythiaParameters = cms.PSet(
              pythia8CommonSettingsBlock,
              pythia8CUEP8M1SettingsBlock,
              pythia8aMCatNLOSettingsBlock,
              processParameters = cms.vstring(
                  'TimeShower:nPartonsInBorn = $NCOL', #number of coloured particles (before resonance decays) in born matrix element
              ),
              parameterSets = cms.vstring('pythia8CommonSettings',
                  'pythia8CUEP8M1Settings',
                  'pythia8aMCatNLOSettings',
                  'processParameters',
              )
          )
"
    shift; shift;
########################################### LEADING ORDER + JETS (MADGRAPH) ###########################################
elif [[ "$1" == "--jets" ]]; then
    NJETS="$2"; QCUT="$3"
    if [[ "$NJETS" != "" ]] && [[ "$QCUT" != "" ]] && seq 1 10 | grep -q $NJETS; then
        echo "Will go up to $NJETS from matrix element, with qCut $QCUT";
    else
        echo "ERROR: syntax is --jets NJETS QCUT  (got $NJETS $QCUT)";
        exit 1;
    fi
    EVENTS="$EVENTS
process.generator.PythiaParameters.processParameters = [
              'JetMatching:setMad = off',
              'JetMatching:scheme = 1',
              'JetMatching:merge = on',
              'JetMatching:jetAlgorithm = 2',
              'JetMatching:etaJetMax = 5.',
              'JetMatching:coneRadius = 1.',
              'JetMatching:slowJetPower = 1',
              'JetMatching:qCut = $QCUT'  , #this is the actual merging scale
              'JetMatching:nQmatch = 5', #4 corresponds to 4-flavour scheme (no matching of b-quarks), 5 for 5-flavour scheme
              'JetMatching:nJetMax = $NJETS', #number of partons in born matrix element for highest multiplicity
              'JetMatching:doShowerKt = off', #off for MLM matching, turn on for shower-kT matching
]
process.generator.PythiaParameters.parameterSets += [ 'processParameters' ]
"
    shift; shift; shift;
fi;
########################################### LEADING ORDER (MADGRAPH) ###########################################


CFGFILE=$1; shift;
echo " $AREA/$CFGFILE "
if [[ "$CFGFILE" == "" ]] || test \! -f $AREA/$CFGFILE; then
    echo "Missing cfg file $CFGFILE under $AREA"
fi;

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

JOB=$TMPDIR/FASTSIM_CMG.cfg.py

cat $AREA/$CFGFILE  > $JOB
echo "process.maxEvents.input = -1"                              >> $JOB
echo "process.source.fileNames = [ $INFILES ]"                  >> $JOB
if echo $CFGFILE | grep '_CMG'; then
    echo "getattr(process.subProcess, '_SubProcess__process').outcmg.fileName = '$TMPDIR/$OUTBASE.root'"   >> $JOB
else
    echo "process.AODSIMoutput.fileName = '$TMPDIR/$OUTBASE.root'"   >> $JOB
fi;
cat >> $JOB <<_EOF_
## If needed, select events to process
$EVENTS
## Scramble
import random
rnd = random.SystemRandom()
for X in process.RandomNumberGeneratorService.parameterNames_(): 
   if X != 'saveFileName': getattr(process.RandomNumberGeneratorService,X).initialSeed = rnd.randint(1,99999999)
_EOF_

cd $TMPDIR
echo ${CMSSSW_BASE}
echo ${CMSSSW_RELEASE_BASE}
cmsRun $JOB 2>&1 | tee FASTSIM_CMG_$OUTBASE.log | grep -v '^\(Time\|Trig\)Report'
echo "cmsStageIn $TMPDIR/$OUTBASE.root $OUTFILE" | tee -a FASTSIM_CMG_$OUTBASE.log
test -f $TMPDIR/$OUTBASE.root && cmsStageIn $TMPDIR/$OUTBASE.root $OUTFILE 2>&1 | tee -a FASTSIM_CMG_$OUTBASE.log
perl  ~/sh/skimreport  --path=generation_step --all FASTSIM_CMG_$OUTBASE.log | grep -v HLT > $AREA/jobs/$OUTBASE.skimreport
gzip FASTSIM_CMG_$OUTBASE.log && cp FASTSIM_CMG_$OUTBASE.log.gz $AREA/jobs
