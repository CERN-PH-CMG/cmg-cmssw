#!/bin/bash

T="/data1/p/peruzzi/skimmed_mix_nov22"
CORE="-P $T --s2v --tree treeProducerSusyMultilepton"
CORE="${CORE} -F sf/t {P}/5_allnewfriends_v7/evVarFriend_{cname}.root"

#T="../../cfg/sync_nov16"
#CORE="-P $T --s2v --tree treeProducerSusyMultilepton"
#CORE="${CORE} -F sf/t {P}/0_allfriends/evVarFriend_{cname}.root"

#CORE="${CORE} -p TTWv2_RA5_sync --mcc susy-multilepton/susy_2lssinc_triggerdefs.txt -A alwaystrue TT '(hasTT)'"
FMT='"{run:1d} {lumi:9d} {evt:12d}\t{LepGood1_pdgId:+2d} {LepGood1_conePt:5.1f}\t{LepGood2_pdgId:+2d} {LepGood2_conePt:5.1f}\t{nJet40}\t{nBJetMedium25:2d}\t{met_pt:5.1f}\t{htJet40j:6.1f}\t{SR:2d}\t{appWeight_Loop:5f}"'

CORE="${CORE} -p TT2l --mcc susy-multilepton/susy_2lssinc_triggerdefs.txt -A alwaystrue not_hasTT '(hasTF || hasFF)' -A alwaystrue SR '(SR==2)'"
#FMT='"{evt:1d}\t{LepGood1_pt:5.2f}\t{LepGood2_pt:5.2f}\t{LepGood2_conePt:5.2f}\t{LepGood2_jetPtRelv2:5.2f}\t{LepGood2_pt/LepGood2_jetPtRatiov2:5.2f}\tHH SR{SR:2d}"'

#CORE="${CORE} -p data_forflips --mcc susy-multilepton/susy_2lssinc_triggerdefs.txt -A alwaystrue onlyTF 'hasTF' -A alwaystrue SR 'SR>0' -A alwaystrue nomumu '(abs(LepGood1_pdgId)!=13 || abs(LepGood2_pdgId)!=13)' -A alwaystrue etares '(abs(LepGood1_eta)>0.8 && abs(LepGood2_eta)>0.8)'" #-A alwaystrue weightnon0 'appWeight!=0'"
#FMT='"{evt:1d}\t{LepGood1_pt:5.2f}\t{LepGood2_pt:5.2f}\t{LepGood2_conePt:5.2f}\t{LepGood2_jetPtRelv2:5.2f}\t{LepGood2_pt/LepGood2_jetPtRatiov2:5.2f}\t{SR:2d}"' # formato matthieu
#FMT='"{run:1d} {lumi:9d} {evt:12d}\t{nLepLoose_Mini:2d}\t{LepGood1_pdgId:+2d} {LepGood1_conePt:5.1f}\t{LepGood2_pdgId:+2d} {LepGood2_conePt:5.1f}\t{nJet40}\t{nBJetMedium25:2d}\t{met_pt:5.1f}\t{htJet40j:6.1f}\t{SR:2d}"' # formato ttW

#FMT='"{evt:1d}\t{LepGood1_pdgId:d} {LepGood2_pdgId:d}\t\t{LepGood1_pt:7.4f}\t{LepGood1_eta:7.4f}\t{LepGood1_miniRelIso:7.4f}\t{LepGood1_jetPtRelv2:7.4f}\t{LepGood1_jetPtRatiov2:7.4f}\t\t{LepGood2_pt:7.4f}\t{LepGood2_eta:7.4f}\t{LepGood2_miniRelIso:7.4f}\t{LepGood2_jetPtRelv2:7.4f}\t{LepGood2_jetPtRatiov2:7.4f}\t\t{nJet40_Mini:d} {nBJetMedium25_Mini:d} {htJet40j_Mini:.1f} {met_pt:.1f}"' # formato debug 2 leg



