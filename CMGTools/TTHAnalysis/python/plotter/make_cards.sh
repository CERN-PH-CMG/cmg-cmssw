#!/bin/bash

if [[ "$HOSTNAME" == "cmsphys05" ]]; then
    T="/data/b/botta/TTHAnalysis/trees/TREES_250513_HADD";
    J=6;
elif [[ "$HOSTNAME" == "olsnba03" ]]; then
    T="/data/gpetrucc/TREES_250513_HADD";
    J=16;
elif [[ "$HOSTNAME" == "lxbse14c09.cern.ch" ]]; then
    T="/var/ssdtest/gpetrucc/TREES_250513_HADD";
    J=10;
else
    T="/afs/cern.ch/work/g/gpetrucc/TREES_250513_HADD";
    J=4;
fi

OPTIONS=" -P $T -j $J -l 19.6 -f  --od cards"
SYSTS="systsEnv.txt ../../macros/systematics/btagSysts.txt"
BLoose=" -I 2B "
BAny=" -X 2B "
BTight="  "

if [[ "$1" == "" || "$1" == "2lss" ]]; then
    OPTIONS="${OPTIONS} --FM sf/t $T/0_SFs_v2/sfFriend_{cname}.root --xp FR_data_.* "
    OPT_2L="${OPTIONS} -W puWeight*Eff_2lep*SF_btag*SF_LepMVATight_2l*SF_LepTightCharge_2l*SF_trig2l"
    MVA_2L="-F sf/t   /afs/cern.ch/user/g/gpetrucc/w/TREES_250513_HADD/2_finalmva_2lss_v2/evVarFriend_{cname}.root "
    POS=" -A pt2010 positive LepGood1_charge>0 "
    NEG=" -A pt2010 positive LepGood1_charge<0 "
    for X in 2lss_{mumu,ee,em}; do 
        #if [[ "$X" == "2lss_mumu" ]]; then continue; fi
        ~gpetrucc/sh/bann $X
        python makeShapeCards.py mca-2lss-data.txt bins/${X}_tight.txt 'nJet25' '3,3.5,6.5' $SYSTS $OPT_2L -o ${X}_nJet $BAny; 

        python makeShapeCards.py mca-2lss-data.txt bins/${X}_tight.txt 'nJet25' '3,3.5,6.5' $SYSTS $OPT_2L -o ${X}_nJet_pos $POS $BAny; 
        python makeShapeCards.py mca-2lss-data.txt bins/${X}_tight.txt 'nJet25' '3,3.5,6.5' $SYSTS $OPT_2L -o ${X}_nJet_neg $NEG $BAny; 

        #python makeShapeCards.py mca-2lss-data.txt bins/${X}_tight.txt 'nJet25' '3,3.5,6.5' $SYSTS $OPT_2L -o ${X}_nJet_pos_bt $POS $BTight; 
        #python makeShapeCards.py mca-2lss-data.txt bins/${X}_tight.txt 'nJet25' '3,3.5,6.5' $SYSTS $OPT_2L -o ${X}_nJet_neg_bt $NEG $BTight; 
        #python makeShapeCards.py mca-2lss-data.txt bins/${X}_tight.txt 'nJet25' '3,3.5,6.5' $SYSTS $OPT_2L -o ${X}_nJet_pos_bl $POS $BLoose; 
        #python makeShapeCards.py mca-2lss-data.txt bins/${X}_tight.txt 'nJet25' '3,3.5,6.5' $SYSTS $OPT_2L -o ${X}_nJet_neg_bl $NEG $BLoose; 
        #python makeShapeCards.py mca-2lss-dataFRMC.txt bins/${X}_tight.txt 'nJet25' '3,3.5,6.5' $SYSTS $OPT_2L -o ${X}_nJet_pos_btFRMC $POS $BTight; 
        #python makeShapeCards.py mca-2lss-dataFRMC.txt bins/${X}_tight.txt 'nJet25' '3,3.5,6.5' $SYSTS $OPT_2L -o ${X}_nJet_neg_btFRMC $NEG $BTight; 

        python makeShapeCards.py mca-2lss-data.txt bins/${X}_tight.txt 'MVA_2LSS_4j_6var'  '6,-0.8,0.8' $SYSTS $OPT_2L -o ${X}_MVA $MVA_2L $BAny;

        python makeShapeCards.py mca-2lss-data.txt bins/${X}_tight.txt 'MVA_2LSS_4j_6var'  '4,-0.8,0.8' $SYSTS $OPT_2L -o ${X}_MVA_neg $MVA_2L $NEG $BAny;
        python makeShapeCards.py mca-2lss-data.txt bins/${X}_tight.txt 'MVA_2LSS_4j_6var'  '6,-0.8,0.8' $SYSTS $OPT_2L -o ${X}_MVA_pos $MVA_2L $POS $BAny;

        #python makeShapeCards.py mca-2lss-data.txt bins/${X}_tight.txt 'MVA_2LSS_4j_6var'  '6,-0.8,0.8' $SYSTS $OPT_2L -o ${X}_MVA_4j_6v_neg_bt $MVA_2L $NEG $BTight;
        #python makeShapeCards.py mca-2lss-data.txt bins/${X}_tight.txt 'MVA_2LSS_4j_6var'  '4,-0.8,0.8' $SYSTS $OPT_2L -o ${X}_MVA_4j_6v_pos_bt $MVA_2L $POS $BTight;
        #python makeShapeCards.py mca-2lss-data.txt bins/${X}_tight.txt 'MVA_2LSS_4j_6var'  '4,-0.8,0.8' $SYSTS $OPT_2L -o ${X}_MVA_4j_6v_neg_bl $MVA_2L $NEG $BLoose;
        #python makeShapeCards.py mca-2lss-data.txt bins/${X}_tight.txt 'MVA_2LSS_4j_6var'  '6,-0.8,0.8' $SYSTS $OPT_2L -o ${X}_MVA_4j_6v_pos_bl $MVA_2L $POS $BLoose;
        #python makeShapeCards.py mca-2lss-dataFRMC.txt bins/${X}_tight.txt 'MVA_2LSS_4j_6var'  '6,-0.8,0.8' $SYSTS $OPT_2L -o ${X}_MVA_4j_6v_neg_btFRMC $MVA_2L $NEG $BTight;
        #python makeShapeCards.py mca-2lss-dataFRMC.txt bins/${X}_tight.txt 'MVA_2LSS_4j_6var'  '4,-0.8,0.8' $SYSTS $OPT_2L -o ${X}_MVA_4j_6v_pos_btFRMC $MVA_2L $POS $BTight;
        echo "Done at $(date)"
    done
