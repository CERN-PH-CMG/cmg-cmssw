#!/bin/bash

if [[ "$HOSTNAME" == "cmsphys06" ]]; then
    T="/data1/emanuele/monox/TREES_150515_MET200SKIM";
    J=6;
else
    T="/cmshome/dimarcoe/TREES_060515_MET200SKIM";
    J=6;
fi

OPTIONS=" -P $T --s2v -j $J -l 5.0 -f  --od cards/new "
SYSTS="systsEnv.txt"

if [[ "$1" == "" ]] ; then
    X="monojet"
    OPTIONS="${OPTIONS} -F mjvars/t \"$T/0_eventvars_mj_v1/evVarFriend_{cname}.root\" "
    echo "python makeShapeCards.py mca-Phys14.txt sr/monojet.txt  'metNoMu_pt' '6,200,1000' $SYSTS $OPTIONS -o ${X} "
    echo "Done at $(date)"
fi;
