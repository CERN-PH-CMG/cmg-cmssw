#!/bin/bash

RINPUTS=0
if [[ "$1" == "-r" ]]; then
    echo "# will print the command to write the inputs for the R factors"
    RINPUTS=1; shift;
fi;

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
    MCA="${WORKDIR}/mca-74X-sr.txt "
fi

ROOTPREF="plots/Summer15/v2.0"
ROOT="${ROOTPREF}/${WHAT}"
ROOTR="${ROOTPREF}/transfer_factors"
COREOPT="-P $T --s2v -j $J -l 2.215 "
COREY="mcAnalysis.py ${MCA} ${COREOPT} -G  "
COREP="mcPlots.py ${MCA} ${COREOPT} -f --poisson --pdir ${ROOT} --showRatio --maxRatioRange 0.5 1.5 "
CORER="mcSystematics.py ${MCA} ${COREOPT} -f --select-plot \"metnomu\" "
FEV=" -F mjvars/t \"$T/friends/evVarFriend_{cname}.root\" "
SF=" --FM sf/t \"$T/friends/sfFriend_{cname}.root\" "

RUNYSR="${COREY} ${WORKDIR}/monojet_twiki.txt "
RUNY2M="${COREY} ${WORKDIR}/zmumu_twiki.txt "
RUNY2E="${COREY} ${WORKDIR}/zee_twiki.txt "
RUNY1M="${COREY} ${WORKDIR}/wmunu_twiki.txt "
RUNY1E="${COREY} ${WORKDIR}/wenu_twiki.txt "
RUNY1G="${COREY} ${WORKDIR}/gjets_twiki.txt "

PLOTSR="${COREP} ${WORKDIR}/monojet_twiki.txt ${WORKDIR}/monojet_plots.txt --noStackSig --showSigShape "
PLOT2M="${COREP} ${WORKDIR}/zmumu_twiki.txt ${WORKDIR}/zmumu_plots.txt "
PLOT2E="${COREP} ${WORKDIR}/zee_twiki.txt ${WORKDIR}/zee_plots.txt "
PLOT1M="${COREP} ${WORKDIR}/wmunu_twiki.txt ${WORKDIR}/wmunu_plots.txt "
PLOT1E="${COREP} ${WORKDIR}/wenu_twiki.txt ${WORKDIR}/wenu_plots.txt "

SYSTSR="${CORER} ${WORKDIR}/monojet_twiki.txt ${WORKDIR}/monojet_plots.txt monojet/syst_2l.txt "
SYST2M="${CORER} ${WORKDIR}/zmumu_twiki.txt ${WORKDIR}/zmumu_plots.txt monojet/syst_2l.txt "
SYST2E="${CORER} ${WORKDIR}/zee_twiki.txt ${WORKDIR}/zee_plots.txt monojet/syst_2l.txt "
SYST1M="${CORER} ${WORKDIR}/wmunu_twiki.txt ${WORKDIR}/wmunu_plots.txt monojet/syst_1l.txt "
SYST1E="${CORER} ${WORKDIR}/wenu_twiki.txt ${WORKDIR}/wenu_plots.txt monojet/syst_1l.txt "

case $WHAT in
sr)
        FULLOPT=" $FEV $SF -W 'vtxWeight*SF_trigmetnomu*SF_BTag*SF_NLO_QCD*SF_NLO_EWK' "
        if [[ "$RINPUTS" != "0" ]]; then
            echo "python ${SYSTSR} ${FULLOPT} -p 'ZNuNuHT,WJetsHT' -o ${ROOTR}/rinputs_SR.root "
        else
            echo "python ${RUNYSR} ${FULLOPT} "
            echo "python ${PLOTSR} ${FULLOPT} "
        fi;
;;
zmm)
        FULLOPT=" $FEV $SF -W 'vtxWeight*SF_trigmetnomu*SF_LepTightLoose*SF_BTag*SF_NLO_QCD*SF_NLO_EWK' "
        if [[ "$RINPUTS" != "0" ]]; then
            echo "python ${SYST2M} ${FULLOPT} -p DYJetsHT -o ${ROOTR}/rinputs_DYJetsHT_CR2MU.root "
        else
            echo "python ${RUNY2M} ${FULLOPT} --sp DYJetsHT "
            echo "python ${PLOT2M} ${FULLOPT} --sp DYJetsHT "
        fi;
;;
wmn)
        FULLOPT=" $FEV $SF -W 'vtxWeight*SF_trigmetnomu*SF_LepTight*SF_BTag*SF_NLO_QCD*SF_NLO_EWK' "
        if [[ "$RINPUTS" != "0" ]]; then
            echo "python ${SYST1M} ${FULLOPT} -p WJetsHT -o ${ROOTR}/rinputs_WJetsHT_CR1MU.root "
        else
            echo "python ${RUNY1M} ${FULLOPT} --sp WJetsHT "
            echo "python ${PLOT1M} ${FULLOPT} --sp WJetsHT "
        fi;
;;
zee)
        FULLOPT=" $FEV $SF -W 'vtxWeight*SF_trig1lep*SF_LepTightLoose*SF_BTag*SF_NLO_QCD*SF_NLO_EWK' "
        if [[ "$RINPUTS" != "0" ]]; then
            echo "python ${SYST2E} ${FULLOPT} -p DYJetsHT -o ${ROOTR}/rinputs_DYJetsHT_CR2E.root "
        else
            echo "python ${RUNY2E} ${FULLOPT} --sp DYJetsHT "
            echo "python ${PLOT2E} ${FULLOPT} --sp DYJetsHT "
        fi;
;;
wen)
        FULLOPT=" $FEV $SF -W 'vtxWeight*SF_trig1lep*SF_LepTight*SF_BTag*SF_NLO_QCD*SF_NLO_EWK' "
        if [[ "$RINPUTS" != "0" ]]; then
            echo "python ${SYST1E} ${FULLOPT} -p WJetsHT -o ${ROOTR}/rinputs_WJetsHT_CR1E.root "
        else
            echo "python ${RUNY1E} ${FULLOPT} --sp WJetsHT "
            echo "python ${PLOT1E} ${FULLOPT} --sp WJetsHT "
        fi;
;;
TF)
        echo "python monojet/prepareRFactors.py ZNuNuHT DYJetsHT ${ROOTR}/rinputs_SR.root ${ROOTR}/rinputs_DYJetsHT_CR2MU.root SR CR --pdir ${ROOTR}"
        echo "python monojet/prepareRFactors.py WJetsHT WJetsHT ${ROOTR}/rinputs_SR.root ${ROOTR}/rinputs_WJetsHT_CR1MU.root SR CR --pdir ${ROOTR}"
        echo "python monojet/prepareRFactors.py ZNuNuHT WJetsHT ${ROOTR}/rinputs_SR.root ${ROOTR}/rinputs_SR.root SR SR --pdir ${ROOTR}"
esac;
