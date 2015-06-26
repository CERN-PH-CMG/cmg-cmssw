#!/bin/bash

if [[ "$1" == "afs" ]]; then
    T="/afs/cern.ch/work/g/gpetrucc/TREES_72X_040115";
    J=4;
elif [[ "$1" == "SingleLepAFS" ]]; then
#    T="/afs/cern.ch/work/k/kirschen/public/PlotExampleSamples/V2";
    T="/afs/cern.ch/work/k/kirschen/public/PlotExampleSamples/V3";
    J=4;
elif [[ "$HOSTNAME" == "cmsphys10" ]]; then
    T="/data/g/gpetrucc/TREES_72X_040115";
    J=8;
else
    T="/afs/cern.ch/work/g/gpetrucc/TREES_72X_040115";
    J=4;
fi

LUMI=4.0
OUTDIR="susy_cards_1l_4fb"
#OPTIONS=" -P $T -j $J -l $LUMI -f --s2v --tree treeProducerSusyMultilepton --od $OUTDIR --asimov "
#OPTIONS=" $OPTIONS -F sf/t $T/0_lepMVA_v1/evVarFriend_{cname}.root "
OPTIONS=" -P $T -j $J -l $LUMI -f --s2v --tree treeProducerSusySingleLepton --od $OUTDIR --asimov "