fi

if [[ "$1" == "" || "$1" == "3l_tight" ]]; then
    OPTIONS="${OPTIONS} --FM sf/t $T/0_SFs_v2/sfFriend_{cname}.root --xp FR_data_.*   "
    OPT_3L="${OPTIONS} -W  puWeight*Eff_3lep*SF_btag*SF_LepMVATight_3l*SF_LepTightCharge_3l"
    MVA_3L="-F finalMVA/t $T/0_finalmva_3l/finalMVA_3L_{cname}.root"
    POS=" -A pt2010 positive LepGood1_charge+LepGood2_charge+LepGood3_charge>0 "
    NEG=" -A pt2010 positive LepGood1_charge+LepGood2_charge+LepGood3_charge<0 "
    #python makeShapeCards.py mca-3l_tight-data.txt bins/3l_tight.txt 'nJet25' '4,1.5,5.5' $SYSTS $OPT_3L -o 3l_nJet $BAny; 

    #python makeShapeCards.py mca-3l_tight-data.txt bins/3l_tight.txt 'nJet25' '4,1.5,5.5' $SYSTS $OPT_3L -o 3l_nJet_pos $POS $BAny; 
    #python makeShapeCards.py mca-3l_tight-data.txt bins/3l_tight.txt 'nJet25' '4,1.5,5.5' $SYSTS $OPT_3L -o 3l_nJet_neg $NEG $BAny; 

    #python makeShapeCards.py mca-3l_tight-data.txt bins/3l_tight.txt 'nJet25' '4,1.5,5.5' $SYSTS $OPT_3L -o 3l_nJet_pos_bt $POS $BTight; 
    #python makeShapeCards.py mca-3l_tight-data.txt bins/3l_tight.txt 'nJet25' '4,1.5,5.5' $SYSTS $OPT_3L -o 3l_nJet_neg_bt $NEG $BTight; 
    #python makeShapeCards.py mca-3l_tight-data.txt bins/3l_tight.txt 'nJet25' '4,1.5,5.5' $SYSTS $OPT_3L -o 3l_nJet_pos_bl $POS $BLoose; 
    #python makeShapeCards.py mca-3l_tight-data.txt bins/3l_tight.txt 'nJet25' '4,1.5,5.5' $SYSTS $OPT_3L -o 3l_nJet_neg_bl $NEG $BLoose; 
    #python makeShapeCards.py mca-3l_tight-dataFRMC.txt bins/3l_tight.txt 'nJet25' '4,1.5,5.5' $SYSTS $OPT_3L -o 3l_nJet_pos_btFRMC $POS $BTight; 
    #python makeShapeCards.py mca-3l_tight-dataFRMC.txt bins/3l_tight.txt 'nJet25' '4,1.5,5.5' $SYSTS $OPT_3L -o 3l_nJet_neg_btFRMC $NEG $BTight; 

    #python makeShapeCards.py mca-3l_tight-data.txt bins/3l_tight.txt 'FinalMVA_3L_BDTG' '6,-1.0,0.6' $SYSTS $OPT_3L -o 3l_MVA $MVA_3L $BAny;

    #python makeShapeCards.py mca-3l_tight-data.txt bins/3l_tight.txt 'FinalMVA_3L_BDTG' '6,-1.0,0.6' $SYSTS $OPT_3L -o 3l_MVA_neg $MVA_3L $NEG $BAny;
    #python makeShapeCards.py mca-3l_tight-data.txt bins/3l_tight.txt 'FinalMVA_3L_BDTG' '6,-1.0,0.6' $SYSTS $OPT_3L -o 3l_MVA_pos $MVA_3L $POS $BAny;

    python makeShapeCards.py mca-3l_tight-data.txt bins/3l_tight.txt 'FinalMVA_3L_BDTG' '6,-1.0,0.6' $SYSTS $OPT_3L -o 3l_MVA_Zpeak_neg $MVA_3L $NEG $BAny -I 'Z veto';
    python makeShapeCards.py mca-3l_tight-data.txt bins/3l_tight.txt 'FinalMVA_3L_BDTG' '6,-1.0,0.6' $SYSTS $OPT_3L -o 3l_MVA_Zpeak_pos $MVA_3L $POS $BAny -I 'Z veto';
    #python makeShapeCards.py mca-3l_tight-data.txt bins/3l_tight.txt 'FinalMVA_3L_BDTG' '4,-1.0,0.6' $SYSTS $OPT_3L -o 3l_MVA_pos_bt $MVA_3L $POS $BTight;
    #python makeShapeCards.py mca-3l_tight-data.txt bins/3l_tight.txt 'FinalMVA_3L_BDTG' '4,-1.0,0.6' $SYSTS $OPT_3L -o 3l_MVA_neg_bt $MVA_3L $NEG $BTight;
    #python makeShapeCards.py mca-3l_tight-data.txt bins/3l_tight.txt 'FinalMVA_3L_BDTG' '6,-1.0,0.6' $SYSTS $OPT_3L -o 3l_MVA_neg_bl $MVA_3L $NEG $BLoose;
    #python makeShapeCards.py mca-3l_tight-data.txt bins/3l_tight.txt 'FinalMVA_3L_BDTG' '6,-1.0,0.6' $SYSTS $OPT_3L -o 3l_MVA_pos_bl $MVA_3L $POS $BLoose;
    #python makeShapeCards.py mca-3l_tight-dataFRMC.txt bins/3l_tight.txt 'FinalMVA_3L_BDTG' '4,-1.0,0.6' $SYSTS $OPT_3L -o 3l_MVA_pos_btFRMC $MVA_3L $POS $BTight;
    #python makeShapeCards.py mca-3l_tight-dataFRMC.txt bins/3l_tight.txt 'FinalMVA_3L_BDTG' '4,-1.0,0.6' $SYSTS $OPT_3L -o 3l_MVA_neg_btFRMC $MVA_3L $NEG $BTight;
    echo "Done at $(date)"
fi



if [[ "$1" == "" || "$1" == "4l" ]]; then
    OPTIONS="${OPTIONS} --FM sf/t $T/0_SFs_v2/sfFriend_{cname}.root  "
    OPT_4L="${OPTIONS} -W puWeight*Eff_4lep*SF_btag*SF_LepMVALoose_4l"
    MVA_4L="-F finalMVA/t $T/0_finalmva_4l/finalMVA_4L_{cname}.root"
    python makeShapeCards.py mca-4l-ttscale.txt bins/4l.txt 'nJet25' '3,0.5,3.5' $SYSTS $OPT_4L -o 4l_nJet    $BAny; 
    #python makeShapeCards.py mca-4l-ttscale.txt bins/4l.txt 'nJet25' '3,0.5,3.5' $SYSTS $OPT_4L -o 4l_nJet_bl $BLoose; 
    #python makeShapeCards.py mca-4l-ttscale.txt bins/4l.txt 'nJet25' '3,0.5,3.5' $SYSTS $OPT_4L -o 4l_nJet_bt $BTight; 
    echo "Done at $(date)"
fi
