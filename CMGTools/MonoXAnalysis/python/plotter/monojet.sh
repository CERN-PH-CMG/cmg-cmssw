#!/bin/bash

WHAT=$1; if [[ "$1" == "" ]]; then echo "monojet.sh <what>"; exit 1; fi

if [[ "$HOSTNAME" == "cmsphys06" ]]; then
    T="/data1/emanuele/monox/TREES_25ns_1LEPSKIM_23NOV2015";
    J=6;
else
    T="/cmshome/dimarcoe/TREES_060515_MET200SKIM";
    J=6;
fi
COREOPT="-P $T --s2v -j $J -l 2.11 -W vtxWeight "
COREY="mcAnalysis.py ${COREOPT} -G  "
COREP="mcPlots.py  ${COREOPT} -f --poisson "
FEV=" -F mjvars/t \"$T/friends/evVarFriend_{cname}.root\" "

ROOT="plots/Summer15/v1.0/$WHAT"

RUNY=""
if [ "$WHAT" == "zmmI" ] || [ "$WHAT" == "zeeI" ] ; then
    RUNY="${COREY} mca-74X.txt --s2v "
else
    RUNY="${COREY} mca-74X-analysis.txt --s2v "
fi

RUNYSR="${RUNY} sync/monojet_twiki.txt "
RUNY2M="${RUNY} sync/zmumu_twiki.txt "
RUNY2E="${RUNY} sync/zee_twiki.txt "
RUNY1M="${RUNY} sync/wmunu_twiki.txt "
RUNY1G="${RUNY} sync/gjets_twiki.txt "

PLOT="${COREP} mca-74X.txt --s2v "
PLOTSR="${PLOT} sr/monojet.txt sr/monojet-plots.txt --noStackSig --showSigShape "

case $WHAT in
sr)
        SF=" "
        echo "python ${RUNYSR} $FEV $SF "
        echo "python ${PLOTSR}  $FEV $SF --pdir plots/sr "
;;
wmunu)
        echo "python ${RUNY} control-samples/w_munu.txt $FEV --sp WJets --xp M10V "
        echo "python ${PLOT} control-samples/w_munu.txt control-samples/w_munu-plots.txt $FEV --sp WJets --xp M10V --pdir plots/wmunu "
;;
zmm)
        echo "python ${RUNY2M} $FEV --sp DYJetsHT "
        echo "python ${PLOT} sync/zmumu_twiki.txt sync/zmumu_plots.txt $FEV --sp DYJetsHT --pdir ${ROOT} --showRatio "
;;
zee)
        echo "python ${RUNY2E} $FEV --sp DYJetsHT "
        echo "python ${PLOT} sync/zee_twiki.txt sync/zee_plots.txt $FEV --sp DYJetsHT --pdir ${ROOT} --showRatio "
;;
zmmI)
        echo "python ${RUNY2M} -X recoil -X jet100 -X jetclean -X dphijm $FEV --sp DYJets "
        echo "python ${PLOT} sync/zmumu_twiki.txt sync/zmumu_incl_plots.txt -X recoil -X jet100 -X jetclean -X dphijm $FEV --sp DYJets --pdir ${ROOT} --showRatio --scaleSigToData "
;;
zeeI)
        echo "python ${RUNY2E} -X recoil -X jet100 -X jetclean -X dphijm $FEV --sp DYJets "
        echo "python ${PLOT} sync/zee_twiki.txt sync/zee_incl_plots.txt -X recoil -X jet100 -X jetclean -X dphijm $FEV --sp DYJets --pdir ${ROOT} --showRatio --scaleSigToData "
;;
esac;