function makeCard_1l {
    local EXPR=$1; local BINS=$2; local SYSTS=$3; local OUT=$4; local GO=$5

    # b-jet cuts
    case $nB in
    0B)  GO="${GO} -R 1nB 0nB nBJetCMVAMedium30==0 " ;;
    1B)  GO="${GO} -R 1nB 1nB nBJetCMVAMedium30==1 " ;;
    2B)  GO="${GO} -R 1nB 2nB nBJetCMVAMedium30==2 " ;;
    2Btop)  GO="${GO} -R 1nB 2nB nBJetCMVAMedium30==2&&Topness>5 " ;;
    3p)  GO="${GO} -R 1nB 3nBp nBJetCMVAMedium30>=3 " ;;
    esac;

    # ST categories
    case $ST in
    ST0)  GO="${GO} -R st200 st200250 LepGood_pt[tightLepsIdx[0]]+met_pt>200&&LepGood_pt[tightLepsIdx[0]]+met_pt<250 " ;;
    ST1)  GO="${GO} -R st200 st250350 LepGood_pt[tightLepsIdx[0]]+met_pt>250&&LepGood_pt[tightLepsIdx[0]]+met_pt<350 " ;;
    ST2)  GO="${GO} -R st200 st350450 LepGood_pt[tightLepsIdx[0]]+met_pt>350&&LepGood_pt[tightLepsIdx[0]]+met_pt<450 " ;;
    ST3)  GO="${GO} -R st200 st450600 LepGood_pt[tightLepsIdx[0]]+met_pt>450&&LepGood_pt[tightLepsIdx[0]]+met_pt<600 " ;;
    ST4)  GO="${GO} -R st200 st600Inf LepGood_pt[tightLepsIdx[0]]+met_pt>600 " ;;
    esac;

    # jet multiplicities
    case $nJ in
    45j)  GO="${GO} -R geq6j 45j nCentralJet30>=4&&nCentralJet30<=5"  ;;
    68j)  GO="${GO} -R geq6j 67j nCentralJet30>=6&&nCentralJet30<=8"  ;;
    6Infj)  GO="${GO} -R geq6j geq6j nCentralJet30>=6"  ;;
    9Infj)  GO="${GO} -R geq6j geq8j nCentralJet30>=9"  ;;
    68TTj)  GO="${GO} -R geq6j 68TTj nCentralJet30+2*nHighPtTopTagPlusTau23>=6&&nCentralJet30+2*nHighPtTopTagPlusTau23<9"  ;; 
    9InfTTj)  GO="${GO} -R geq6j 9InfTTj nCentralJet30+2*nHighPtTopTagPlusTau23>=9"  ;;
    esac;

    # HT and "R&D" categories
    case $HT in
    HT0) GO="${GO} -R ht500 ht5001000 htJet30j>500&&htJet30j<1000"  ;;
    HT1) GO="${GO} -R ht500 ht1000Inf htJet30j>=1000"  ;;
    HTDPhi) GO="${GO} -R ht500 ht1000Inf htJet30j>=1000 -R dp1 dp05 fabs(DeltaPhiLepW)>0.5 "  ;;
    HTStop) GO="${GO} -R ht500 ht1000Inf htJet30j>=1000 -R dp1 dp05 fabs(DeltaPhiLepW)>0.5 -A dp1 stopness (TopVarsMETovTopMin[0]-0.5)/0.5+(TopVarsMtopMin[0]-175)/175>1.25"  ;;
    HTTop) GO="${GO} -R ht500 ht1000Inf htJet30j>=1000 -R dp1 dp05 fabs(DeltaPhiLepW)>0.5 -A dp1 stopness (TopVarsMETovTopMin[0]-0.5)/0.5+(TopVarsMtopMin[0]-175)/175>1.25&&Topness>5"  ;;
    HTLowLepPt) GO="${GO} -R ht500 ht1000Inf htJet30j>=1000 -R 1tl 1tllowpt nTightLeps==1&&LepGood_pt[tightLepsIdx[0]]<=25  -R dp1 dp00 fabs(DeltaPhiLepW)>0.0 -A dp1 stopness (TopVarsMETovTopMin[0]-0.5)/0.5+(TopVarsMtopMin[0]-175)/175>1.25&&Topness>5"  ;;
    HTLowLepPtDPhi) GO="${GO} -R ht500 ht1000Inf htJet30j>=1000 -R 1tl 1tllowpt nTightLeps==1&&LepGood_pt[tightLepsIdx[0]]<=25"  ;;
    HTTTYes) GO="${GO} -R ht500 ht1000Inf htJet30j>=1000&&nHighPtTopTagPlusTau23>=1"  ;;
    HTTTNo) GO="${GO} -R ht500 ht1000Inf htJet30j>=1000&&nHighPtTopTagPlusTau23==0"  ;;
    esac;

    if [[ "$PRETEND" == "1" ]]; then
        echo "making datacard $OUT from makeShapeCardsSusy.py mca-Phys14_1l.txt bins/1l_CardsFullCutFlow.txt \"$EXPR\" \"$BINS\" $SYSTS $GO --dummyYieldsForZeroBkg;"
    else
        echo "making datacard $OUT from makeShapeCardsSusy.py mca-Phys14_1l.txt bins/1l_CardsFullCutFlow.txt \"$EXPR\" \"$BINS\" $SYSTS $GO --dummyYieldsForZeroBkg;"
        python makeShapeCardsSusy.py mca-Phys14_1l.txt bins/1l_CardsFullCutFlow.txt "$EXPR" "$BINS" $SYSTS -o $OUT $GO --dummyYieldsForZeroBkg;
        echo "  -- done at $(date)";
    fi;
}


function combineCardsSmart {
    DummyC=0
    AllC=0
    CMD=""
    for C in $*; do
        # missing datacards 
        test -f $C || continue;

	if grep -q "DummyContent" $C; then
	    echo "empty bin ${C}" >&2 
	    DummyC=$(($DummyC+1))
	    if grep -q "observation 0.0$" $C; then
		echo "this is not the way it was intended..."
	    fi
	fi

        grep -q "observation 0.0$" $C && continue
	AllC=$((AllC+1))
        CMD="${CMD} $(basename $C .card.txt)=$C ";
    done
    if [[ "$CMD" == "" ]]; then
        echo "Not any card found in $*" 1>&2 ;
    else
#	echo "combineCards.py $CMD" >&2
        combineCards.py $CMD
    fi
    if [[ "$DummyC" != "0" ]]; then
	echo "In total $DummyC out of $AllC are empty, but taken into account by adding DummyContent." >&2
    fi
    #echo "In total $DummyC out of $AllC are empty, but taken into account by adding DummyContent." >&2
}

