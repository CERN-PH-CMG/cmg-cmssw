#!/bin/bash

if [[ "$HOSTNAME" == "cmsphys05" ]]; then
    T="/data/b/botta/TTHAnalysis/trees/TREES_250513_HADD";
    J=6;
elif [[ "$HOSTNAME" == "olsnba03" ]]; then
    T="/data/gpetrucc/TREES_250513_HADD";
    J=6;
elif [[ "$HOSTNAME" == "lxbse14c09.cern.ch" ]]; then
    T="/var/ssdtest/gpetrucc/TREES_250513_HADD";
    J=10;
else
    T="/afs/cern.ch/work/g/gpetrucc/TREES_250513_HADD";
    J=4;
fi

OPTIONS=" -P $T -j $J -l 19.6 -f --xp data --od cards"
SYSTS="systsEnv.txt ../../macros/systematics/btagSysts.txt"
if [[ "$1" == "" || "$1" == "2lss" ]]; then
    OPTIONS="${OPTIONS} --FM sf/t $T/0_SFs_v2/sfFriend_{cname}.root "
    OPT_2L="${OPTIONS} -W puWeight*Eff_2lep*SF_btag*SF_LepMVATight_2l*SF_LepTightCharge_2l*SF_trig2l"
    #MVA_2L="-F sf/t  /afs/cern.ch/user/g/gpetrucc/w/TREES_250513_HADD/2_finalmva_2lss_v1/evVarFriend_{cname}.root "
    MVA_2L="-F sf/t   /afs/cern.ch/user/g/gpetrucc/w/TREES_250513_HADD/2_finalmva_2lss_v2/evVarFriend_{cname}.root "
    J4=" -A pt2010 4j nJet25>=4 "
    J23=" -A pt2010 4j nJet25<=3 "
    J3=" -A pt2010 4j nJet25==3 "
    #BLoose=" -R 2b 2bl (nBJetLoose25>=2)*(nBJetMedium25<2) "
    BLoose2=" -I 2B "
    BAny2=" -X 2B "
    #BMedium=" -R 2b 2bl ((nBJetLoose25>=2)*(nBJetMedium25==1)) "
    #BAnyM=" -R 2b 2bl ((nBJetLoose25>=2)*(nBJetMedium25>=1)) "
    BTight="  "
    LTight=" -A 2b 2lt (LepGood2_pt>20)*(LepGood1_pt+LepGood2_pt+met>100)  "
    POS=" -A pt2010 positive LepGood1_charge>0 "
    NEG=" -A pt2010 positive LepGood1_charge<0 "
    for X in 2lss_{mumu,ee,em}; do 
        #if [[ "$X" == "2lss_mumu" ]]; then continue; fi
        ~gpetrucc/sh/bann $X
        #python makeShapeCards.py mca-2lss-data.txt bins/$X.txt 'nJet25' '5,1.5,6.5' $SYSTS $OPT_2L -o ${X}_nJet;
        #python makeShapeCards.py mca-2lss-data.txt bins/$X.txt 'nJet25' '5,1.5,6.5' $SYSTS $OPT_2L -o ${X}_nJet_ba2 $BAny2; 
        #python makeShapeCards.py mca-2lss-data.txt bins/$X.txt 'nJet25' '5,1.5,6.5' $SYSTS $OPT_2L -o ${X}_nJet_bam $BAnyM; 
        #python makeShapeCards.py mca-2lss-data.txt bins/$X.txt 'nJet25' '5,1.5,6.5' $SYSTS $OPT_2L -o ${X}_nJet_pos $POS; 
        #python makeShapeCards.py mca-2lss-data.txt bins/$X.txt 'nJet25' '5,1.5,6.5' $SYSTS $OPT_2L -o ${X}_nJet_neg $NEG; 
        #python makeShapeCards.py mca-2lss-data.txt bins/$X.txt 'nJet25' '5,1.5,6.5' $SYSTS $OPT_2L -o ${X}_nJet_pos_bl $POS $BLoose; 
        #python makeShapeCards.py mca-2lss-data.txt bins/$X.txt 'nJet25' '5,1.5,6.5' $SYSTS $OPT_2L -o ${X}_nJet_neg_bl $NEG $BLoose; 
        #python makeShapeCards.py mca-2lss-data.txt bins/$X.txt 'nJet25' '5,1.5,6.5' $SYSTS $OPT_2L -o ${X}_nJet_pos_bl2 $POS $BLoose2; 
        #python makeShapeCards.py mca-2lss-data.txt bins/$X.txt 'nJet25' '5,1.5,6.5' $SYSTS $OPT_2L -o ${X}_nJet_neg_bl2 $NEG $BLoose2; 
        #python makeShapeCards.py mca-2lss-data.txt bins/$X.txt 'nJet25' '5,1.5,6.5' $SYSTS $OPT_2L -o ${X}_nJet_pos_bm $POS $BMedium; 
        #python makeShapeCards.py mca-2lss-data.txt bins/$X.txt 'nJet25' '5,1.5,6.5' $SYSTS $OPT_2L -o ${X}_nJet_neg_bm $NEG $BMedium; 
        #python makeShapeCards.py mca-2lss-data.txt bins/$X.txt 'nJet25' '5,1.5,6.5' $SYSTS $OPT_2L -o ${X}_nJet_pos_bt $POS $BTight; 
        #python makeShapeCards.py mca-2lss-data.txt bins/$X.txt 'nJet25' '5,1.5,6.5' $SYSTS $OPT_2L -o ${X}_nJet_neg_bt $NEG $BTight; 
        #python makeShapeCards.py mca-2lss-data.txt bins/$X.txt 'nJet25' '3,3.5,6.5' $SYSTS $OPT_2L $J4 -o ${X}_nJet4_ba2 $BAny2; 
        python makeShapeCards.py mca-2lss-data.txt bins/$X.txt 'nJet25' '3,3.5,6.5' $SYSTS $OPT_2L $J4 -o ${X}_nJet4_pos_bt $POS $BTight; 
        python makeShapeCards.py mca-2lss-data.txt bins/$X.txt 'nJet25' '3,3.5,6.5' $SYSTS $OPT_2L $J4 -o ${X}_nJet4_neg_bt $NEG $BTight; 
        python makeShapeCards.py mca-2lss-data.txt bins/$X.txt 'nJet25' '3,3.5,6.5' $SYSTS $OPT_2L $J4 -o ${X}_nJet4_pos_bl2 $POS $BLoose2; 
        python makeShapeCards.py mca-2lss-data.txt bins/$X.txt 'nJet25' '3,3.5,6.5' $SYSTS $OPT_2L $J4 -o ${X}_nJet4_neg_bl2 $NEG $BLoose2; 

        #python makeShapeCards.py mca-2lss-data.txt bins/$X.txt 'nJet25' '3,3.5,6.5' $SYSTS $OPT_2L $J4 -o ${X}_nJet4T_ba2 $BAny2 $LTight; 

        #python makeShapeCards.py mca-2lss-data.txt bins/$X.txt 'nJet25' '3,3.5,6.5' $SYSTS $OPT_2L $J4 -o ${X}_nJet4T_pos_bt  $POS $BTight $LTight; 
        #python makeShapeCards.py mca-2lss-data.txt bins/$X.txt 'nJet25' '3,3.5,6.5' $SYSTS $OPT_2L $J4 -o ${X}_nJet4T_neg_bt  $NEG $BTight $LTight; 
        #python makeShapeCards.py mca-2lss-data.txt bins/$X.txt 'nJet25' '3,3.5,6.5' $SYSTS $OPT_2L $J4 -o ${X}_nJet4T_pos_bl2 $POS $BLoose2 $LTight; 
        #python makeShapeCards.py mca-2lss-data.txt bins/$X.txt 'nJet25' '3,3.5,6.5' $SYSTS $OPT_2L $J4 -o ${X}_nJet4T_neg_bl2 $NEG $BLoose2 $LTight; 

        #python makeShapeCards.py mca-2lss-data.txt bins/$X.txt 'MVA_2LSS_4j_6var'  '6,-0.8,0.8' $SYSTS $OPT_2L -o ${X}_MVA_4j_6v  $J4  $MVA_2L;
        #python makeShapeCards.py mca-2lss-data.txt bins/$X.txt 'MVA_2LSS_4j_6var'  '6,-0.8,0.8' $SYSTS $OPT_2L -o ${X}_MVA_4j_6v_ba2  $J4  $MVA_2L $BAny2;
        #python makeShapeCards.py mca-2lss-data.txt bins/$X.txt 'MVA_2LSS_4j_6var'  '6,-0.8,0.8' $SYSTS $OPT_2L -o ${X}_MVA_4j_6v_bam  $J4  $MVA_2L $BAnyM;
        #python makeShapeCards.py mca-2lss-data.txt bins/$X.txt 'MVA_2LSS_4j_6var_cat'  '6,-0.8,0.8' $SYSTS $OPT_2L -o ${X}_MVA_4j_6v_cat      $J4  $MVA_2L;
        #python makeShapeCards.py mca-2lss-data.txt bins/$X.txt 'MVA_2LSS_4j_6var_cat'  '6,-0.8,0.8' $SYSTS $OPT_2L -o ${X}_MVA_4j_6v_cat_ba2  $J4  $MVA_2L $BAny2;
        #python makeShapeCards.py mca-2lss-data.txt bins/$X.txt 'MVA_2LSS_4j_6var_cat'  '6,-0.8,0.8' $SYSTS $OPT_2L -o ${X}_MVA_4j_6v_cat_bam  $J4  $MVA_2L $BAnyM;
        #python makeShapeCards.py mca-2lss-data.txt bins/$X.txt 'MVA_2LSS_4j_6var_cat'  '6,-0.8,0.8' $SYSTS $OPT_2L -o ${X}_MVA_4j_6v_cat  $J4  $MVA_2L;
        #python makeShapeCards.py mca-2lss-data.txt bins/$X.txt 'MVA_2LSS_23j_6var' '6,-0.8,0.8' $SYSTS $OPT_2L -o ${X}_MVA_23j_6v $J23 $MVA_2L;
        #python makeShapeCards.py mca-2lss-data.txt bins/$X.txt 'MVA_2LSS_23j_6var' '6,-0.8,0.8' $SYSTS $OPT_2L -o ${X}_MVA_3j_6v $J3 $MVA_2L;
        #python makeShapeCards.py mca-2lss-data.txt bins/$X.txt 'MVA_2LSS_23j_6var' '6,-0.8,0.8' $SYSTS $OPT_2L -o ${X}_MVA_3j_6v_ba2 $J3 $MVA_2L $BAny2;
        #python makeShapeCards.py mca-2lss-data.txt bins/$X.txt 'MVA_2LSS_23j_6var' '6,-0.8,0.8' $SYSTS $OPT_2L -o ${X}_MVA_3j_6v_bam $J3 $MVA_2L $BAnyM;
        #python makeShapeCards.py mca-2lss-data.txt bins/$X.txt 'MVA_2LSS_23j_6var' '6,-0.8,0.8' $SYSTS $OPT_2L -o ${X}_MVA_23j_6v_neg $J23 $MVA_2L $NEG;
        #python makeShapeCards.py mca-2lss-data.txt bins/$X.txt 'MVA_2LSS_23j_6var' '6,-0.8,0.8' $SYSTS $OPT_2L -o ${X}_MVA_23j_6v_pos $J23 $MVA_2L $POS;
        #python makeShapeCards.py mca-2lss-data.txt bins/$X.txt 'MVA_2LSS_23j_6var' '6,-0.8,0.8' $SYSTS $OPT_2L -o ${X}_MVA_3j_6v_neg $J3 $MVA_2L $NEG;
        #python makeShapeCards.py mca-2lss-data.txt bins/$X.txt 'MVA_2LSS_23j_6var' '6,-0.8,0.8' $SYSTS $OPT_2L -o ${X}_MVA_3j_6v_pos $J3 $MVA_2L $POS;
        #python makeShapeCards.py mca-2lss-data.txt bins/$X.txt 'MVA_2LSS_4j_6var'  '6,-0.8,0.8' $SYSTS $OPT_2L -o ${X}_MVA_4j_6v_neg  $J4  $MVA_2L $NEG;
        #python makeShapeCards.py mca-2lss-data.txt bins/$X.txt 'MVA_2LSS_4j_6var'  '6,-0.8,0.8' $SYSTS $OPT_2L -o ${X}_MVA_4j_6v_pos  $J4  $MVA_2L $POS;

        #python makeShapeCards.py mca-2lss-data.txt bins/$X.txt 'MVA_2LSS_23j_6var' '6,-0.8,0.8' $SYSTS $OPT_2L -o ${X}_MVA_23j_6v_neg_bl $J23 $MVA_2L $NEG $BLoose;
        #python makeShapeCards.py mca-2lss-data.txt bins/$X.txt 'MVA_2LSS_23j_6var' '6,-0.8,0.8' $SYSTS $OPT_2L -o ${X}_MVA_23j_6v_neg_bt $J23 $MVA_2L $NEG $BTight;
        #python makeShapeCards.py mca-2lss-data.txt bins/$X.txt 'MVA_2LSS_23j_6var' '6,-0.8,0.8' $SYSTS $OPT_2L -o ${X}_MVA_23j_6v_pos_bl $J23 $MVA_2L $POS $BLoose;
        #python makeShapeCards.py mca-2lss-data.txt bins/$X.txt 'MVA_2LSS_23j_6var' '6,-0.8,0.8' $SYSTS $OPT_2L -o ${X}_MVA_23j_6v_pos_bt $J23 $MVA_2L $POS $BTight;

        #python makeShapeCards.py mca-2lss-data.txt bins/$X.txt 'MVA_2LSS_23j_6var' '6,-0.8,0.8' $SYSTS $OPT_2L -o ${X}_MVA_3j_6v_pos_bl  $J3  $MVA_2L $POS $BLoose;
        #python makeShapeCards.py mca-2lss-data.txt bins/$X.txt 'MVA_2LSS_23j_6var' '6,-0.8,0.8' $SYSTS $OPT_2L -o ${X}_MVA_3j_6v_neg_bl  $J3  $MVA_2L $NEG $BLoose;
        #python makeShapeCards.py mca-2lss-data.txt bins/$X.txt 'MVA_2LSS_23j_6var' '6,-0.8,0.8' $SYSTS $OPT_2L -o ${X}_MVA_3j_6v_pos_bt  $J3  $MVA_2L $POS $BTight;
        #python makeShapeCards.py mca-2lss-data.txt bins/$X.txt 'MVA_2LSS_23j_6var' '6,-0.8,0.8' $SYSTS $OPT_2L -o ${X}_MVA_3j_6v_neg_bt  $J3  $MVA_2L $NEG $BTight;
        #python makeShapeCards.py mca-2lss-data.txt bins/$X.txt 'MVA_2LSS_23j_6var' '6,-0.8,0.8' $SYSTS $OPT_2L -o ${X}_MVA_3j_6v_pos_bl2 $J3  $MVA_2L $POS $BLoose2;
        #python makeShapeCards.py mca-2lss-data.txt bins/$X.txt 'MVA_2LSS_23j_6var' '6,-0.8,0.8' $SYSTS $OPT_2L -o ${X}_MVA_3j_6v_neg_bl2 $J3  $MVA_2L $NEG $BLoose2;
        #python makeShapeCards.py mca-2lss-data.txt bins/$X.txt 'MVA_2LSS_23j_6var' '6,-0.8,0.8' $SYSTS $OPT_2L -o ${X}_MVA_3j_6v_pos_bm $J3  $MVA_2L $POS $BMedium;
        #python makeShapeCards.py mca-2lss-data.txt bins/$X.txt 'MVA_2LSS_23j_6var' '6,-0.8,0.8' $SYSTS $OPT_2L -o ${X}_MVA_3j_6v_neg_bm $J3  $MVA_2L $NEG $BMedium;

        #python makeShapeCards.py mca-2lss-data.txt bins/$X.txt 'MVA_2LSS_23j_9var' '6,-0.8,0.8' $SYSTS $OPT_2L -o ${X}_MVA_23j_9v_neg_bl $J23 $MVA_2L $NEG $BLoose;
        #python makeShapeCards.py mca-2lss-data.txt bins/$X.txt 'MVA_2LSS_23j_9var' '6,-0.8,0.8' $SYSTS $OPT_2L -o ${X}_MVA_23j_9v_neg_bt $J23 $MVA_2L $NEG $BTight;
        #python makeShapeCards.py mca-2lss-data.txt bins/$X.txt 'MVA_2LSS_23j_9var' '6,-0.8,0.8' $SYSTS $OPT_2L -o ${X}_MVA_23j_9v_pos_bl $J23 $MVA_2L $POS $BLoose;
        #python makeShapeCards.py mca-2lss-data.txt bins/$X.txt 'MVA_2LSS_23j_9var' '6,-0.8,0.8' $SYSTS $OPT_2L -o ${X}_MVA_23j_9v_pos_bt $J23 $MVA_2L $POS $BTight;
        #python makeShapeCards.py mca-2lss-data.txt bins/$X.txt 'MVA_2LSS_23j_9var' '6,-0.8,0.8' $SYSTS $OPT_2L -o ${X}_MVA_3j_9v_pos_bl  $J3  $MVA_2L $POS $BLoose;
        #python makeShapeCards.py mca-2lss-data.txt bins/$X.txt 'MVA_2LSS_23j_9var' '6,-0.8,0.8' $SYSTS $OPT_2L -o ${X}_MVA_3j_9v_neg_bl  $J3  $MVA_2L $NEG $BLoose;
        #python makeShapeCards.py mca-2lss-data.txt bins/$X.txt 'MVA_2LSS_23j_9var' '6,-0.8,0.8' $SYSTS $OPT_2L -o ${X}_MVA_3j_9v_pos_bt  $J3  $MVA_2L $POS $BTight;
        #python makeShapeCards.py mca-2lss-data.txt bins/$X.txt 'MVA_2LSS_23j_9var' '6,-0.8,0.8' $SYSTS $OPT_2L -o ${X}_MVA_3j_9v_neg_bt  $J3  $MVA_2L $NEG $BTight;

        #python makeShapeCards.py mca-2lss-data.txt bins/$X.txt 'MVA_2LSS_4j_6var'  '6,-0.8,0.8' $SYSTS $OPT_2L -o ${X}_MVA_4j_6v_neg_bl  $J4  $MVA_2L $NEG $BLoose;
        #python makeShapeCards.py mca-2lss-data.txt bins/$X.txt 'MVA_2LSS_4j_6var'  '6,-0.8,0.8' $SYSTS $OPT_2L -o ${X}_MVA_4j_6v_pos_bl  $J4  $MVA_2L $POS $BLoose;
        #python makeShapeCards.py mca-2lss-data.txt bins/$X.txt 'MVA_2LSS_4j_6var'  '6,-0.8,0.8' $SYSTS $OPT_2L -o ${X}_MVA_4j_6v_neg_bt  $J4  $MVA_2L $NEG $BTight;
        #python makeShapeCards.py mca-2lss-data.txt bins/$X.txt 'MVA_2LSS_4j_6var'  '4,-0.8,0.8' $SYSTS $OPT_2L -o ${X}_MVA_4j_6v_pos_bt  $J4  $MVA_2L $POS $BTight;
        #python makeShapeCards.py mca-2lss-data.txt bins/$X.txt 'MVA_2LSS_4j_6var'  '4,-0.8,0.8' $SYSTS $OPT_2L -o ${X}_MVA_4j_6v_neg_bl2 $J4  $MVA_2L $NEG $BLoose2;
        #python makeShapeCards.py mca-2lss-data.txt bins/$X.txt 'MVA_2LSS_4j_6var'  '6,-0.8,0.8' $SYSTS $OPT_2L -o ${X}_MVA_4j_6v_pos_bl2 $J4  $MVA_2L $POS $BLoose2;
        #python makeShapeCards.py mca-2lss-data.txt bins/$X.txt 'MVA_2LSS_4j_6var'  '4,-0.8,0.8' $SYSTS $OPT_2L -o ${X}_MVA_4j_6v_neg_bm  $J4  $MVA_2L $NEG $BMedium;
        #python makeShapeCards.py mca-2lss-data.txt bins/$X.txt 'MVA_2LSS_4j_6var'  '4,-0.8,0.8' $SYSTS $OPT_2L -o ${X}_MVA_4j_6v_pos_bm  $J4  $MVA_2L $POS $BMedium;

        #python makeShapeCards.py mca-2lss-data.txt bins/$X.txt 'MVA_2LSS_4j_6var'  '6,-0.8,0.8' $SYSTS $OPT_2L -o ${X}_MVA_4jT_6v_neg_bt  $J4 $LTight $MVA_2L $NEG $BTight;
        #python makeShapeCards.py mca-2lss-data.txt bins/$X.txt 'MVA_2LSS_4j_6var'  '4,-0.8,0.8' $SYSTS $OPT_2L -o ${X}_MVA_4jT_6v_pos_bt  $J4 $LTight $MVA_2L $POS $BTight;
        #python makeShapeCards.py mca-2lss-data.txt bins/$X.txt 'MVA_2LSS_4j_6var'  '4,-0.8,0.8' $SYSTS $OPT_2L -o ${X}_MVA_4jT_6v_neg_bl2 $J4 $LTight $MVA_2L $NEG $BLoose2;
        #python makeShapeCards.py mca-2lss-data.txt bins/$X.txt 'MVA_2LSS_4j_6var'  '6,-0.8,0.8' $SYSTS $OPT_2L -o ${X}_MVA_4jT_6v_pos_bl2 $J4 $LTight $MVA_2L $POS $BLoose2;

        #python makeShapeCards.py mca-2lss-data.txt bins/$X.txt 'MVA_2LSS_4j_6var_cat'  '6,-0.8,0.8' $SYSTS $OPT_2L -o ${X}_MVA_4j_6v_cat_neg_bl  $J4  $MVA_2L $NEG $BLoose;
        #python makeShapeCards.py mca-2lss-data.txt bins/$X.txt 'MVA_2LSS_4j_6var_cat'  '6,-0.8,0.8' $SYSTS $OPT_2L -o ${X}_MVA_4j_6v_cat_pos_bl  $J4  $MVA_2L $POS $BLoose;
        #python makeShapeCards.py mca-2lss-data.txt bins/$X.txt 'MVA_2LSS_4j_6var_cat'  '6,-0.8,0.8' $SYSTS $OPT_2L -o ${X}_MVA_4j_6v_cat_neg_bt  $J4  $MVA_2L $NEG $BTight;
        #python makeShapeCards.py mca-2lss-data.txt bins/$X.txt 'MVA_2LSS_4j_6var_cat'  '4,-0.8,0.8' $SYSTS $OPT_2L -o ${X}_MVA_4j_6v_cat_pos_bt  $J4  $MVA_2L $POS $BTight;
        #python makeShapeCards.py mca-2lss-data.txt bins/$X.txt 'MVA_2LSS_4j_6var_cat'  '4,-0.8,0.8' $SYSTS $OPT_2L -o ${X}_MVA_4j_6v_cat_neg_bl2 $J4  $MVA_2L $NEG $BLoose2;
        #python makeShapeCards.py mca-2lss-data.txt bins/$X.txt 'MVA_2LSS_4j_6var_cat'  '6,-0.8,0.8' $SYSTS $OPT_2L -o ${X}_MVA_4j_6v_cat_pos_bl2 $J4  $MVA_2L $POS $BLoose2;
        #python makeShapeCards.py mca-2lss-data.txt bins/$X.txt 'MVA_2LSS_4j_6var_cat'  '4,-0.8,0.8' $SYSTS $OPT_2L -o ${X}_MVA_4j_6v_cat_neg_bm  $J4  $MVA_2L $NEG $BMedium;
        #python makeShapeCards.py mca-2lss-data.txt bins/$X.txt 'MVA_2LSS_4j_6var_cat'  '4,-0.8,0.8' $SYSTS $OPT_2L -o ${X}_MVA_4j_6v_cat_pos_bm  $J4  $MVA_2L $POS $BMedium;

        #python makeShapeCards.py mca-2lss-data.txt bins/$X.txt 'MVA_2LSS_4j_10var'  '6,-0.8,0.8' $SYSTS $OPT_2L -o ${X}_MVA_4j_10v_neg_bl  $J4  $MVA_2L $NEG $BLoose;
        #python makeShapeCards.py mca-2lss-data.txt bins/$X.txt 'MVA_2LSS_4j_10var'  '6,-0.8,0.8' $SYSTS $OPT_2L -o ${X}_MVA_4j_10v_neg_bt  $J4  $MVA_2L $NEG $BTight;
        #python makeShapeCards.py mca-2lss-data.txt bins/$X.txt 'MVA_2LSS_4j_10var'  '6,-0.8,0.8' $SYSTS $OPT_2L -o ${X}_MVA_4j_10v_pos_bl  $J4  $MVA_2L $POS $BLoose;
        #python makeShapeCards.py mca-2lss-data.txt bins/$X.txt 'MVA_2LSS_4j_10var'  '6,-0.8,0.8' $SYSTS $OPT_2L -o ${X}_MVA_4j_10v_pos_bt  $J4  $MVA_2L $POS $BTight;
        #python makeShapeCards.py mca-2lss-data.txt bins/$X.txt 'MVA_2LSS_4j_10var'  '6,-0.8,0.8' $SYSTS $OPT_2L -o ${X}_MVA_4j_10v_pos_bl2 $J4  $MVA_2L $POS $BLoose2;
        #python makeShapeCards.py mca-2lss-data.txt bins/$X.txt 'MVA_2LSS_4j_10var'  '6,-0.8,0.8' $SYSTS $OPT_2L -o ${X}_MVA_4j_10v_neg_bl2 $J4  $MVA_2L $NEG $BLoose2;

        #python makeShapeCards.py mca-2lss-data.txt bins/$X.txt 'MVA_2LSS_4j_15var'  '6,-0.8,0.8' $SYSTS $OPT_2L -o ${X}_MVA_4j_15v_neg_bl  $J4  $MVA_2L $NEG $BLoose;
        #python makeShapeCards.py mca-2lss-data.txt bins/$X.txt 'MVA_2LSS_4j_15var'  '6,-0.8,0.8' $SYSTS $OPT_2L -o ${X}_MVA_4j_15v_neg_bt  $J4  $MVA_2L $NEG $BTight;
        #python makeShapeCards.py mca-2lss-data.txt bins/$X.txt 'MVA_2LSS_4j_15var'  '6,-0.8,0.8' $SYSTS $OPT_2L -o ${X}_MVA_4j_15v_pos_bl  $J4  $MVA_2L $POS $BLoose;
        #python makeShapeCards.py mca-2lss-data.txt bins/$X.txt 'MVA_2LSS_4j_15var'  '6,-0.8,0.8' $SYSTS $OPT_2L -o ${X}_MVA_4j_15v_pos_bt  $J4  $MVA_2L $POS $BTight;
        #python makeShapeCards.py mca-2lss-data.txt bins/$X.txt 'MVA_2LSS_4j_15var'  '6,-0.8,0.8' $SYSTS $OPT_2L -o ${X}_MVA_4j_15v_pos_bl2 $J4  $MVA_2L $POS $BLoose2;
        #python makeShapeCards.py mca-2lss-data.txt bins/$X.txt 'MVA_2LSS_4j_15var'  '6,-0.8,0.8' $SYSTS $OPT_2L -o ${X}_MVA_4j_15v_neg_bl2 $J4  $MVA_2L $NEG $BLoose2;

        echo "Done at $(date)"
    done
