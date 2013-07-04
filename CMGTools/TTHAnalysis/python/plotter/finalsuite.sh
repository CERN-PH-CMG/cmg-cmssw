#!/bin/bash

WHAT=$1; if [[ "$1" == "" ]]; then echo "frsuite.sh <what>"; exit 1; fi

if [[ "$HOSTNAME" == "cmsphys05" ]]; then
    T="/data/b/botta/TTHAnalysis/trees/TREES_250513_HADD";
    J=6;
elif [[ "$HOSTNAME" == "olsnba03" ]]; then
    T="/data/gpetrucc/TREES_250513_HADD";
    J=12;
elif [[ "$HOSTNAME" == "lxbse14c09.cern.ch" ]]; then
    T="/var/ssdtest/gpetrucc/TREES_250513_HADD";
    J=6;
else
    T="/afs/cern.ch/work/g/gpetrucc/TREES_250513_HADD";
    J=6;
fi
CORE="mcPlots.py -P $T -j $J -f -l 19.6  "
FSF=" --FM sf/t $T/0_SFs_v2/sfFriend_{cname}.root "

ROOT="plots/250513/.unblinded/$WHAT"

RUN2L="${CORE} mca-2lss-data.txt --showRatio --maxRatioRange 0 3.7 --poisson"
RUN2E="${RUN2L} bins/2lss_ee_tight.txt   bins/mvaVars_2lss.txt  "
RUN2M="${RUN2L} bins/2lss_mumu_tight.txt bins/mvaVars_2lss.txt --xp QF_data "
RUN2X="${RUN2L} bins/2lss_em_tight.txt   bins/mvaVars_2lss.txt  "
BINCL=" -X 2B"
BLOOSE=" -I 2B"
BMEDIUM=" -R 2B 2b1B '(nBJetLoose25>=2 && nBJetMedium25>=1)'"
B2LOOSE=" -R 2B 2bl '(nBJetLoose25>=2)'"
BTIGHT=" "
### =============== 2L SS, 2b loose =================
### =============== 2L SS, 2b loose =================
case $WHAT in
2lss)
    SF="$FSF -W 'puWeight*Eff_2lep*SF_btag*SF_LepMVATight_2l*SF_LepTightCharge_2l*SF_trig2l'"
    MVA=" -F sf/t   $T/2_finalmva_2lss_v2/evVarFriend_{cname}.root "
    
    #echo "python $RUN2M $SF $MVA --pdir $ROOT/mm/nostack $BINCL --rebin 4 --noStackSig --showSigShape"
    #echo "python $RUN2E $SF $MVA --pdir $ROOT/ee/nostack $BINCL --rebin 6 --noStackSig --showSigShape"
    #echo "python $RUN2X $SF $MVA --pdir $ROOT/em/nostack $BINCL --rebin 4 --noStackSig --showSigShape"

    echo "python ${RUN2X/mvaVars_2lss/cr_2lss_lowj_plots} $MVA $SF --pdir $ROOT/kinematics/em   $BINCL --rebin 2   "
    echo "python ${RUN2E/mvaVars_2lss/cr_2lss_lowj_plots} $MVA $SF --pdir $ROOT/kinematics/ee   $BINCL --rebin 4   "
    echo "python ${RUN2M/mvaVars_2lss/cr_2lss_lowj_plots} $MVA $SF --pdir $ROOT/kinematics/mumu $BINCL --rebin 2   "
    echo "python ${RUN2X/mvaVars_2lss/cr_2lss_lowj_plots} $MVA $SF --pdir $ROOT/kinematics/bloose/em   $BLOOSE --rebin 2   "
    echo "python ${RUN2E/mvaVars_2lss/cr_2lss_lowj_plots} $MVA $SF --pdir $ROOT/kinematics/bloose/ee   $BLOOSE --rebin 4   "
    echo "python ${RUN2M/mvaVars_2lss/cr_2lss_lowj_plots} $MVA $SF --pdir $ROOT/kinematics/bloose/mumu $BLOOSE --rebin 2   "
    echo "python ${RUN2X/mvaVars_2lss/cr_2lss_lowj_plots} $MVA $SF --pdir $ROOT/kinematics/btight/em   $BTIGHT --rebin 4   "
    echo "python ${RUN2E/mvaVars_2lss/cr_2lss_lowj_plots} $MVA $SF --pdir $ROOT/kinematics/btight/ee   $BTIGHT --rebin 6   "
    echo "python ${RUN2M/mvaVars_2lss/cr_2lss_lowj_plots} $MVA $SF --pdir $ROOT/kinematics/btight/mumu $BTIGHT --rebin 4   "

    echo "python ${RUN2X} $MVA $SF --pdir $ROOT/em   $BINCL --rebin 2   "
    echo "python ${RUN2E} $MVA $SF --pdir $ROOT/ee   $BINCL --rebin 4   "
    echo "python ${RUN2M} $MVA $SF --pdir $ROOT/mumu $BINCL --rebin 2   "
    echo "python ${RUN2X} $MVA $SF --pdir $ROOT/bloose/em   $BLOOSE --rebin 2   "
    echo "python ${RUN2E} $MVA $SF --pdir $ROOT/bloose/ee   $BLOOSE --rebin 4   "
    echo "python ${RUN2M} $MVA $SF --pdir $ROOT/bloose/mumu $BLOOSE --rebin 2   "
    echo "python ${RUN2X} $MVA $SF --pdir $ROOT/btight/em   $BTIGHT --rebin 4   "
    echo "python ${RUN2E} $MVA $SF --pdir $ROOT/btight/ee   $BTIGHT --rebin 6   "
    echo "python ${RUN2M} $MVA $SF --pdir $ROOT/btight/mumu $BTIGHT --rebin 4   "

;;
3l_tight)
    RUN3L="${CORE} mca-3l_tight-data.txt bins/3l_tight.txt bins/mvaVars_3l.txt  --showRatio --maxRatioRange 0 3.7 "
    SF="$FSF -W 'puWeight*Eff_3lep*SF_btag*SF_LepMVATight_3l'"
    MVA="-F finalMVA/t $T/0_finalmva_3l/finalMVA_3L_{cname}.root"
    echo "python $RUN3L $SF $MVA --pdir $ROOT/        $BINCL  --rebin 5"
    echo "python $RUN3L $SF $MVA --pdir $ROOT/bloose  $BLOOSE --rebin 5"
    echo "python $RUN3L $SF $MVA --pdir $ROOT/btight  $BTIGHT --rebin 5"
;;
4l)
    RUN4L="${CORE} mca-4l-ttscale.txt bins/4l.txt bins/mvaVars_4l.txt "
    MVA="-F finalMVA/t $T/0_finalmva_4l/finalMVA_4L_{cname}.root"
    SF="$FSF -W 'puWeight*Eff_4lep*SF_btag*SF_LepMVALoose_4l'"
    echo "python $RUN4L $SF --pdir $ROOT/        $BINCL "
    echo "python $RUN4L $SF --pdir $ROOT/bloose  $BLOOSE"
    echo "python $RUN4L $SF --pdir $ROOT/btight  $BTIGHT"
;;
esac;

