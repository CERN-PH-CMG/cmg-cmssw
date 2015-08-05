#!/bin/bash

WHAT=$1; if [[ "$1" == "" ]]; then echo "monojet.sh <what>"; exit 1; fi

if [[ "$HOSTNAME" == "cmsphys06" ]]; then
    T="/data1/emanuele/monox/TREES_1LEPSKIM_30July2015";
    J=6;
else
    T="/cmshome/dimarcoe/TREES_1LEPSKIM_30July2015";
    J=6;
fi

LUMI="0.04024"
OPTCAPTION="--rspam '#sqrt{s} = 13 TeV, L = %(lumipb).1f pb^{-1}' "

COREOPT="-P $T --s2v -j $J -l $LUMI -W vtxW "
COREY="mcAnalysis.py ${COREOPT} -G "
COREP="mcPlots.py  ${COREOPT} -f --poisson "
FEV=" -F mjvars/t \"$T/0_eventvars_mj_v1/evVarFriend_{cname}.root\" "

ROOT="plots/Run2015B/v1.0/$WHAT"

RUNY="${COREY} mca-74X.txt "
SEL2MU="control-samples/zmumuincl.txt "
SEL2E="control-samples/zeeincl.txt "


RUNY2MU="${RUNY} ${SEL2MU} "
RUNY2E="${RUNY} ${SEL2E} "

PLOT="${COREP} mca-74X.txt $OPTCAPTION "
PLOT2MU="${PLOT} ${SEL2MU} control-samples/zmumu-plots.txt "
PLOT2E="${PLOT} ${SEL2E} control-samples/zee-plots.txt "

case $WHAT in
zmumu)
        echo "python ${RUNY2MU} $FEV --sp DYJets "
        echo "python ${PLOT2MU} $FEV --sp DYJets --pdir plots/Run2015B_Zmm "
;;
zee)
        echo "python ${RUNY2E} $FEV --sp DYJets "
        echo "python ${PLOT2E} $FEV --sp DYJets --pdir plots/Run2015B_Zee "
;;
zee_bb)
        echo "python ${RUNY2E} $FEV --sp DYJets -A muVeto ebeb 'abs(LepGood1_eta) < 1.479 && abs(LepGood2_eta) < 1.479' "
        echo "python ${PLOT2E} $FEV --sp DYJets -A muVeto ebeb 'abs(LepGood1_eta) < 1.479 && abs(LepGood2_eta) < 1.479' --pdir plots/Run2015B_Zee_EBEB --print=pdf,png,C,root --scaleSigToData "
;;
zee_ee)
        echo "python ${RUNY2E} $FEV --sp DYJets -A muVeto ebeb 'abs(LepGood1_eta) > 1.479 && abs(LepGood2_eta) > 1.479' "
        echo "python ${PLOT2E} $FEV --sp DYJets -A muVeto ebeb 'abs(LepGood1_eta) > 1.479 && abs(LepGood2_eta) > 1.479' --pdir plots/Run2015B_Zee_EEEE --print=pdf,png,C,root --scaleSigToData "
;;
esac;