POST="";
WHAT="$1"; shift;
if [[ "$WHAT" == "mccounts" ]]; then
    FMT='"{run:1d} {lumi:9d} {evt:12d}"'
    GO="python mcDump.py $CORE susy-multilepton/mca-Spring15-analysis-all.txt susy-multilepton/susy_2lss_multiiso.txt  $FMT"
    POST="| awk '{print \$3}' "
elif [[ "$WHAT" == "mcyields" ]]; then
    GO="python mcAnalysis.py $CORE susy-multilepton/mca-Spring15-analysis-sideband.txt susy-multilepton/susy_2lss_multiiso.txt   -f -G"
elif [[ "$WHAT" == "mcplots" ]]; then
    GO="python mcPlots.py $CORE susy-multilepton/mca-Spring15-analysis-all.txt susy-multilepton/susy_2lss_multiiso.txt -f -G  -j 8 -f --legendWidth 0.30 susy-multilepton/susy_2lss_plots.txt"
elif [[ "$WHAT" == "mcdumps" ]]; then
    GO="python mcDump.py --dumpFile .fdump.txt $CORE susy-multilepton/mca-Spring15-sync.txt  susy-multilepton/susy_2lss_multiiso.txt   $FMT"
    POST=" &&  sort -n -k1 -k2 -k3 .fdump.txt > fdump.txt && rm .fdump.txt"
else
    echo "I don't know what you want"
    exit;
fi

SAVE="${GO}"
#for LL  in ee em mm ll; do 
#for SR  in 0 10 20 30; do # 0X 1X 2X 3X; do 
#for LPt in hh hl; do
#for LL  in ll; do 
#for SR  in 0; do # 0X 1X 2X 3X; do 
#for LPt in hh; do
for LL  in ll; do 
for SR  in 0; do # 0X 1X 2X 3X; do 
for LPt in ii; do
for MOD in multiiso; do #oldpresel ptrel miniiso; do

GO="${SAVE}"
case $SR in
0)   GO="${GO} -R nBjet nBjet0 'nBJetMedium25 >= 0' " ;;
00)  GO="${GO} -R nBjet nBjet0 'nBJetMedium25 == 0' " ;;
10)  GO="${GO} -R nBjet nBjet1 'nBJetMedium25 == 1' " ;;
20)  GO="${GO} -R nBjet nBjet2 'nBJetMedium25 == 2' " ;;
20+)  GO="${GO} -R nBjet nBjet2 'nBJetMedium25 >= 2' " ;;
30)  GO="${GO} -R nBjet nBjet3 'nBJetMedium25 >= 3' " ;;
esac;

case $LL in
ee)  GO="${GO} -R anyll ee 'abs(LepGood1_pdgId) == 11 && abs(LepGood2_pdgId) == 11' -A alwaystrue trig_ee Triggers_ee" ;;
em)  GO="${GO} -R anyll em 'abs(LepGood1_pdgId) != abs(LepGood2_pdgId)' -A alwaystrue trig_em Triggers_em" ;;
mm)  GO="${GO} -R anyll mm 'abs(LepGood1_pdgId) == 13 && abs(LepGood2_pdgId) == 13' -A alwaystrue trig_mm Triggers_mm" ;;
ll)  GO="${GO} -A alwaystrue trig_ll '(abs(LepGood1_pdgId) != 11 || abs(LepGood2_pdgId) != 11 || Triggers_ee) && (abs(LepGood1_pdgId) != 13 || abs(LepGood2_pdgId) != 13 || Triggers_mm) && (abs(LepGood1_pdgId)==abs(LepGood2_pdgId) || Triggers_em)'" ;;
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
    if [[ "${WHAT}" == "mcplots" ]]; then
        echo "$GO --pdir /afs/cern.ch/user/p/peruzzi/www/ra5sync_oct29//2lss_${MOD}/${LL}_pt_${LPt}/${SR}${PF}/"
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
