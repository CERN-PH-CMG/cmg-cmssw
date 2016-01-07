#!/bin/bash

WHAT=$1; if [[ "$1" == "" ]]; then echo "monojet.sh <what>"; exit 1; fi

BASEDIR=""
if [ "$WHAT" == "zee" ] || [ "$WHAT" == "zeeI" ] || [ "$WHAT" == "wenI" ] || [ "$WHAT" == "wen" ] ; then
    BASEDIR="TREES_25ns_1LEPSKIM_23NOV2015"
else 
    BASEDIR="TREES_25ns_MET200SKIM_1DEC2015"
fi

if [[ "$HOSTNAME" == "cmsphys06" ]]; then
    T="/data1/emanuele/monox/${BASEDIR}";
    #T="/afs/cern.ch/work/e/emanuele/Trees/"
    J=6;
else
    T="/cmshome/dimarcoe/${BASEDIR}";
    J=6;
fi

COREOPT="-P $T --s2v -j $J -l 2.215 -W vtxWeight "
COREY="mcAnalysis.py ${COREOPT} -G  "
COREP="mcPlots.py  ${COREOPT} -f --poisson "
FEV=" -F mjvars/t \"$T/friends/evVarFriend_{cname}.root\" "

ROOT="plots/Summer15/v1.0/$WHAT"

MCA=""
if [ "$WHAT" == "zee" ] || [ "$WHAT" == "zeeI" ] ; then
    MCA="mca-74X.txt "
elif [ "$WHAT" == "wmnI" ] || [ "$WHAT" == "wmn" ] || [ "$WHAT" == "zmmI" ] || [ "$WHAT" == "zmm" ] ; then
    MCA="mca-74X-Vm.txt "
else
    MCA="mca-74X-analysis.txt "
fi

RUNYSR="${COREY} ${MCA} monojet_twiki.txt "
RUNY2M="${COREY} ${MCA} zmumu_twiki.txt "
RUNY2E="${COREY} ${MCA} zee_twiki.txt "
RUNY1M="${COREY} ${MCA} wmunu_twiki.txt "
RUNY1G="${COREY} ${MCA} gjets_twiki.txt "

PLOT="${COREP} ${MCA} "
PLOTSR="${PLOT} sr/monojet.txt sr/monojet-plots.txt --noStackSig --showSigShape "

case $WHAT in
sr)
        SF=" "
        echo "python ${RUNYSR} $FEV $SF "
        echo "python ${PLOTSR}  $FEV $SF --pdir ${ROOT} --showRatio --maxRatioRange 0 2 "
;;
zmm)
        echo "python ${RUNY2M} $FEV --sp DYJetsHT --xp WJets --xp DYJets "
        echo "python ${PLOT} zmumu_twiki.txt zmumu_plots.txt $FEV --sp DYJetsHT --xp WJets --xp DYJets --pdir ${ROOT} --showRatio --maxRatioRange 0 2 "
;;
wmn)
        echo "python ${RUNY1M} $FEV --sp WJetsHT --xp WJets --xp DYJets "
        echo "python ${PLOT} wmunu_twiki.txt wmunu_plots.txt $FEV --sp WJetsHT --xp WJets --xp DYJets -X recoil -X jet100 -X jetclean -X dphijm --pdir ${ROOT} --showRatio --maxRatioRange 0 2 "
;;
zee)
        echo "python ${RUNY2E} $FEV --sp DYJetsHT "
        echo "python ${PLOT} zee_twiki.txt zee_plots.txt $FEV --sp DYJetsHT --pdir ${ROOT} --showRatio --maxRatioRange 0 2 "
;;
# zmmI)
#         echo "python ${RUNY2M} -X recoil -X jet100 -X jetclean -X dphijm $FEV --sp DYJets "
#         echo "python ${PLOT} zmumu_twiki.txt control-samples/zmumu_incl_plots.txt -X recoil -X jet100 -X jetclean -X dphijm $FEV --sp DYJets --pdir ${ROOT} --showRatio --maxRatioRange 0 2 "
# ;;
# zeeI)
#         echo "python ${RUNY2E} -X recoil -X jet100 -X jetclean -X dphijm $FEV --sp DYJets "
#         echo "python ${PLOT} zee_twiki.txt control-samples/zee_incl_plots.txt -X recoil -X jet100 -X jetclean -X dphijm $FEV --sp DYJets --pdir ${ROOT} --showRatio --maxRatioRange 0 2 "
# ;;
# wmnI)
#         echo "python ${RUNY1M} $FEV --sp WJets --xp DYJetsHT --xp WJetsHT -X recoil -X jet100 -X jetclean -X dphijm "
#         echo "python ${PLOT} wmunu_twiki.txt wmunu_incl_plots.txt $FEV --sp WJets  --xp DYJetsHT --xp WJetsHT -X recoil -X jet100 -X jetclean -X dphijm --pdir ${ROOT} --showRatio --maxRatioRange 0 2 "
# ;;
esac;