fi

# if [[ "$1" == "" || "$1" == "3l" ]]; then
#     OPTIONS="${OPTIONS} --FM sf/t $T/0_SFs_v2/sfFriend_{cname}.root "
#     OPT_3L="${OPTIONS} -W  puWeight*Eff_3lep*SF_btag*SF_LepMVALoose_3l "
#     MVA_3L="-F finalMVA/t $T/0_finalmva_3l/finalMVA_3L_{cname}.root"
#     POS=" -A pt2010 positive LepGood1_charge+LepGood2_charge+LepGood3_charge>0 "
#     NEG=" -A pt2010 positive LepGood1_charge+LepGood2_charge+LepGood3_charge<0 "
#     python makeShapeCards.py mca-3l-data.txt bins/3l_auto.txt 'nJet25' '4,1.5,5.5' $SYSTS $OPT_3L -o 3l_loose_nJet; 
#     python makeShapeCards.py mca-3l-data.txt bins/3l_auto.txt 'nJet25' '4,1.5,5.5' $SYSTS $OPT_3L -o 3l_loose_nJet_pos $POS; 
#     python makeShapeCards.py mca-3l-data.txt bins/3l_auto.txt 'nJet25' '4,1.5,5.5' $SYSTS $OPT_3L -o 3l_loose_nJet_neg $NEG; 
#     python makeShapeCards.py mca-3l-data.txt bins/3l_auto.txt 'FinalMVA_3L_BDTG' '6,-1.0,0.8' $SYSTS $OPT_3L -o 3l_loose_MVA $MVA_3L;
#     echo "Done at $(date)"
# fi
# 
if [[ "$1" == "" || "$1" == "3l_tight" ]]; then
    OPTIONS="${OPTIONS} --FM sf/t $T/0_SFs_v2/sfFriend_{cname}.root  "
    OPT_3L="${OPTIONS} -W  puWeight*Eff_3lep*SF_btag*SF_LepMVATight_3l*SF_LepTightCharge_3l"
    MVA_3L="-F finalMVA/t $T/0_finalmva_3l/finalMVA_3L_{cname}.root"
    POS=" -A pt2010 positive LepGood1_charge+LepGood2_charge+LepGood3_charge>0 "
    NEG=" -A pt2010 positive LepGood1_charge+LepGood2_charge+LepGood3_charge<0 "
    BAny2=" -R 2b 2bl ((nBJetLoose25>=2)+(nJet25>=2)*(nBJetMedium25==1)) "
    BLoose=" -R 2b 2bl (nBJetLoose25>=2)*(nBJetMedium25<2) "
    BLoose2=" -R 2b 2bl ((nBJetLoose25>=2)+(nJet25>=2)*(nBJetMedium25==1))*(nBJetMedium25<2) "
    BTight=" -R 2b 2bt nBJetMedium25>=2 "
    #python makeShapeCards.py mca-3l_tight-data.txt bins/3l_tight.txt 'nJet25' '4,1.5,5.5' $SYSTS $OPT_3L -o 3l_nJet_ba2 $BAny2; 
    #python makeShapeCards.py mca-3l_tight-data.txt bins/3l_tight.txt 'nJet25' '4,1.5,5.5' $SYSTS $OPT_3L -o 3l_nJet; 
    #python makeShapeCards.py mca-3l_tight-data.txt bins/3l_tight.txt 'nJet25' '4,1.5,5.5' $SYSTS $OPT_3L -o 3l_nJet_pos $POS; 
    #python makeShapeCards.py mca-3l_tight-data.txt bins/3l_tight.txt 'nJet25' '4,1.5,5.5' $SYSTS $OPT_3L -o 3l_nJet_neg $NEG; 
    #python makeShapeCards.py mca-3l_tight-data.txt bins/3l_tight.txt 'nJet25' '4,1.5,5.5' $SYSTS $OPT_3L -o 3l_nJet_pos_bl $POS $BLoose; 
    #python makeShapeCards.py mca-3l_tight-data.txt bins/3l_tight.txt 'nJet25' '4,1.5,5.5' $SYSTS $OPT_3L -o 3l_nJet_neg_bl $NEG $BLoose; 
    python makeShapeCards.py mca-3l_tight-data.txt bins/3l_tight.txt 'nJet25' '4,1.5,5.5' $SYSTS $OPT_3L -o 3l_nJet_pos_bt $POS $BTight; 
    python makeShapeCards.py mca-3l_tight-data.txt bins/3l_tight.txt 'nJet25' '4,1.5,5.5' $SYSTS $OPT_3L -o 3l_nJet_neg_bt $NEG $BTight; 
    python makeShapeCards.py mca-3l_tight-data.txt bins/3l_tight.txt 'nJet25' '4,1.5,5.5' $SYSTS $OPT_3L -o 3l_nJet_pos_bl2 $POS $BLoose2; 
    python makeShapeCards.py mca-3l_tight-data.txt bins/3l_tight.txt 'nJet25' '4,1.5,5.5' $SYSTS $OPT_3L -o 3l_nJet_neg_bl2 $NEG $BLoose2; 
    #python makeShapeCards.py mca-3l_tight-data.txt bins/3l_tight.txt 'FinalMVA_3L_BDTG' '9,-1.0,0.8' $SYSTS $OPT_3L -o 3l_MVA $MVA_3L;
    #python makeShapeCards.py mca-3l_tight-data.txt bins/3l_tight.txt 'FinalMVA_3L_BDTG' '9,-1.0,0.8' $SYSTS $OPT_3L -o 3l_MVA_pos $MVA_3L $POS;
    #python makeShapeCards.py mca-3l_tight-data.txt bins/3l_tight.txt 'FinalMVA_3L_BDTG' '9,-1.0,0.8' $SYSTS $OPT_3L -o 3l_MVA_neg $MVA_3L $NEG;
    #python makeShapeCards.py mca-3l_tight-data.txt bins/3l_tight.txt 'FinalMVA_3L_BDTG' '9,-1.0,0.8' $SYSTS $OPT_3L -o 3l_MVA_bt  $MVA_3L $BTight;
    #python makeShapeCards.py mca-3l_tight-data.txt bins/3l_tight.txt 'FinalMVA_3L_BDTG' '9,-1.0,0.8' $SYSTS $OPT_3L -o 3l_MVA_bl  $MVA_3L $BLoose;
    #python makeShapeCards.py mca-3l_tight-data.txt bins/3l_tight.txt 'FinalMVA_3L_BDTG' '9,-1.0,0.8' $SYSTS $OPT_3L -o 3l_MVA_bl2 $MVA_3L $BLoose2;
    python makeShapeCards.py mca-3l_tight-data.txt bins/3l_tight.txt 'FinalMVA_3L_BDTG' '4,-1.0,0.6' $SYSTS $OPT_3L -o 3l_MVA_pos_bt  $MVA_3L $POS $BTight;
    python makeShapeCards.py mca-3l_tight-data.txt bins/3l_tight.txt 'FinalMVA_3L_BDTG' '4,-1.0,0.6' $SYSTS $OPT_3L -o 3l_MVA_neg_bt  $MVA_3L $NEG $BTight;
    python makeShapeCards.py mca-3l_tight-data.txt bins/3l_tight.txt 'FinalMVA_3L_BDTG' '6,-1.0,0.6' $SYSTS $OPT_3L -o 3l_MVA_neg_bl2 $MVA_3L $NEG $BLoose2;
    python makeShapeCards.py mca-3l_tight-data.txt bins/3l_tight.txt 'FinalMVA_3L_BDTG' '6,-1.0,0.6' $SYSTS $OPT_3L -o 3l_MVA_pos_bl2 $MVA_3L $POS $BLoose2;
    #python makeShapeCards.py mca-3l_tight-data.txt bins/3l_tight.txt 'FinalMVA_3L_BDTG' '9,-1.0,0.6' $SYSTS $OPT_3L -o 3l_MVA_pos_bl  $MVA_3L $POS $BLoose;
    #python makeShapeCards.py mca-3l_tight-data.txt bins/3l_tight.txt 'FinalMVA_3L_BDTG' '9,-1.0,0.6' $SYSTS $OPT_3L -o 3l_MVA_neg_bl  $MVA_3L $NEG $BLoose;
    echo "Done at $(date)"
