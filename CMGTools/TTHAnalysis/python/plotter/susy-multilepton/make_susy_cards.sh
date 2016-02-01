#!/bin/bash

if [[ "$1" == "afs" ]]; then
    T="~/work/ra5trees/skimmed_mix_nov22";
    J=4;
elif [[ "$HOSTNAME" == "cmsphys10" ]]; then
    T="/data1/p/peruzzi/skimmed_mix_nov22";
    J=8;
else
    T="~/work/ra5trees/skimmed_mix_nov22";
    J=4;
fi

LUMI=2.16
OUTDIR="susy_cards"
OPTIONS=" -P $T --neg --s2v --tree treeProducerSusyMultilepton -F sf/t {P}/5_allnewfriends_v7/evVarFriend_{cname}.root --mcc susy-multilepton/susy_2lssinc_triggerdefs.txt -f -j $J --od $OUTDIR --FMC sf/t {P}/1_purew_mix_true_nvtx/evVarFriend_{cname}.root -W vtxWeight*btagMediumSF_Mini*triggerSF_Loop*leptonSF_Loop -l $LUMI --plotgroup _fakesappl_data+=_promptratesub --plotgroup _fakesappl_data_ewk_Up+=_promptratesub_ewk_Up --plotgroup _fakesappl_data_ewk_Dn+=_promptratesub_ewk_Dn  -X lep1_pt25 -X lep2_pt25 -A alwaystrue SR_ii SR>0 --xp _standard_fakes_.* --xp _sig_SMS_.*"
OPTIONS="$OPTIONS --postfix-pred (?!_fakesappl_data).**=cutCentralValueAtZero --postfix-pred _fakesappl_data*=takeFakesPredictionFromMC"


function makeCard_2lss {
    local EXPR=$1; local BINS=$2; local SYSTS=$3; local OUT=$4; local GO=$5

    if [[ "$PRETEND" == "1" ]]; then
        echo "making datacard $OUT from makeShapeCardsSusy.py susy-multilepton/mca-Spring15-analysis-all.txt susy-multilepton/susy_2lss_multiiso.txt \"$EXPR\" \"$BINS\" $SYSTS $GO;"
    else
        echo "making datacard $OUT from makeShapeCardsSusy.py susy-multilepton/mca-Spring15-analysis-all.txt susy-multilepton/susy_2lss_multiiso.txt \"$EXPR\" \"$BINS\" $SYSTS $GO;"
        python makeShapeCardsSusy.py susy-multilepton/mca-Spring15-analysis-all.txt susy-multilepton/susy_2lss_multiiso.txt "$EXPR" "$BINS" $SYSTS -o $OUT $GO;
        echo "  -- done at $(date)";
    fi;
}

function combineCardsSmart {
    CMD=""
    for C in $*; do
        # missing datacards 
        test -f $C || continue;
        # datacards with no event yield
        grep -q "observation 0.0$" $C && continue
        CMD="${CMD} $(basename $C .card.txt)=$C ";
    done
    if [[ "$CMD" == "" ]]; then
        echo "Not any card found in $*" 1>&2 ;
    else
        combineCards.py $CMD
    fi
}

if [[ "$1" == "--pretend" ]]; then
    PRETEND=1; shift;
fi;

if [[ "$1" == "ra5" ]]; then
    SYSTS="susy-multilepton/syst/susyRa5.txt"
    CnC_expr="SR"
    CnC_bins="66,0.5,66.5"

    makeCard_2lss $CnC_expr $CnC_bins $SYSTS SR "$OPTIONS";

    echo "Done at $(date)";

elif [[ "$1" == "ra5_MI_highHT" ]]; then
    SYSTS="susy-multilepton/syst/susyRa5_MI_highHT.txt"
    CnC_expr="1"
    CnC_bins="1,0.5,1.5"

    if [[ "$3" == "HT" ]]; then
    makeCard_2lss $CnC_expr $CnC_bins $SYSTS SR_$2_HT$4 "$OPTIONS -A alwaystrue HT300 htJet40j_Mini>300 -A alwaystrue SR_$2 SR==$2 --postfix-pred _sig_.**=normTo1Observed -A alwaystrue HT_$4 htJet40j_Mini>$4";
    elif [[ "$3" == "MET" ]]; then
    makeCard_2lss $CnC_expr $CnC_bins $SYSTS SR_$2_MET$4 "$OPTIONS -A alwaystrue HT300 htJet40j_Mini>300 -A alwaystrue SR_$2 SR==$2 --postfix-pred _sig_.**=normTo1Observed -A alwaystrue MET_$4 met_pt>$4";
    fi
    echo "Done at $(date)";

