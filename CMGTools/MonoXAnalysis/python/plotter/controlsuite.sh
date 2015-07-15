#!/bin/bash

WHAT=$1; if [[ "$1" == "" ]]; then echo "monojet.sh <what>"; exit 1; fi

if [[ "$HOSTNAME" == "cmsphys06" ]]; then
    T="/data1/emanuele/monox/TREES_SPRING15_50ns_2LEPSKIM_10JUL2015";
    J=6;
else
    T="/cmshome/dimarcoe/TREES_SPRING15_50ns_2LEPSKIM_10JUL2015";
    J=6;
fi

COREOPT="-P $T --s2v -j $J -l 0.0017"
COREY="mcAnalysis.py ${COREOPT} -G "
COREP="mcPlots.py  ${COREOPT} -f --poisson "
#FEV=" -F mjvars/t \"$T/0_eventvars_mj_v1/evVarFriend_{cname}.root\" "

ROOT="plots/Run2015B/v1.0/$WHAT"

RUNY="${COREY} mca-74X.txt "
RUNY2L="${RUNY} control-samples/zmumu.txt "

PLOT="${COREP} mca-74X.txt "
PLOT2L="${PLOT} control-samples/zmumu.txt control-samples/zmumu-plots.txt "

case $WHAT in
zmumu)
        echo "python ${RUNY2L} --sp DYJets "
        echo "python ${PLOT2L} --sp DYJets --pdir plots/Run2015B_Zmm "
esac;