fi



if [[ "$1" == "" || "$1" == "4l" ]]; then
    OPTIONS="${OPTIONS} --FM sf/t $T/0_SFs_v2/sfFriend_{cname}.root  "
    OPT_4L="${OPTIONS} -W puWeight*Eff_4lep*SF_btag*SF_LepMVALoose_4l"
    MVA_4L="-F finalMVA/t $T/0_finalmva_4l/finalMVA_4L_{cname}.root"
    BAny2=" -R 2b 2bl ((nBJetLoose25>=2)+(nJet25>=2)*(nBJetMedium25==1)) "
    BLoose=" -R 2b 2bl (nBJetLoose25>=2)*(nBJetMedium25<2) "
    BLoose2=" -R 2b 2bl ((nBJetLoose25>=2)+(nJet25>=2)*(nBJetMedium25==1))*(nBJetMedium25<2) "
    BTight=" -R 2b 2bt nBJetMedium25>=2 "
    python makeShapeCards.py mca-4l-data.txt bins/4l.txt 'nJet25' '3,0.5,3.5' $SYSTS $OPT_4L -o 4l_nJet_ba2 $BAny2; 
    python makeShapeCards.py mca-4l-data.txt bins/4l.txt 'nJet25' '3,0.5,3.5' $SYSTS $OPT_4L -o 4l_nJet_bl2 $BLoose2; 
    #python makeShapeCards.py mca-4l-data.txt bins/4l.txt 'nJet25' '3,0.5,3.5' $SYSTS $OPT_4L -o 4l_nJet_bl  $BLoose; 
    python makeShapeCards.py mca-4l-data.txt bins/4l.txt 'nJet25' '3,0.5,3.5' $SYSTS $OPT_4L -o 4l_nJet_bt  $BTight; 
   # python makeShapeCards.py mca-4l-data.txt bins/4l.txt 'FinalMVA_4L_BDTG' '5,-1.0,1.0' $SYSTS $OPT_4L $MVA_4L -o 4l_MVA; 
   # python makeShapeCards.py mca-4l-data.txt bins/4l.txt 'FinalMVA_4L_BDTG' '5,-1.0,1.0' $SYSTS $OPT_4L $MVA_4L -o 4l_MVA_bl2 $BLoose2; 
   # python makeShapeCards.py mca-4l-data.txt bins/4l.txt 'FinalMVA_4L_BDTG' '5,-1.0,1.0' $SYSTS $OPT_4L $MVA_4L -o 4l_MVA_bl  $BLoose; 
   # python makeShapeCards.py mca-4l-data.txt bins/4l.txt 'FinalMVA_4L_BDTG' '5,-1.0,1.0' $SYSTS $OPT_4L $MVA_4L -o 4l_MVA_bt  $BTight; 
    echo "Done at $(date)"
fi
