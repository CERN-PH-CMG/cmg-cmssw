#!/bin/bash

WHAT=$1; if [[ "$1" == "" ]]; then echo "monojet.sh <what>"; exit 1; fi

BASEDIR=""
if [ "$WHAT" == "zee" ] || [ "$WHAT" == "zeeI" ] || [ "$WHAT" == "wenI" ] || [ "$WHAT" == "wen" ] ; then
    BASEDIR="TREES_25ns_1LEPSKIM_23NOV2015"
else 
    BASEDIR="TREES_25ns_MET200SKIM_1DEC2015"
fi

WORKDIR="monojet"

if [[ "$HOSTNAME" == "cmsphys06" ]]; then
    T="/data1/emanuele/monox/${BASEDIR}";
    #T="/afs/cern.ch/work/e/emanuele/Trees/"
    J=8;
else
    T="/cmshome/dimarcoe/${BASEDIR}";
    J=3;
fi

MCA=""
if [ "$WHAT" == "wen" ] || [ "$WHAT" == "zee" ] ; then
    MCA="${WORKDIR}/mca-74X-Ve.txt "
elif [ "$WHAT" == "wmn" ] || [ "$WHAT" == "zmm" ] ; then
    MCA="${WORKDIR}/mca-74X-Vm.txt "
else
    MCA="${WORKDIR}/mca-74X-analysis.txt "
fi

ROOT="plots/Summer15/v1.0/$WHAT"
COREOPT="-P $T --s2v -j $J -l 2.215 -W vtxWeight "
COREY="mcAnalysis.py ${MCA} ${COREOPT} -G  "
COREP="mcPlots.py ${MCA} ${COREOPT} -f --poisson --pdir ${ROOT} --showRatio --maxRatioRange 0 2 "
FEV=" -F mjvars/t \"$T/friends/evVarFriend_{cname}.root\" "

RUNYSR="${COREY} ${WORKDIR}/monojet_twiki.txt "
RUNY2M="${COREY} ${WORKDIR}/zmumu_twiki.txt "
RUNY2E="${COREY} ${WORKDIR}/zee_twiki.txt "
RUNY1M="${COREY} ${WORKDIR}/wmunu_twiki.txt "
RUNY1E="${COREY} ${WORKDIR}/wenu_twiki.txt "
RUNY1G="${COREY} ${WORKDIR}/gjets_twiki.txt "

PLOTSR="${COREP} ${WORKDIR}/monojet_twiki.txt ${WORKDIR}/monojet-plots.txt --noStackSig --showSigShape "
PLOT2M="${COREP} ${WORKDIR}/zmumu_twiki.txt ${WORKDIR}/zmumu_plots.txt "
PLOT2E="${COREP} ${WORKDIR}/zee_twiki.txt ${WORKDIR}/zee_plots.txt "
PLOT1M="${COREP} ${WORKDIR}/wmunu_twiki.txt ${WORKDIR}/wmunu_plots.txt "
PLOT1E="${COREP} ${WORKDIR}/wenu_twiki.txt ${WORKDIR}/wenu_plots.txt "

case $WHAT in
sr)
        SF=" "
        echo "python ${RUNYSR} $FEV $SF "
        echo "python ${PLOTSR} $FEV $SF "
;;
zmm)
        echo "python ${RUNY2M} $FEV --sp DYJetsHT "
        echo "python ${PLOT2M} $FEV --sp DYJetsHT "
;;
wmn)
        echo "python ${RUNY1M} $FEV --sp WJetsHT "
        echo "python ${PLOT1M} $FEV --sp WJetsHT "
;;
zee)
        echo "python ${RUNY2E} $FEV --sp DYJetsHT "
        echo "python ${PLOT2E} $FEV --sp DYJetsHT "
;;
wen)
        echo "python ${RUNY1E} $FEV --sp WJetsHT "
        echo "python ${PLOT1E} $FEV --sp WJetsHT "
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