#elif [[ "$1" == "2lss-2012" ]]; then
#    OPTIONS=" $OPTIONS -F sf/t $T/1_susyVars_2lssInc_v0/evVarFriend_{cname}.root "
#    SYSTS="syst/susyDummy.txt"
#    CnC_expr="1+4*(met_pt>120)+(htJet40j>400)+2*(nJet40>=4)"
#    CnC_bins="[0.5,1.5,2.5,3.5,4.5,5.5,6.5,7.5,8.5]"
#    MOD=inc;
#
#    echo "Making individual datacards"
#    for LL in ee em mm; do for LPt in 2020; do for SR in 0X 1X 2X+; do
#        echo " --- CnC2012_${SR}_${LL} ---"
#        #makeCard_2lss $CnC_expr $CnC_bins $SYSTS CnC2012_${SR}_${LL} "$OPTIONS";
#    done; done; done
#    echo "Making combined datacards"
#    for D in $OUTDIR/T[0-9]*; do
#        test -f $D/CnC2012_0X_ee.card.txt || continue
#        (cd $D;
#            for SR in 0X 1X 2X+; do
#                combineCards.py CnC2012_${SR}_{ee,em,mm}.card.txt >  CnC2012_${SR}.card.txt
#            done
#            combineCards.py CnC2012_{0X,1X,2X+}.card.txt >  CnC2012.card.txt
#        );
#        echo "Made combined card $D/CnC2012.card.txt"
#    done
#    echo "Done at $(date)";
#
#elif [[ "$1" == "2lss-2015" ]]; then
#    OPTIONS=" $OPTIONS -F sf/t $T/1_susyVars_2lssInc_v0/evVarFriend_{cname}.root "
#    SYSTS="syst/susyDummy.txt"
#    CnC_expr="1+4*(met_pt>120)+(htJet40j>400)+2*(nJet40>=4)"
#    CnC_bins="[0.5,1.5,2.5,3.5,4.5,5.5,6.5,7.5,8.5]"
#    MOD=inc;
#
#    echo "Making individual datacards"
#    for LL in ee em mm; do for LPt in hh hl ll; do for SR in 0X 1X 2X 3X 2X+; do
#    #for LL in ee em mm; do for LPt in hh hl ll ; do for SR in 0Xs 1Xs 2Xs 3Xs; do
#        echo " --- CnC2015_${SR}_${LL}_${LPt} ---"
#        makeCard_2lss $CnC_expr $CnC_bins $SYSTS CnC2015_${SR}_${LL}_${LPt} "$OPTIONS";
#    done; done; done
#    #exit
#    echo "Making combined datacards"
#    for D in $OUTDIR/T[0-9]*; do
#        test -f $D/CnC2015_0X_ee_hh.card.txt || continue
#        (cd $D && echo "    $D";
#        for SR in 0X 1X 2X 3X 2X+; do
#        #for SR in 0Xs 1Xs 2Xs 3Xs; do
#            combineCardsSmart CnC2015_${SR}_{ee,em,mm}_hh.card.txt >  CnC2015_${SR}_hh.card.txt
#            combineCardsSmart CnC2015_${SR}_{ee,em,mm}_{hh,hl,ll}.card.txt >  CnC2015_${SR}.card.txt
#        done
#        combineCardsSmart CnC2015_{0X,1X,2X+}.card.txt   >  CnC2015_2b.card.txt
#        combineCardsSmart CnC2015_{0X,1X,2X+}_hh.card.txt   >  CnC2015_2b_hh.card.txt
#        combineCardsSmart CnC2015_{0X,1X,2X,3X}_hh.card.txt >  CnC2015_3b_hh.card.txt
#        combineCardsSmart CnC2015_{0X,1X,2X,3X}.card.txt >  CnC2015_3b.card.txt
#        #combineCardsSmart CnC2015_{0Xs,1Xs,2Xs,3Xs}_hh.card.txt >  CnC2015_3bs_hh.card.txt
#        #combineCardsSmart CnC2015_{0Xs,1Xs,2Xs,3Xs}.card.txt >  CnC2015_3bs.card.txt
#        )
#    done
#    echo "Done at $(date)";
#
#elif [[ "$1" == "2lss-2015x" ]]; then
#    OPTIONS=" $OPTIONS -F sf/t $T/1_susyVars_2lssInc_v0/evVarFriend_{cname}.root "
#    SYSTS="syst/susyDummy.txt"
#    CnC_expr="1+4*(met_pt>120)+(htJet40j>400)+2*(nJet40>=4)"
#    CnC_bins="[0.5,1.5,2.5,3.5,4.5,5.5,6.5,7.5,8.5]"
#    MOD=excl;
#
#    echo "Making individual datacards"
#    for LL in ee em mm 3l; do for LPt in hh hl ll; do for SR in 0X 1X 2X 3X; do
#        echo " --- CnC2015X_${SR}_${LL}_${LPt} ---"
#        makeCard_2lss $CnC_expr $CnC_bins $SYSTS CnC2015X_${SR}_${LL}_${LPt} "$OPTIONS";
#    done; done; done
#    #exit
#    echo "Making combined datacards"
#    for D in $OUTDIR/T[0-9]*; do
#        test -f $D/CnC2015X_0X_ee_hh.card.txt || continue
#        (cd $D && echo "    $D";
#        for SR in 0X 1X 2X 3X; do
#            combineCardsSmart CnC2015X_${SR}_{ee,em,mm}_hh.card.txt >  CnC2015X_${SR}_hh.card.txt
#            combineCardsSmart CnC2015X_${SR}_{ee,em,mm}_{hh,hl,ll}.card.txt >  CnC2015X_${SR}.card.txt
#            combineCardsSmart CnC2015X_${SR}_{ee,em,mm,3l}_hh.card.txt >  CnC2015X_${SR}_hh_w3l.card.txt
#            combineCardsSmart CnC2015X_${SR}_{ee,em,mm,3l}_{hh,hl,ll}.card.txt >  CnC2015X_${SR}_w3l.card.txt
#        done
#        combineCardsSmart CnC2015X_{0X,1X,2X,3X}_hh.card.txt >  CnC2015X_3b_hh.card.txt
#        combineCardsSmart CnC2015X_{0X,1X,2X,3X}.card.txt >  CnC2015X_3b.card.txt
#        combineCardsSmart CnC2015X_{0X,1X,2X,3X}_hh_w3l.card.txt >  CnC2015X_3b_hh_w3l.card.txt
#        combineCardsSmart CnC2015X_{0X,1X,2X,3X}_w3l.card.txt >  CnC2015X_3b_w3l.card.txt
#        )
#    done
#    echo "Done at $(date)";

fi