if [[ "$1" == "--pretend" ]]; then
    PRETEND=1; shift;
fi;

if [[ "$2" == "1l-2015" ]]; then
    OPTIONS=" $OPTIONS -F sf/t $T/PHYS14_V3_Friend_Batool/evVarFriend_{cname}.root "

    SYSTS="syst/susyDummy.txt"
    CnC_expr="1" #not used as of now
    CnC_bins="[0.5,1.5]"


    echo "Making individual datacards"
    for ST in ST0 ST1 ST2 ST3 ST4; do for nJ in 68j 6Infj 9Infj; do for nB in 2B 3p; do for HT in HT0 HT1; do
#    for ST in ST0 ST1 ST2 ST3 ST4; do for nJ in 45j 68j 6Infj 9Infj 68TTj 9InfTTj; do for nB in 0B 1B 2B 3p; do for HT in HT0 HT1 HTDPhi HTStop HTTop HTLowLepPt HTLowLepPtDPhi HTTTYes HTTTNo; do
        echo " --- CnC2015X_${nB}_${ST}_${nJ}_${HT} ---"
        makeCard_1l $CnC_expr $CnC_bins $SYSTS CnC2015X_${nB}_${ST}_${nJ}_${HT} "$OPTIONS";
    done; done; done; done
    #exit
    echo "Making combined datacards"
    for D in $OUTDIR/T[0-9]*; do
        test -f $D/CnC2015X_2B_ST0_68j_HT0.card.txt || continue
        (cd $D && echo "    $D";
#        for nB in 0B 1B 2B 3p; do
        for nB in 2B 3p; do
            combineCardsSmart CnC2015X_${nB}_{ST0,ST1,ST2,ST3,ST4}_6Infj_{HT0,HT1}.card.txt >  CnC2015X_${nB}_standardnJ.card.txt
            combineCardsSmart CnC2015X_${nB}_{ST0,ST1,ST2,ST3,ST4}_{68j,9Infj}_{HT0,HT1}.card.txt >  CnC2015X_${nB}_finenJ.card.txt

#            combineCardsSmart CnC2015X_${nB}_{ST0,ST1,ST2,ST3,ST4}_6Infj_HT1.card.txt >  CnC2015X_${nB}_standardnJ_HT1.card.txt # only high HT
#            combineCardsSmart CnC2015X_${nB}_{ST0,ST1,ST2,ST3,ST4}_6Infj_HTLowLepPt.card.txt >  CnC2015X_${nB}_standardnJ_HTLowLepPt.card.txt # only high HT
#            combineCardsSmart CnC2015X_${nB}_{ST0,ST1,ST2,ST3,ST4}_6Infj_HTLowLepPtDPhi.card.txt >  CnC2015X_${nB}_standardnJ_HTLowLepPtDPhi.card.txt # only high HT
#            combineCardsSmart CnC2015X_${nB}_{ST0,ST1,ST2,ST3,ST4}_6Infj_HTDPhi.card.txt >  CnC2015X_${nB}_standardnJ_HTDPhi.card.txt # only high HT
#            combineCardsSmart CnC2015X_${nB}_{ST0,ST1,ST2,ST3,ST4}_6Infj_HTStop.card.txt >  CnC2015X_${nB}_standardnJ_HTStop.card.txt # only high HT
#            combineCardsSmart CnC2015X_${nB}_{ST0,ST1,ST2,ST3,ST4}_6Infj_HTTop.card.txt >  CnC2015X_${nB}_standardnJ_HTTop.card.txt # only high HT
#            combineCardsSmart CnC2015X_${nB}_{ST0,ST1,ST2,ST3,ST4}_6Infj_{HTTTYes,HTTTNo}.card.txt >  CnC2015X_${nB}_standardnJ_HTTTYesNo.card.txt # only high HT
#            combineCardsSmart CnC2015X_${nB}_{ST0,ST1,ST2,ST3,ST4}_6Infj_HTTTYes.card.txt >  CnC2015X_${nB}_standardnJ_HTTTYes.card.txt # only high HT
#            combineCardsSmart CnC2015X_${nB}_{ST0,ST1,ST2,ST3,ST4}_6Infj_HTTTNo.card.txt >  CnC2015X_${nB}_standardnJ_HTTTNo.card.txt # only high HT
#
#            combineCardsSmart CnC2015X_${nB}_{ST0,ST1,ST2,ST3,ST4}_{68j,9Infj}_HT1.card.txt >  CnC2015X_${nB}_finenJ_HT1.card.txt # only high HT
#            combineCardsSmart CnC2015X_${nB}_{ST0,ST1,ST2,ST3,ST4}_{68j,9Infj}_HTLowLepPt.card.txt >  CnC2015X_${nB}_finenJ_HTLowLepPt.card.txt # only high HT
#            combineCardsSmart CnC2015X_${nB}_{ST0,ST1,ST2,ST3,ST4}_{68j,9Infj}_HTLowLepPtDPhi.card.txt >  CnC2015X_${nB}_finenJ_HTLowLepPtDPhi.card.txt # only high HT
#            combineCardsSmart CnC2015X_${nB}_{ST0,ST1,ST2,ST3,ST4}_{68j,9Infj}_HTDPhi.card.txt >  CnC2015X_${nB}_finenJ_HTDPhi.card.txt # only high HT
#            combineCardsSmart CnC2015X_${nB}_{ST0,ST1,ST2,ST3,ST4}_{68j,9Infj}_HTStop.card.txt >  CnC2015X_${nB}_finenJ_HTStop.card.txt # only high HT
#            combineCardsSmart CnC2015X_${nB}_{ST0,ST1,ST2,ST3,ST4}_{68j,9Infj}_HTTop.card.txt >  CnC2015X_${nB}_finenJ_HTTop.card.txt # only high HT
#            combineCardsSmart CnC2015X_${nB}_{ST0,ST1,ST2,ST3,ST4}_{68j,9Infj}_{HTTTYes,HTTTNo}.card.txt >  CnC2015X_${nB}_finenJ_HTTTYesNo.card.txt # only high HT
#            combineCardsSmart CnC2015X_${nB}_{ST0,ST1,ST2,ST3,ST4}_{68j,9Infj}_HTTTYes.card.txt >  CnC2015X_${nB}_finenJ_HTTTYes.card.txt # only high HT
#            combineCardsSmart CnC2015X_${nB}_{ST0,ST1,ST2,ST3,ST4}_{68j,9Infj}_HTTTNo.card.txt >  CnC2015X_${nB}_finenJ_HTTTNo.card.txt # only high HT
#
#            combineCardsSmart CnC2015X_${nB}_{ST0,ST1,ST2,ST3,ST4}_{68TTj,9InfTTj}_HT1.card.txt >  CnC2015X_${nB}_finenJ_HT1TT.card.txt # only high HT
#            combineCardsSmart CnC2015X_${nB}_{ST0,ST1,ST2,ST3,ST4}_{68TTj,9InfTTj}_HTTop.card.txt >  CnC2015X_${nB}_finenJ_HTTopTT.card.txt # only high HT
        done
        combineCardsSmart CnC2015X_{2B,3p}_standardnJ.card.txt >  CnC2015X_standardnJ.card.txt # standard nJet-binning; HT-binning
        combineCardsSmart CnC2015X_{2B,3p}_finenJ.card.txt >  CnC2015X_finenJ.card.txt #fine nJet-binning; HT-binning

#	#standard nJet-binning
#        combineCardsSmart CnC2015X_{2B,3p}_standardnJ_HT1.card.txt >  CnC2015X_standardnJ_HT1.card.txt # only high HT
#        combineCardsSmart CnC2015X_{2B,3p}_standardnJ_HTLowLepPt.card.txt >  CnC2015X_standardnJ_HTLowLepPt.card.txt #high HT, soft lepton (topness/single topness selection)
#        combineCardsSmart CnC2015X_{2B,3p}_standardnJ_HTLowLepPtDPhi.card.txt >  CnC2015X_standardnJ_HTLowLepPtDPhi.card.txt #hight HT, soft lepton dphi selection
#        combineCardsSmart CnC2015X_{2B,3p}_standardnJ_HTDPhi.card.txt >  CnC2015X_standardnJ_HTDPhi.card.txt # high HT, dphi >0.5
#        combineCardsSmart CnC2015X_{2B,3p}_standardnJ_HTStop.card.txt >  CnC2015X_standardnJ_HTStop.card.txt # high HT, dphi >0.5 + single topness 
#        combineCardsSmart CnC2015X_{2B,3p}_standardnJ_HTTop.card.txt >  CnC2015X_standardnJ_HTTop.card.txt   # high HT, dphi >0.5 + single topness + topness
#        combineCardsSmart CnC2015X_{2B,3p}_standardnJ_HTTTYesNo.card.txt >  CnC2015X_standardnJ_HTTTYesNo.card.txt # high HT, high pt-top-tagging yes/no combined
#        combineCardsSmart CnC2015X_{2B,3p}_standardnJ_HTTTYes.card.txt >  CnC2015X_standardnJ_HTTTYes.card.txt # high HT, high pt-top-tagging Yes
#        combineCardsSmart CnC2015X_{2B,3p}_standardnJ_HTTTNo.card.txt >  CnC2015X_standardnJ_HTTTNo.card.txt # high HT, high pt-top-tagging No
#        combineCardsSmart CnC2015X_standardnJ_{HT1,HTLowLepPt}.card.txt >  CnC2015X_standardnJ_HighLowLepPt.card.txt # high HT, combining hard/soft leptons
#
#	# fine nJet-binning
#        combineCardsSmart CnC2015X_{2B,3p}_finenJ_HT1.card.txt >  CnC2015X_finenJ_HT1.card.txt
#        combineCardsSmart CnC2015X_{2B,3p}_finenJ_HTLowLepPt.card.txt >  CnC2015X_finenJ_HTLowLepPt.card.txt
#        combineCardsSmart CnC2015X_{2B,3p}_finenJ_HTLowLepPtDPhi.card.txt >  CnC2015X_finenJ_HTLowLepPtDPhi.card.txt
#        combineCardsSmart CnC2015X_{2B,3p}_finenJ_HTDPhi.card.txt >  CnC2015X_finenJ_HTDPhi.card.txt
#        combineCardsSmart CnC2015X_{2B,3p}_finenJ_HTStop.card.txt >  CnC2015X_finenJ_HTStop.card.txt
#        combineCardsSmart CnC2015X_{2B,3p}_finenJ_HTTop.card.txt >  CnC2015X_finenJ_HTTop.card.txt
#        combineCardsSmart CnC2015X_{2B,3p}_finenJ_HTTTYesNo.card.txt >  CnC2015X_finenJ_HTTTYesNo.card.txt
#        combineCardsSmart CnC2015X_{2B,3p}_finenJ_HTTTYes.card.txt >  CnC2015X_finenJ_HTTTYes.card.txt
#        combineCardsSmart CnC2015X_{2B,3p}_finenJ_HTTTNo.card.txt >  CnC2015X_finenJ_HTTTNo.card.txt
#        combineCardsSmart CnC2015X_finenJ_{HT1,HTLowLepPt}.card.txt >  CnC2015X_finenJ_HighLowLepPt.card.txt # high HT, combining hard/soft leptons
#
#	#extra
#        combineCardsSmart CnC2015X_{2B,3p}_finenJ_HT1TT.card.txt >  CnC2015X_finenJ_HT1TT.card.txt # high HT,  modified jet multiplicity (adding 2*nHighPtTopTagPlusTau23)
#        combineCardsSmart CnC2015X_{2B,3p}_finenJ_HTTopTT.card.txt >  CnC2015X_finenJ_HTTopTT.card.txt # high HT, modified jet multiplicity (adding 2*nHighPtTopTagPlusTau23) and  dphi >0.5 + single topness + topness

        )
    done
    echo "Done at $(date)";

fi

