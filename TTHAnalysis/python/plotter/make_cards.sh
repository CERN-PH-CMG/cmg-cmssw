#!/bin/bash

T=/data/b/botta/TTHAnalysis/trees/TREES_250513_HADD
if [[ "$HOSTNAME" == "olsnba03" ]]; then
    T=/data/gpetrucc/TREES_270413_HADD
fi
OPTIONS=" -P $T -j 6 -l 19.6 -f --xp data --od cards"

if [[ "$1" == "" || "$1" == "2lss" ]]; then
    OPTIONS="${OPTIONS} --FM sf/t $T/0_SFs_v1/sfFriend_{cname}.root "
    OPT_2L="${OPTIONS} -W puWeight*Eff_2lep*SF_btag*SF_LepMVATight_2l*SF_LepTightCharge_2l*SF_trig2l"
    MVA_2L="-F finalMVA/t $T/0_finalmva_2lss/finalMVA_2LSS_{cname}.root"
    POS=" -A same-charge positive \"LepGood1_charge>0\" "
    NEG=" -A same-charge positive \"LepGood1_charge<0\" "
    for X in 2lss_{em,mumu,ee}; do 
        #if [[ "$X" != "2lss_mumu" ]]; then continue; fi
        ~gpetrucc/sh/bann $X
        #python makeShapeCards.py mca-2lss-data.txt bins/$X.txt 'nJet25'  '5,1.5,6.5' systsEnv.txt $OPT_2L -o ${X}_nJet;
        python makeShapeCards.py mca-2lss-data.txt bins/$X.txt 'nJet25' '5,1.5,6.5' systsEnv.txt $OPT_2L -o ${X}_nJet_pos $POS; 
        python makeShapeCards.py mca-2lss-data.txt bins/$X.txt 'nJet25' '5,1.5,6.5' systsEnv.txt $OPT_2L -o ${X}_nJet_neg $NEG; 
        python makeShapeCards.py mca-2lss-data.txt bins/$X.txt 'FinalMVA_2LSS_BDTG' '8,-0.8,0.8' systsEnv.txt $OPT_2L -o ${X}_MVA $MVA_2L;
        echo "Done at $(date)"
    done
fi

if [[ "$1" == "" || "$1" == "3l" ]]; then
    OPTIONS="${OPTIONS} --FM sf/t $T/0_SFs_v2/sfFriend_{cname}.root "
    OPT_3L="${OPTIONS} -W  puWeight*Eff_3lep*SF_btag*SF_LepMVALoose_3l "
    MVA_3L="-F finalMVA/t $T/0_finalmva_3l/finalMVA_3L_{cname}.root"
    POS=" -A same-charge positive \"LepGood1_charge+LepGood2_charge+LepGood3_charge>0\" "
    NEG=" -A same-charge positive \"LepGood1_charge+LepGood2_charge+LepGood3_charge<0\" "
    python makeShapeCards.py mca-3l-data.txt bins/3l_auto.txt 'nJet25' '4,1.5,5.5' systsEnv.txt $OPT_3L -o 3l_loose_nJet; python makeShapeCards.py mca-3l-data.txt bins/3l_auto.txt 'nJet25' '4,1.5,5.5' systsEnv.txt $OPT_3L -o 3l_loose_nJet_pos $POS; 
    python makeShapeCards.py mca-3l-data.txt bins/3l_auto.txt 'nJet25' '4,1.5,5.5' systsEnv.txt $OPT_3L -o 3l_loose_nJet_neg $NEG; 
    python makeShapeCards.py mca-3l-data.txt bins/3l_auto.txt 'FinalMVA_3L_BDTG' '6,-1.0,0.8' systsEnv.txt $OPT_3L -o 3l_loose_MVA $MVA_3L;
    echo "Done at $(date)"
fi

if [[ "$1" == "" || "$1" == "3l_tight" ]]; then
    OPTIONS="${OPTIONS} --FM sf/t $T/0_SFs_v2/sfFriend_{cname}.root  "
    OPT_3L="${OPTIONS} -W  puWeight*Eff_3lep*SF_btag*SF_LepMVATight_3l*SF_LepTightCharge_3l"
    MVA_3L="-F finalMVA/t $T/0_finalmva_3l/finalMVA_3L_{cname}.root"
    POS=" -A same-charge positive \"LepGood1_charge+LepGood2_charge+LepGood3_charge>0\" "
    NEG=" -A same-charge positive \"LepGood1_charge+LepGood2_charge+LepGood3_charge<0\" "
    python makeShapeCards.py mca-3l_tight-data.txt bins/3l_tight.txt 'nJet25' '4,1.5,5.5' systsEnv.txt $OPT_3L -o 3l_nJet; 
    python makeShapeCards.py mca-3l_tight-data.txt bins/3l_tight.txt 'nJet25' '4,1.5,5.5' systsEnv.txt $OPT_3L -o 3l_nJet_pos $POS; 
    python makeShapeCards.py mca-3l_tight-data.txt bins/3l_tight.txt 'nJet25' '4,1.5,5.5' systsEnv.txt $OPT_3L -o 3l_nJet_neg $NEG; 
    python makeShapeCards.py mca-3l_tight-data.txt bins/3l_tight.txt 'FinalMVA_3L_BDTG' '9,-1.0,0.8' systsEnv.txt $OPT_3L -o 3l_MVA $MVA_3L;
    echo "Done at $(date)"
fi



if [[ "$1" == "" || "$1" == "4l" ]]; then
    OPTIONS="${OPTIONS} --FM sf/t $T/0_SFs_v1/sfFriend_{cname}.root  "
    OPT_4L="${OPTIONS} -W puWeight*Eff_4lep*SF_btag*SF_LepMVALoose_2l --xp FR_data"
    MVA_4L="-F finalMVA/t $T/0_finalmva_4l/finalMVA_4L_{cname}.root"
    python makeShapeCards.py mca-4l-data.txt bins/4l.txt 'nJet25' '4,0.5,4.5' systsEnv.txt $OPT_4L -o 4l_nJet; 
    python makeShapeCards.py mca-4l-data.txt bins/4l.txt 'FinalMVA_4L_BDTG' '5,-1.0,1.0' systsEnv.txt $OPT_4L -o 4l_MVA $MVA_4L;
    echo "Done at $(date)"
fi
