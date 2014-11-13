#!/bin/bash

if [[ "$HOSTNAME" == "special" ]]; then
    T="/boh";
    J=6;
else
    T="/afs/cern.ch/work/g/gpetrucc/TREES_70X_240914";
    J=4;
fi

LUMI=20.0
OPTIONS=" -P $T -j $J -l $LUMI -f --s2v --tree treeProducerSusyMultilepton --od susy_cards --asimov "
OPTIONS=" $OPTIONS -F sf/t $T/0_lepMVA_v1/evVarFriend_{cname}.root "
if [[ "$1" == "" ]] || echo $1 | grep -q 2lss; then
    #OPTIONS="${OPTIONS} --FM sf/t $T/0_SFs_v3/sfFriend_{cname}.root --xp FR_data_.* "
    OPT_2L="${OPTIONS} "
    #OPT_2L="${OPT_2L} -R MVA MVA min(LepGood1_mvaNew,LepGood2_mvaNew)>0.93"
    MVA_2L="-F sf/t   $T/1_finalMVA_susy2lss_v0/evVarFriend_{cname}.root "
    ZeroB="-R nBjet nBjet nBJetMedium40==0"
    OneB="-R nBjet nBjet nBJetMedium40==1"
    TwoB="-R nBjet nBjet nBJetMedium40>=2"
    #TwoB="$TwoB -R MVA MVA min(LepGood1_mvaTTH,LepGood2_mvaTTH)>0.5"
    SYSTS="syst/susyDummy.txt"

    CnC_expr="1+4*(met_pt>120)+(htJet40j>400)+2*(nJet40>=4)"
    CnC_bins="[0.5,1.5,2.5,3.5,4.5,5.5,6.5,7.5,8.5]"

    MVA_expr="finalMVA_susy_2LSS"
    MVA_bins="10,0,1"
    #OPT_2L="$OPT_2L -p ttH,TT.,TT,T1.* --sp T1.*"
    for X in susy_2lss_{mumu,ee,em}; do 
        echo $X; #~gpetrucc/sh/bann $X

        # ---- Cut-and-count as SUS-13-013 ----
        python makeShapeCardsSusy.py mca-CSA14.txt bins/${X}.txt $CnC_expr $CnC_bins $SYSTS $OPT_2L -o ${X}_CnC_0B $ZeroB;
        python makeShapeCardsSusy.py mca-CSA14.txt bins/${X}.txt $CnC_expr $CnC_bins $SYSTS $OPT_2L -o ${X}_CnC_1B  $OneB;
        python makeShapeCardsSusy.py mca-CSA14.txt bins/${X}.txt $CnC_expr $CnC_bins $SYSTS $OPT_2L -o ${X}_CnC_2B  $TwoB;

        # ---- Cut-and-count as SUS-13-013 ----
        python makeShapeCardsSusy.py mca-CSA14.txt bins/${X}.txt $MVA_2L $MVA_expr $MVA_bins $SYSTS $OPT_2L -o ${X}_MVA_0B $ZeroB;
        python makeShapeCardsSusy.py mca-CSA14.txt bins/${X}.txt $MVA_2L $MVA_expr $MVA_bins $SYSTS $OPT_2L -o ${X}_MVA_1B  $OneB;
        python makeShapeCardsSusy.py mca-CSA14.txt bins/${X}.txt $MVA_2L $MVA_expr $MVA_bins $SYSTS $OPT_2L -o ${X}_MVA_2B  $TwoB;
        echo "Done at $(date)"
        break
    done
fi
