if [[ "$1" == "inclusive" ]]; then
combineCards.py ss_ee=2lss_ee_nJet.card.txt   \
                ss_mm=2lss_mumu_nJet.card.txt \
                ss_em=2lss_em_nJet.card.txt     > 2lss_nJet.card.txt
combineCards.py ss_ee=2lss_ee_MVA.card.txt   \
                ss_mm=2lss_mumu_MVA.card.txt \
                ss_em=2lss_em_MVA.card.txt     > 2lss_MVA.card.txt
combineCards.py .=2lss_nJet.card.txt tl=3l_nJet.card.txt ql=4l_nJet.card.txt > comb_nJet.card.txt
combineCards.py .=2lss_MVA.card.txt  tl=3l_MVA.card.txt  ql=4l_nJet.card.txt > comb_MVA.card.txt

elif [[ "$1" == "charge" ]]; then
    for X in ee em mumu; do
        combineCards.py ss_${X/mumu/mm}_pos=2lss_${X}_nJet_pos.card.txt \
                        ss_${X/mumu/mm}_neg=2lss_${X}_nJet_neg.card.txt > 2lss_${X}_QnJet.card.txt
        combineCards.py ss_${X/mumu/mm}_pos=2lss_${X}_MVA_pos.card.txt \
                        ss_${X/mumu/mm}_neg=2lss_${X}_MVA_neg.card.txt > 2lss_${X}_QMVA.card.txt
    done
    combineCards.py .=2lss_ee_QnJet.card.txt   \
                    .=2lss_mumu_QnJet.card.txt \
                    .=2lss_em_QnJet.card.txt     > 2lss_QnJet.card.txt
    combineCards.py .=2lss_ee_QMVA.card.txt   \
                    .=2lss_mumu_QMVA.card.txt \
                    .=2lss_em_QMVA.card.txt     > 2lss_QMVA.card.txt

    combineCards.py tl_pos=3l_nJet_pos.card.txt \
                    tl_neg=3l_nJet_neg.card.txt > 3l_QnJet.card.txt
    combineCards.py tl_pos=3l_MVA_pos.card.txt \
                    tl_neg=3l_MVA_neg.card.txt > 3l_QMVA.card.txt
    combineCards.py .=2lss_QnJet.card.txt tl=3l_QnJet.card.txt ql=4l_nJet.card.txt > comb_QnJet.card.txt
    combineCards.py .=2lss_QMVA.card.txt  tl=3l_QMVA.card.txt  ql=4l_nJet.card.txt > comb_QMVA.card.txt
elif [[ "$1" == "consistency" ]]; then
    for X in comb_{QnJet,QMVA}.card.txt; do
        text2workspace.py $X -P HiggsAnalysis.CombinedLimit.PhysicsModel:multiSignalModel --PO verbose \
                --PO 'map=ss_ee.*/ttH.*:r_ee[1,-6,12]' \
                --PO 'map=ss_em.*/ttH.*:r_em[1,-6,12]' \
                --PO 'map=ss_mm.*/ttH.*:r_mm[1,-6,12]' \
                --PO 'map=tl_.*/ttH.*:r_3l[1,-6,12]' \
                --PO 'map=ql_.*/ttH.*:r_4l[1,-6,12]' \
                -o CCC_${X/.card.txt/}.root
    done
fi


##   #combineCards.py 2lss_{ee,em,mumu}_nJet.card.txt > 2lss_nJet.card.txt
##   #combineCards.py 2lss_{ee,em,mumu}_nJet_{pos,neg}_{bl,bt}.card.txt > 2lss_QBnJet.card.txt
##   #combineCards.py 2lss_{ee,em,mumu}_nJet_{pos,neg}_{bl,btFRMC}.card.txt > 2lss_QBnJetFRMC.card.txt
##   combineCards.py 2lss_{ee,em,mumu}_nJet_{pos,neg}.card.txt > 2lss_QnJet.card.txt
##   
##   #combineCards.py 2lss_{ee,em,mumu}_MVA_4j_6v.card.txt > 2lss_MVA.card.txt
##   #combineCards.py 2lss_{ee,em,mumu}_MVA_4j_6v_{pos,neg}_{bl,bt}.card.txt  > 2lss_QBMVA.card.txt
##   #combineCards.py 2lss_{ee,em,mumu}_MVA_4j_6v_{pos,neg}_{bl,btFRMC}.card.txt  > 2lss_QBMVAFRMC.card.txt
##   combineCards.py 2lss_{ee,em,mumu}_MVA_4j_6v_{pos,neg}.card.txt  > 2lss_QMVA.card.txt
##   for X in ee em mumu; do
##       #combineCards.py 2lss_${X}_nJet_{pos,neg}_{bl,btFRMC}.card.txt > 2lss_${X}_QBnJetFRMC.card.txt
##       #combineCards.py 2lss_${X}_MVA_4j_6v_{pos,neg}_{bl,btFRMC}.card.txt  > 2lss_${X}_QBMVAFRMC.card.txt
##       combineCards.py 2lss_${X}_nJet_{pos,neg}.card.txt > 2lss_${X}_QnJet.card.txt
##       combineCards.py 2lss_${X}_MVA_4j_6v_{pos,neg}.card.txt  > 2lss_${X}_QMVA.card.txt
##   done
##   
##   #combineCards.py 3l_nJet_{pos,neg}_{bl,btFRMC}.card.txt  > 3l_QBnJetFRMC.card.txt
##   #combineCards.py 3l_MVA_{pos,neg}_{bl,bt}.card.txt   > 3l_QBMVA.card.txt
##   #combineCards.py 3l_MVA_{pos,neg}_{bl,btFRMC}.card.txt   > 3l_QBMVAFRMC.card.txt
##   combineCards.py 3l_nJet_{pos,neg}.card.txt  > 3l_QnJet.card.txt
##   combineCards.py 3l_MVA_{pos,neg}.card.txt   > 3l_QMVA.card.txt
##   
##   #combineCards.py 4l_nJet_{bl,bt}.card.txt  > 4l_BnJet.card.txt
##   
##   #combineCards.py 2lss_nJet.card.txt 3l_nJet.card.txt 4l_nJet.card.txt > comb_nJet.card.txt
##   #combineCards.py 2lss_QBnJet.card.txt 3l_QBnJet.card.txt 4l_BnJet.card.txt > comb_QBnJet.card.txt
##   #combineCards.py 2lss_QBnJetFRMC.card.txt 3l_QBnJetFRMC.card.txt 4l_BnJet.card.txt > comb_QBnJetFRMC.card.txt
##   #combineCards.py 2lss_MVA.card.txt 3l_MVA.card.txt 4l_nJet.card.txt > comb_MVA.card.txt
##   #combineCards.py 2lss_QBMVA.card.txt 3l_QBMVA.card.txt 4l_BnJet.card.txt > comb_QBMVA.card.txt
##   #combineCards.py 2lss_QBMVAFRMC.card.txt 3l_QBMVAFRMC.card.txt 4l_BnJet.card.txt > comb_QBMVAFRMC.card.txt
##   combineCards.py 2lss_QnJet.card.txt 3l_QnJet.card.txt 4l_BnJet.card.txt > comb_QnJet.card.txt
##   combineCards.py 2lss_QMVA.card.txt 3l_QMVA.card.txt 4l_BnJet.card.txt > comb_QMVA.card.txt
