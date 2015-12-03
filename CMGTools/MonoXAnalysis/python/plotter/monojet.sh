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

MCA=""
if [ "$WHAT" == "zmmI" ] || [ "$WHAT" == "zeeI" ] ; then
    MCA="mca-74X.txt "
elif [ "$WHAT" == "wmnI" ] || [ "$WHAT" == "wenI" ] ; then
    MCA="mca-74X-singleLep.txt "
else
    MCA="mca-74X-analysis.txt "
fi

RUNYSR="${COREY} ${MCA} sync/monojet_twiki.txt "
RUNY2M="${COREY} ${MCA} sync/zmumu_twiki.txt "
RUNY2E="${COREY} ${MCA} sync/zee_twiki.txt "
RUNY1M="${COREY} ${MCA} sync/wmunu_twiki.txt "
RUNY1G="${COREY} ${MCA} sync/gjets_twiki.txt "

PLOT="${COREP} ${MCA} "
PLOTSR="${PLOT} sr/monojet.txt sr/monojet-plots.txt --noStackSig --showSigShape "

case $WHAT in
sr)
        SF=" "
        echo "python ${RUNYSR} $FEV $SF "
        echo "python ${PLOTSR}  $FEV $SF --pdir ${ROOT} --showRatio "
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
wmnI)
        echo "python ${RUNY1M} $FEV --sp WJets --xp DYJetsHT --xp WJetsHT -X recoil -X jet100 -X jetclean -X dphijm "
        echo "python ${PLOT} sync/wmunu_twiki.txt sync/wmunu_incl_plots.txt $FEV --sp WJets  --xp DYJetsHT --xp WJetsHT -X recoil -X jet100 -X jetclean -X dphijm --pdir ${ROOT} --showRatio --scaleSigToData "
;;
wmn)
        echo "python ${RUNY1M} $FEV --sp WJetsHT --xp WJets --xp DYJets "
        echo "python ${PLOT} sync/wmunu_twiki.txt sync/wmunu_incl_plots.txt $FEV --sp WJetsHT --xp WJets --xp DYJets -X recoil -X jet100 -X jetclean -X dphijm --pdir ${ROOT} --showRatio --scaleSigToData "
;;
esac;
