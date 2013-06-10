#!/bin/bash

WHAT=$1; if [[ "$1" == "" ]]; then echo "frsuite.sh <what>"; exit 1; fi

if [[ "$HOSTNAME" == "cmsphys05" ]]; then
    T="/data/b/botta/TTHAnalysis/trees/TREES_250513_HADD";
    J=6;
elif [[ "$HOSTNAME" == "olsnba03" ]]; then
    T="/data/gpetrucc/TREES_250513_HADD";
    J=6;
else
    T="/afs/cern.ch/user/g/gpetrucc/w/TREES_250513_HADD";
    #T="/afs/cern.ch/user/g/gpetrucc/ttH/TREES_250513_LITE";
    J=5;
fi

CORE="mcPlots.py -P $T -j $J --print=png,pdf -l 19.6 --showRatio --maxRatioRange 0 2.4 -f   --doStatTest=chi2l"
CORE="${CORE} --FM sf/t $T/0_SFs_v2/sfFriend_{cname}.root "

ROOT="plots/250513/cr/$WHAT"

RUN2L="${CORE} mca-2lss-data.txt -A pt2010 lowj 'nJet25 == 3 || nJet25 == 2' "
RUN2E="${RUN2L} bins/2lss_ee.txt   bins/cr_2lss_lowj_plots.txt "
RUN2M="${RUN2L} bins/2lss_mumu.txt bins/cr_2lss_lowj_plots.txt --xp QF_data "
RUN2X="${RUN2L} bins/2lss_em.txt   bins/cr_2lss_lowj_plots.txt  "
J2="  -A pt2010 lowj2 'nJet25 == 3' "
J3="  -A pt2010 lowj3 'nJet25 == 3' "
BLOOSE=" -R 2b 1B '(nBJetLoose25>=2 || (nBJetMedium25==1 && nJet25 >=2))'"
### =============== 2L SS, 2b loose =================
case $WHAT in
2lss)
    SF="-W 'puWeight*Eff_2lep*SF_btag*SF_LepMVATight_2l*SF_LepTightCharge_2l*SF_trig2l'"
    echo "python $RUN2M $SF --pdir $ROOT/mm --rebin 3"
    echo "python $RUN2E $SF --pdir $ROOT/ee --rebin 3"
    echo "python $RUN2X $SF --pdir $ROOT/em --rebin 3"
    echo "python $RUN2M $SF --pdir $ROOT/mm/bloose2 ${BLOOSE} --rebin 3"
    echo "python $RUN2E $SF --pdir $ROOT/ee/bloose2 ${BLOOSE} --rebin 3"
    echo "python $RUN2X $SF --pdir $ROOT/em/bloose2 ${BLOOSE} --rebin 3"
    echo "python $RUN2M $SF --pdir $ROOT/mm/2j $J2 --rebin 3"
    echo "python $RUN2E $SF --pdir $ROOT/ee/2j $J2 --rebin 3"
    echo "python $RUN2M $SF --pdir $ROOT/mm/bloose2/2j ${BLOOSE} --rebin 3 $J2"
    echo "python $RUN2E $SF --pdir $ROOT/ee/bloose2/2j ${BLOOSE} --rebin 3 $J2"
    echo "python $RUN2M $SF --pdir $ROOT/mm/3j $J3 --rebin 3"
    echo "python $RUN2E $SF --pdir $ROOT/ee/3j $J3 --rebin 3"
    echo "python $RUN2M $SF --pdir $ROOT/mm/bloose2/3j ${BLOOSE} --rebin 3 $J3"
    echo "python $RUN2E $SF --pdir $ROOT/ee/bloose2/3j ${BLOOSE} --rebin 3 $J3"
;;
#2lss_loose)
#    SF="-W 'puWeight*Eff_2lep*SF_btag*SF_LepMVALoose_2l*SF_LepTightCharge_2l*SF_trig2l'"
#    echo "python ${RUN2M/2lss-data/2lss-loose-data} $SF --pdir $ROOT/mm ${LOOSE2L} "
#    echo "python ${RUN2E/2lss-data/2lss-loose-data} $SF --pdir $ROOT/ee ${LOOSE2L} "
#    echo "python ${RUN2M/2lss-data/2lss-loose-data} $SF --pdir $ROOT/mm/1B ${ONEB_METLD} ${LOOSE2L} "
#    echo "python ${RUN2E/2lss-data/2lss-loose-data} $SF --pdir $ROOT/ee/1B ${ONEB_METLD} ${LOOSE2L} "
#;;
#3l)
#    RUN3L="${CORE} mca-3l-data.txt bins/3l_auto.txt bins/cr_3l_lowj_plots.txt -A pt2010 lowj 'nJet25 <= 2' "
#    SF="-W 'puWeight*Eff_3lep*SF_btag*SF_LepMVALoose_3l'"
#   echo "python $RUN3L $SF --pdir $ROOT/3l --rebin 3"
#   echo "python $RUN3L $SF --pdir $ROOT/3l/1B ${ONEB_METLD} --rebin 2"
#;;
3l_tight)
    RUN3L="${CORE} mca-3l_tight-data.txt bins/3l_tight.txt bins/cr_3l_lowj_plots.txt -A pt2010 lowj 'nJet25 == 2' "
    SF="-W 'puWeight*Eff_3lep*SF_btag*SF_LepMVATight_3l'"
    echo "python $RUN3L $SF --pdir $ROOT/                  --rebin 3"
    echo "python $RUN3L $SF --pdir $ROOT/bloose2 ${BLOOSE} --rebin 3"
