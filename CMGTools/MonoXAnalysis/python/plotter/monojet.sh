#!/bin/bash

WHAT=$1; if [[ "$1" == "" ]]; then echo "monojet.sh <what>"; exit 1; fi

if [[ "$HOSTNAME" == "cmsphys06" ]]; then
    T="/data1/emanuele/monox/TREES_040515_MET200SKIM";
    J=6;
else
    T="/cmshome/dimarcoe/TREES_040515_MET200SKIM";
    J=6;
fi
COREOPT="-P $T --s2v -j $J -l 5.0"
COREY="mcAnalysis.py ${COREOPT} -G  "
COREP="mcPlots.py  ${COREOPT} -f --poisson "
FEV=" -F mjvars/t \"$T/0_eventvars_mj_v1/evVarFriend_{cname}.root\" "

ROOT="plots/050515/v1.0/$WHAT"

RUNY="${COREY} mca-Phys14.txt --s2v "
RUNYSR="${RUNY} sr/monojet.txt "

PLOT="${COREP} mca-Phys14.txt --s2v "
PLOTSR="${PLOT} sr/monojet.txt sr/monojet-plots.txt --noStackSig --showSigShape "

case $WHAT in
sr)
        #SF="-W 'puWeight'"
        SF=" "
        echo "python ${RUNYSR} $FEV $SF "
        echo "python ${PLOTSR}  $FEV $SF "
;;
wmunu)
        echo "python ${RUNY} control-samples/w_munu.txt $FEV --sp WJets --xp M10V "
        echo "python ${PLOT} control-samples/w_munu.txt control-samples/w_munu_plots.txt $FEV --sp WJets --xp M10V --pdir plots/wmunu "
;;
esac;
