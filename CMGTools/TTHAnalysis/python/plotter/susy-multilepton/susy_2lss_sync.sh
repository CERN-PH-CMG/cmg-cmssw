#!/bin/bash

T="/afs/cern.ch/user/g/gpetrucc/w/TREES_72X_050515_2lssSync"
#T="/afs/cern.ch/user/g/gpetrucc/w/TREES_72X_210315_NoIso"
CORE="-P $T --s2v --tree treeProducerSusyMultilepton "
CORE="${CORE} -F sf/t {P}/1_lepJetReClean_Susy_v4/evVarFriend_{cname}.root"
CORE="${CORE} -X exclusive --mcc susy-multilepton/susy_2lssinc_lepchoice_multiiso.txt"

POST="";
WHAT="$1"; shift;
if [[ "$WHAT" == "mccounts" ]]; then
    GO="python mcAnalysis.py $CORE mca-Phys14.txt susy-multilepton/susy_2lss_multiiso.txt -p T1tttt_HM -f -G -u "
    POST="| awk '/all/{print \$2}' "
elif [[ "$WHAT" == "mcyields" ]]; then
    #GO="python mcAnalysis.py $CORE mca-Phys14.txt susy-multilepton/susy_2lss_sync.txt -p 'T1tttt_HM,T5tttt_MLDx,T5qqqqWW_HM,T5qqqqWWD,TTX,WZ,TT,WJets,DY' --pgroup TTX=TT[WZH] -f -G -l 4.0"
    GO="python mcAnalysis.py $CORE mca-Phys14.txt susy-multilepton/susy_2lss_multiiso.txt -p 'T1tttt_HM,TTX,TT' --pgroup TTX=TT[WZH] -f -G -l 4.0"
elif [[ "$WHAT" == "mcplots" ]]; then
    GO="python mcPlots.py $CORE mca-Phys14.txt susy-multilepton/susy_2lss_multiiso.txt -f -G -l 4.0  -p  T1tttt_HM,T5tttt_MLDx,T5qqqqWW_HM,T5qqqqWWD,TT.,TT,WJets,DY,T,TW,WZ  -j 8 -f   --showIndivSigs --noStackSig --legendWidth 0.30 --ss 5 susy-multilepton/susy_2lss_plots.txt"
elif [[ "$WHAT" == "mcdumps" ]]; then
    FMT='{run:1d} {lumi:9d} {evt:12d}\t{nLepGood_Mini:2d}\t{LepGood1_pdgId:+2d} {LepGood1_pt:5.1f}\t{LepGood2_pdgId:+2d} {LepGood2_pt:5.1f}\t{nJet40}\t{nBJetMedium25:2d}\t{met_pt:5.1f}\t{htJet40j:6.1f}\t{SR_Mini:2d}'
    python mcDump.py $CORE mca-Phys14.txt susy-multilepton/susy_2lss_multiiso.txt -p 'T1tttt_HM'  $FMT  | sort -n -k1 -k2 > 2lssInc_all.txt
    python mcDump.py $CORE mca-Phys14.txt susy-multilepton/susy_2lss_multiiso.txt -p 'T1tttt_HM' -X lep1_pt25 -X lep2_pt25 -X 'lep id' -X 'lep iso' -X 'lep dxy' -X 'ele cuts' $FMT | sort -n -k1 -k2 > 2lssInc_all_relaxLept.txt
    wc -l  2lssInc_all.txt  2lssInc_all_relaxLept.txt
    exit;
else
    echo "I don't know what you want"
    exit;
fi

SAVE="${GO}"
#for LL  in ee em mm ll; do 
for LL  in ee em mm; do 
for SR  in 0 10 20 30; do # 0X 1X 2X 3X; do 
for LPt in hh hl; do
for MOD in multiiso; do #oldpresel ptrel miniiso; do

GO="${SAVE}"
case $SR in
0)   GO="${GO} -R nBjet nBjet0 'nBJetMedium25 >= 0' " ;;
00)  GO="${GO} -R nBjet nBjet0 'nBJetMedium25 == 0' " ;;
10)  GO="${GO} -R nBjet nBjet1 'nBJetMedium25 == 1' " ;;
20)  GO="${GO} -R nBjet nBjet2 'nBJetMedium25 == 2' " ;;
20+)  GO="${GO} -R nBjet nBjet2 'nBJetMedium25 >= 2' " ;;
30)  GO="${GO} -R nBjet nBjet3 'nBJetMedium25 >= 3' " ;;
0[1-9X])  GO="${GO} -R nBjet nBjet0 'nBJetMedium25 == 0' -R met metSR 'met_pt > 50' -R ht htSR 'htJet40j > 200'" ;;
1[1-9X])  GO="${GO} -R nBjet nBjet1 'nBJetMedium25 == 1' -R met metSR 'met_pt > 50' -R ht htSR 'htJet40j > 200'" ;;
2[1-9X])  GO="${GO} -R nBjet nBjet2 'nBJetMedium25 == 2' -R met metSR 'met_pt > 50' -R ht htSR 'htJet40j > 200'" ;;
3[1-9X])  GO="${GO} -R nBjet nBjet3 'nBJetMedium25 >= 3' -R met metSR 'met_pt > 50' -R ht htSR 'htJet40j > 200'" ;;
2[1-9X]+)  GO="${GO} -R nBjet nBjet2 'nBJetMedium25 >= 2' -R met metSR 'met_pt > 50' -R ht htSR 'htJet40j > 200'" ;;