;;
#tt_2l)
#    CORE="${CORE/--maxRatioRange 0 2.4/--maxRatioRange 0.65 1.35}"
#    CORE="${CORE/--doStatTest=chi2l/} "
#    RUN2L="${CORE} mca-2l-data.txt bins/cr_tt_2l_em.txt bins/cr_2lss_lowj_plots.txt --xp QF_data "
#    SF="-W 'puWeight*Eff_2lep*SF_btag*SF_LepMVATight_2l*SF_LepTightCharge_2l*SF_trig2l'"
#    echo "python $RUN2L $SF --pdir $ROOT/ "
#    echo "python ${RUN2L/mca-2l-data.txt/mca-2l-data-topPtW.txt} $SF --pdir $ROOT/topPtW/ "
#;;
ttZ_3l)
    RUN3L="${CORE} mca-3l-data.txt bins/cr_ttz.txt bins/cr_ttz_plots.txt "
    SF="-W 'puWeight*Eff_3lep*SF_btag*SF_LepMVALoose_3l'"
    echo "python $RUN3L $SF --pdir $ROOT/ " 
    echo "python $RUN3L $SF --pdir $ROOT/4j -A 1B 4j 'nJet25 >= 4' --rebin 2"
;;
ttZ_3l_tight)
    RUN3L="${CORE} mca-3l_tight-data.txt bins/cr_ttz_tight.txt bins/cr_ttz_plots.txt "
    SF="-W 'puWeight*Eff_3lep*SF_btag*SF_LepMVATight_3l'"
    echo "python $RUN3L $SF --pdir $ROOT/ " 
    echo "python $RUN3L $SF --pdir $ROOT/4j -A 1B 4j 'nJet25 >= 4' --rebin 2"
;;
zz_4l)
    CORE="${CORE/--doStatTest=chi2l/} "
    CORE="${CORE/--maxRatioRange 0 2.4/--maxRatioRange 0.4 1.7}"
    RUN4L="${CORE} mca-4l-data.txt bins/cr_zz4l.txt bins/cr_zz4l_plots.txt "
    S0="-W 'puWeight*Eff_4lep*SF_btag'"
    SF="-W 'puWeight*Eff_4lep*SF_btag*SF_LepMVALoose_4l'"
    FMU="-A pt2010 4mu 'abs(LepGood1_pdgId) == 13 && abs(LepGood1_pdgId) == 13'"
    FEL="-A pt2010 4el 'abs(LepGood1_pdgId) == 11 && abs(LepGood1_pdgId) == 11'"
    echo "python $RUN4L $SF --pdir $ROOT/ " 
    echo "python $RUN4L $SF --pdir $ROOT/z4l/ -A pt2010 Z4l 'm4l > 80 && m4l < 106' " 
    echo "python $RUN4L $S0 --pdir $ROOT/noLepSF/ " 
    echo "python $RUN4L $S0 --pdir $ROOT/z4l/noLepSF/ -A pt2010 Z4l 'm4l > 80 && m4l < 106' " 
    echo "python $RUN4L $SF --pdir $ROOT/4mu/ $FMU" 
    echo "python $RUN4L $SF --pdir $ROOT/4e/  $FEL" 
;;
z_3l)
    CORE="${CORE/--doStatTest=chi2l/} "
    RUN="${CORE} mca-incl-dysplit.txt bins/cr_z_3l.txt bins/cr_z_3l_plots.txt  --scaleSigToData  --sp 'DY.*' "
    RNR="${RUN/--showRatio/} "
    SF="-W 'puWeight*Eff_3lep*SF_btag*SF_LepMVATight_2l' --mcc mcCorrections.txt";
    EL=" -A Z12 ele 'abs(LepGood1_pdgId) == 11' --sP 'l3.*' "
    MU=" -A Z12 ele 'abs(LepGood1_pdgId) == 13' --sP 'l3.*' "
    echo "python $RUN $SF --pdir $ROOT/ " 
    echo "python $RNR $SF --pdir $ROOT/noratio/ " 
    echo "python $RUN $SF --pdir $ROOT/el/ $EL " 
    echo "python $RNR $SF --pdir $ROOT/el/noratio/ $EL " 
    echo "python $RUN $SF --pdir $ROOT/mu/ $MU " 
    echo "python $RNR $SF --pdir $ROOT/mu/noratio/ $MU " 
;;
tt_3l)
    CORE="${CORE/--doStatTest=chi2l/} "
    RUN="${CORE} mca-ttsplit.txt bins/cr_tt_3l.txt bins/cr_tt_3l_plots.txt  --scaleSigToData  --sp 'TT[lb]'  "
    RNR="${RUN/--showRatio/} "
    SF="-W 'puWeight*Eff_3lep*SF_btag*SF_LepMVATight_2l' --mcc mcCorrections.txt";
    EL=" -A Z12 ele 'abs(LepGood1_pdgId) == 11' --sP 'l3.*' "
    MU=" -A Z12 ele 'abs(LepGood1_pdgId) == 13' --sP 'l3.*' "
    echo "python $RUN $SF --pdir $ROOT/ " 
    echo "python $RNR $SF --pdir $ROOT/noratio/ " 
    echo "python $RUN $SF --pdir $ROOT/el/ $EL " 
    echo "python $RNR $SF --pdir $ROOT/el/noratio/ $EL " 
    echo "python $RUN $SF --pdir $ROOT/mu/ $MU " 
    echo "python $RNR $SF --pdir $ROOT/mu/noratio/ $MU " 
;;
esac;

