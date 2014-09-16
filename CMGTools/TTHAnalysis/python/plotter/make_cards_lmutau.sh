#!/bin/bash

if [[ "$HOSTNAME" == "cmsphys10" ]]; then
    T="/data/g/gpetrucc/TREES_53X_170714";
    J=0;
else
    T="/afs/cern.ch/work/g/gpetrucc/TREES_53X_170714";
    J=4;
fi

SCENARIO=""
if echo "X$1" | grep -q "scenario"; then SCENARIO="$1"; shift; fi
OPTIONS=" -P $T -j $J  -l 19.7 --s2v -f --tree treeProducerSusyMultilepton --od lmutau-cards/ "
OPTIONS="${OPTIONS} --masses lmutau-masses.txt --mass-int-algo=none"
SYSTS="systsLMuTau.txt"

if [[ "$1" == "" ]] || echo $1 | grep -q high_3mu; then
    OPT_3L="${OPTIONS} -W puWeight*LepEff_3lep -p S3m_[1234].*,WZ,ZZ -X mtW100"
    BINS="[40,45,50,55,60,65,70,75,80,82,84,86,88,90,92,94,96,98,100,102,104,106,108,110,115,120,125,130,135,140,145,150,160,170,180,190,200,210,220,230,240,250,265,280,295,310,325,340,355,370,385,400]"
    python makeShapeCardsLMuTau.py mca-lmutau.txt bins/lmutau_3mu.txt 'minMllSFOS' $BINS $SYSTS $OPT_3L  -o high_3mu 
fi
if [[ "$1" == "" ]] || echo $1 | grep -q low_3mu; then
    OPT_3L="${OPTIONS} -W puWeight*LepEff_3lep -p S3m_[5-9].*,WZ,ZZ,DY -X mtW120"
    python makeShapeCardsLMuTau.py mca-lmutau.txt bins/lmutau_3mu.txt 'mZ1'  '40,40,120' $SYSTS $OPT_3L  -o low_3mu
fi        