esac;
case $LL in
ee)  GO="${GO} -R anyll ee 'abs(LepGood1_pdgId) == 11 && abs(LepGood2_pdgId) == 11' " ;;
em)  GO="${GO} -R anyll em 'abs(LepGood1_pdgId) != abs(LepGood2_pdgId)' " ;;
mm)  GO="${GO} -R anyll mm 'abs(LepGood1_pdgId) == 13 && abs(LepGood2_pdgId) == 13' " ;;
esac;
case $LPt in
hl)  GO="${GO} -I lep2_pt25" ;;
ll)  GO="${GO} -I lep1_pt25 -X lep2_pt25" ;;
ii)  GO="${GO} -X lep1_pt25 -X lep2_pt25" ;;
esac;
case $MOD in
oldpresel) GO="${GO//multiiso/oldpresel} " ;;
miniiso)   GO="${GO//multiiso/miniiso} "   ;;
ptrel)     GO="${GO//multiiso/ptrel} "     ;;
esac;

if [[ "${WHAT}" == "mcplots" || "${WHAT}" == "mcrocs" ]]; then
    case $SR in
    0[1-9X])  GO=" ${GO/T1tttt_HM,T5tttt_MLDx,T5qqqqWW_HM,T5qqqqWWD/T5qqqqWW_H.,T5qqqqWWD,T6ttWW_H.}"   ;;
    1[1-9X])  GO=" ${GO/T1tttt_HM,T5tttt_MLDx,T5qqqqWW_HM,T5qqqqWWD/T1tttt_HM,T5tttt_MLDx,T6ttWW_H.,T5qqqqWW_H.,T5qqqqWWD}"   ;;
    2[1-9X])  GO=" ${GO/T1tttt_HM,T5tttt_MLDx,T5qqqqWW_HM,T5qqqqWWD/T1tttt_HL,T1tttt_HM,T5tttt_MLDx,T1ttbbWW_HL10,T6ttWW_H.}"   ;;
    2[1-9X]+) GO=" ${GO/T1tttt_HM,T5tttt_MLDx,T5qqqqWW_HM,T5qqqqWWD/T1tttt_HL,T1tttt_HM,T5tttt_MLDx,T1ttbbWW_HL10,T6ttWW_H.}"   ;;
    3[1-9X])  GO=" ${GO/T1tttt_HM,T5tttt_MLDx,T5qqqqWW_HM,T5qqqqWWD/T1tttt_HL,T1tttt_HM,T5tttt_MLDx,T1ttbbWW_HL10,T1ttbbWW_MM5}"   ;;
    0[1-9X])  GO=" ${GO/T1tttt_HM,T5tttt_MLDx,T5qqqqWW_HM,T5qqqqWWD/T5qqqqWW_HM,T5qqqqWWD}"   ;;
    1[1-9X])  GO=" ${GO/T1tttt_HM,T5tttt_MLDx,T5qqqqWW_HM,T5qqqqWWD/T1tttt_HM,T5tttt_MLDx,T5qqqqWW_HM,T5qqqqWWD}"   ;;
    2[1-9X])  GO=" ${GO/T1tttt_HM,T5tttt_MLDx,T5qqqqWW_HM,T5qqqqWWD/T1tttt_HL,T1tttt_HM,T5tttt_MLDx}"   ;;
    2[1-9X]+) GO=" ${GO/T1tttt_HM,T5tttt_MLDx,T5qqqqWW_HM,T5qqqqWWD/T1tttt_HL,T1tttt_HM,T5tttt_MLDx}"   ;;
    3[1-9X])  GO=" ${GO/T1tttt_HM,T5tttt_MLDx,T5qqqqWW_HM,T5qqqqWWD/T1tttt_HL,T1tttt_HM,T5tttt_MLDx}"   ;;
    0[1-9X])  GO=" ${GO/T1tttt_HM,T5tttt_MLDx,T5qqqqWW_HM,T5qqqqWWD/T5qqqqWW_HM.*,T5qqqqWWD,T5qqqqWWD_.*}"   ;;
    1[1-9X])  GO=" ${GO/T1tttt_HM,T5tttt_MLDx,T5qqqqWW_HM,T5qqqqWWD/T1tttt_HM.*,T5tttt_MLDx.*,T5qqqqWW_HM.*,T5qqqqWWD,T5qqqqWWD_.*}"   ;;
    2[1-9X])  GO=" ${GO/T1tttt_HM,T5tttt_MLDx,T5qqqqWW_HM,T5qqqqWWD/T1tttt_HL.*,T1tttt_HM.*,T5tttt_MLDx.*}"   ;;
    2[1-9X]+) GO=" ${GO/T1tttt_HM,T5tttt_MLDx,T5qqqqWW_HM,T5qqqqWWD/T1tttt_HL.*,T1tttt_HM.*,T5tttt_MLDx.*}"   ;;
    3[1-9X])  GO=" ${GO/T1tttt_HM,T5tttt_MLDx,T5qqqqWW_HM,T5qqqqWWD/T1tttt_HL.*,T1tttt_HM.*,T5tttt_MLDx.*}"   ;;
    esac
    if [[ "${WHAT}" == "mcplots" ]]; then
        echo "$GO --pdir plots/72X/v2/4fb/vars/2lss_${MOD}/${LL}_pt_${LPt}/${SR}${PF}/"
    else
        echo "$GO -o plots/72X/v2/4fb/vars/2lss_${MOD}/${LL}_pt_${LPt}/${SR}${PF}/rocs.root"
    fi
else
    echo "echo; echo \" ===== SR $SR${PF} $LL $LPt $MOD $LId ===== \"; $GO $POST"
fi

done
done
done
done
