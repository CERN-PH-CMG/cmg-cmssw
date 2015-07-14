#!/bin/bash

if [[ "$1" == "SingleLepAFS" ]]; then
    shift # shift register
    T="/afs/cern.ch/work/k/kirschen/public/PlotExampleSamples/V3";
    FT="/afs/cern.ch/work/k/kirschen/public/PlotExampleSamples/PHYS14_V3_FriendsRefinedIds"
    J=4;
elif [[ "$HOSTNAME" == *"lxplus"* ]] ; then
    T="/afs/cern.ch/work/k/kirschen/public/PlotExampleSamples/V3";
    FT="/afs/cern.ch/work/a/alobanov/public/SUSY/CMG/CMGtuples/FriendTrees/phys14_v3_btagCSVv2"
    J=4;
elif [[ "$1" == "DESYV3" ]] ; then
    shift # shift register
    T="/nfs/dust/cms/group/susy-desy/Run2/MC/CMGtuples/Phys14_v3/ForCMGplot";
    FT="/nfs/dust/cms/group/susy-desy/Run2/MC/CMGtuples/Phys14_v3/Phys14_V3_Friend_CSVbtag"
    J=8;
elif [[ "$HOSTNAME" == *"naf"* ]] ; then
    T="/nfs/dust/cms/group/susy-desy/Run2/MC/CMGtuples/Phys14_v3/ForCMGplot";
    FT="/nfs/dust/cms/group/susy-desy/Run2/MC/CMGtuples/Phys14_v3/Phys14_V3_Friend_CSVbtag"
    J=8;
elif [[ "$1" == "QCDLp" ]] ; then
    shift # shift register
    T="/nfs/dust/cms/group/susy-desy/Run2/MC/CMGtuples/Phys14_v3/ForCMGplot";
    FT="/nfs/dust/cms/user/lobanov/SUSY/Run2/RA4/MC/CMGtuples/FriendTrees/phys14_v3_Lp"
    J=8;
else
    echo "Didn't specify location!"
    echo "Usage: ./make_cards.sh location analysis "
    exit 0
fi

LUMI=3.0
OUTDIR="susy_cards_1l_4fb_test"
OPTIONS=" -P $T -j $J -l $LUMI -f --s2v --tree treeProducerSusySingleLepton --od $OUTDIR --asimov "

# Get current plotter dir
#PLOTDIR="$CMSSW_BASE/src/CMGTools/TTHAnalysis/python/plotter/"
PLOTDIR=$(pwd -P)
PLOTDIR=${PLOTDIR/plotter/plotterX}
PLOTDIR=$(echo $PLOTDIR |  cut -d 'X' -f 1 )

# Append FriendTree dir
OPTIONS=" $OPTIONS -F sf/t $FT/evVarFriend_{cname}.root "

function makeCard_1l {
    local EXPR=$1; local BINS=$2; local SYSTS=$3; local OUT=$4; local GO=$5

    # choose CF and MCA files
    CutFlowCard="1l_CardsFullCutFlow.txt"
    McaFile="mca-Phys14_1l_Cards.txt"

    if [[ "$CutFlowCard" == "2l_CardsFullCutFlow.txt" ]]; then
	FTDiLep="/afs/cern.ch/user/k/kirschen/private/SUSY_SingleLepton/temp_DELETEIMMEDIATELY/CMSSW_7_2_3/src/CMGTools/TTHAnalysis/macros/Phys14_Friend_DiLepImpr_NoAddMET_DiLepFlag"
	echo "$GO"
	GO=${GO/$FT/$FTDiLep}
	echo "$GO"
    fi


    EXTRALABEL=""

    # b-jet cuts
    case $nB in
	0B)  GO="${GO} -R 1nB 0nB nBJetMedium30==0 "; EXTRALABEL="${EXTRALABEL} nB=0\n" ;;
	1B)  GO="${GO} -R 1nB 1nB nBJetMedium30==1 "; EXTRALABEL="${EXTRALABEL} nB=1\n" ;;
	2B)  GO="${GO} -R 1nB 2nB nBJetMedium30==2 "; EXTRALABEL="${EXTRALABEL} nB=2\n" ;;
	2Btop)  GO="${GO} -R 1nB 2nB nBJetMedium30==2&&Topness>5 "; EXTRALABEL="${EXTRALABEL} nB=2(+topness)\n" ;;
	1p)  GO="${GO} -R 1nB 1nBp nBJetMedium30>=1 "; EXTRALABEL="${EXTRALABEL} nB#geq1\n" ;;
	2p)  GO="${GO} -R 1nB 2nBp nBJetMedium30>=2 "; EXTRALABEL="${EXTRALABEL} nB#geq2\n" ;;
	3p)  GO="${GO} -R 1nB 3nBp nBJetMedium30>=3 "; EXTRALABEL="${EXTRALABEL} nB#geq3\n" ;;
    esac;

    # ST categories
    case $ST in
	STInc)  GO="${GO} -R st200 st200Inf ST>200 "; EXTRALABEL="${EXTRALABEL} ST>200 GeV\n" ;;
	ST0)  GO="${GO} -R st200 st200250 ST>200&&ST<250 "; EXTRALABEL="${EXTRALABEL} 200<ST<250 GeV\n" ;;
	ST1)  GO="${GO} -R st200 st250350 ST>250&&ST<350 "; EXTRALABEL="${EXTRALABEL} 250<ST<350 GeV\n" ;;
	ST2)  GO="${GO} -R st200 st350450 ST>350&&ST<450 "; EXTRALABEL="${EXTRALABEL} 350<ST<450 GeV\n" ;;
	ST3)  GO="${GO} -R st200 st450550 ST>450&&ST<550 "; EXTRALABEL="${EXTRALABEL} 450<ST<550 GeV\n" ;;
	ST4)  GO="${GO} -R st200 st550700 ST>550&&ST<700 "; EXTRALABEL="${EXTRALABEL} 550<ST<700 GeV\n" ;;
	ST5)  GO="${GO} -R st200 st700Inf ST>700 "; EXTRALABEL="${EXTRALABEL} ST>700 GeV\n" ;;
	STDynDP0)  GO="${GO} -R st200 st200250 ST>200&&ST<250 -R dp1 dp10 fabs(DeltaPhiLepW)>1.0 "; EXTRALABEL="${EXTRALABEL} 200<ST<250 GeV\n #Delta#phi>1.0\n" ;;
	STDynDP1)  GO="${GO} -R st200 st250350 ST>250&&ST<350 -R dp1 dp10 fabs(DeltaPhiLepW)>1.0 "; EXTRALABEL="${EXTRALABEL} 250<ST<350 GeV\n #Delta#phi>1.0\n" ;;
	STDynDP2)  GO="${GO} -R st200 st350450 ST>350&&ST<450 -R dp1 dp075 fabs(DeltaPhiLepW)>0.75 "; EXTRALABEL="${EXTRALABEL} 350<ST<450 GeV\n #Delta#phi>0.75\n" ;;
	STDynDP3)  GO="${GO} -R st200 st450550 ST>450&&ST<550 -R dp1 dp075 fabs(DeltaPhiLepW)>0.75 "; EXTRALABEL="${EXTRALABEL} 450<ST<550 GeV\n #Delta#phi>0.75\n" ;;
	STDynDP4)  GO="${GO} -R st200 st550700 ST>550&&ST<700 -R dp1 dp05 fabs(DeltaPhiLepW)>0.5 "; EXTRALABEL="${EXTRALABEL} 550<ST<700 GeV\n #Delta#phi>0.5\n" ;;
	STDynDP5)  GO="${GO} -R st200 st700Inf ST>700 -R dp1 dp05 fabs(DeltaPhiLepW)>0.5 "; EXTRALABEL="${EXTRALABEL} ST>700 GeV\n #Delta#phi>0.5\n" ;;
    esac;



    if [[ "$CutFlowCard" == "2l_CardsFullCutFlow.txt" ]]; then
	case $nJ in
	    23j)  GO="${GO} -R geq6j 23j nCentralJet30>=1&&nCentralJet30<=2"; EXTRALABEL="${EXTRALABEL} 2-3 jets\n"  ;;
	    45j)  GO="${GO} -R geq6j 45j nCentralJet30>=3&&nCentralJet30<=4"; EXTRALABEL="${EXTRALABEL} 4-5 jets\n"  ;;
	    68j)  GO="${GO} -R geq6j 67j nCentralJet30>=5&&nCentralJet30<=7"; EXTRALABEL="${EXTRALABEL} 6-8 jets\n"  ;;
	    6Infj)  GO="${GO} -R geq6j geq6j nCentralJet30>=5"; EXTRALABEL="${EXTRALABEL} #geq6 jets\n"  ;;
	    9Infj)  GO="${GO} -R geq6j geq8j nCentralJet30>=8"; EXTRALABEL="${EXTRALABEL} #geq9 jets\n"  ;;
	esac;
    else
	case $nJ in
	    23j)  GO="${GO} -R geq6j 23j nCentralJet30>=2&&nCentralJet30<=3"; EXTRALABEL="${EXTRALABEL} 2-3 jets\n"  ;;
	    45j)  GO="${GO} -R geq6j 45j nCentralJet30>=4&&nCentralJet30<=5"; EXTRALABEL="${EXTRALABEL} 4-5 jets\n"  ;;
	    68j)  GO="${GO} -R geq6j 67j nCentralJet30>=6&&nCentralJet30<=8"; EXTRALABEL="${EXTRALABEL} 6-8 jets\n"  ;;
	    6Infj)  GO="${GO} -R geq6j geq6j nCentralJet30>=6"; EXTRALABEL="${EXTRALABEL} #geq6 jets\n"  ;;
	    9Infj)  GO="${GO} -R geq6j geq8j nCentralJet30>=9"; EXTRALABEL="${EXTRALABEL} #geq9 jets\n"  ;;
	    68TTj)  GO="${GO} -R geq6j 68TTj nCentralJet30+2*nHighPtTopTagPlusTau23>=6&&nCentralJet30+2*nHighPtTopTagPlusTau23<9"; EXTRALABEL="${EXTRALABEL} 6-8 TT enh. jets\n"  ;;
	    9InfTTj)  GO="${GO} -R geq6j 9InfTTj nCentralJet30+2*nHighPtTopTagPlusTau23>=9"; EXTRALABEL="${EXTRALABEL} #geq9 TT enh. jets\n"  ;;
	esac;
    fi

    # jet multiplicities

    # HT and "R&D" categories
    case $HT in
	HTInc) GO="${GO} -R ht500 ht500Inf HT>500"; EXTRALABEL="${EXTRALABEL} HT>500 GeV\n"  ;;
	HT0) GO="${GO} -R ht500 ht500750 HT>500&&HT<=750"; EXTRALABEL="${EXTRALABEL} 500<HT<750 GeV\n"  ;;
	HT1) GO="${GO} -R ht500 ht7501250 HT>750&&HT<=1250"; EXTRALABEL="${EXTRALABEL} 750<HT<1250 GeV\n"  ;;
	HT2) GO="${GO} -R ht500 ht1250Inf HT>1250"; EXTRALABEL="${EXTRALABEL} HT>1250 GeV\n"  ;;
    esac;

    # "R&D" categories
    case $RD in
	Def) GO="${GO} "; EXTRALABEL="${EXTRALABEL} baseline\n"  ;;
	DPhi10) GO="${GO} "; EXTRALABEL="${EXTRALABEL} #Delta#phi>1.0\n"  ;;
	DPhi075) GO="${GO} -R dp1 dp075 fabs(DeltaPhiLepW)>0.75 "; EXTRALABEL="${EXTRALABEL} #Delta#phi>0.75\n"  ;;
	DPhi05) GO="${GO} -R dp1 dp05 fabs(DeltaPhiLepW)>0.5 "; EXTRALABEL="${EXTRALABEL} #Delta#phi>0.5\n"  ;;
	DPhi00) GO="${GO} -R dp1 dp00 fabs(DeltaPhiLepW)>0.0 "; EXTRALABEL="${EXTRALABEL} #Delta#phi>0.0\n"  ;;
	Stop) GO="${GO} -R dp1 dp05 fabs(DeltaPhiLepW)>0.5 -A dp1 stopness (TopVarsMETovTopMin[0]-0.5)/0.5+(TopVarsMtopMin[0]-175)/175>1.25"; EXTRALABEL="${EXTRALABEL} #Delta#phi>0.5+STop\n"  ;;
	Top) GO="${GO} -R dp1 dp05 fabs(DeltaPhiLepW)>0.5 -A dp1 stopness (TopVarsMETovTopMin[0]-0.5)/0.5+(TopVarsMtopMin[0]-175)/175>1.25&&Topness>5"; EXTRALABEL="${EXTRALABEL} #Delta#phi>0.5+STop+Top\n"  ;;
	LowLepPtStop) GO="${GO} -R 1tl 1tllowpt nTightLeps==1&&LepGood1_pt<=25  -R dp1 dp00 fabs(DeltaPhiLepW)>0.0 -A dp1 stopness (TopVarsMETovTopMin[0]-0.5)/0.5+(TopVarsMtopMin[0]-175)/175>1.25"; EXTRALABEL="${EXTRALABEL} soft lept.\n#Delta#phi>0.0+STop\n"  ;;
	LowLepPtTop) GO="${GO} -R 1tl 1tllowpt nTightLeps==1&&LepGood1_pt<=25  -R dp1 dp00 fabs(DeltaPhiLepW)>0.0 -A dp1 stopness (TopVarsMETovTopMin[0]-0.5)/0.5+(TopVarsMtopMin[0]-175)/175>1.25&&Topness>5"; EXTRALABEL="${EXTRALABEL} soft lept.\n#Delta#phi>0.0+STop+Top\n"  ;;
	HTLowLepPtDPhi) GO="${GO} -R 1tl 1tllowpt nTightLeps==1&&LepGood1_pt<=25"; EXTRALABEL="${EXTRALABEL} soft lept.\n#Delta#phi>1.0\n"  ;;
	TTYes) GO="${GO} -A dp1 TopTag nHighPtTopTagPlusTau23>=1"; EXTRALABEL="${EXTRALABEL} TopTag\n"  ;;
	TTNo) GO="${GO} -A dp1 TopTag nHighPtTopTagPlusTau23==0"; EXTRALABEL="${EXTRALABEL} no TopTag\n"  ;;
    esac;

    echo $EXTRALABEL

    if [[ "$PRETEND" == "1" ]]; then
	echo "making datacard $OUT from makeShapeCardsSusy.py $McaFile $CutFlowCard \"$EXPR\" \"$BINS\" $SYSTS $GO --dummyYieldsForZeroBkg;"
    else
	echo "making datacard $OUT from makeShapeCardsSusy.py $McaFile $CutFlowCard \"$EXPR\" \"$BINS\" $SYSTS $GO --dummyYieldsForZeroBkg;"
	python $PLOTDIR/makeShapeCardsSusy.py $PLOTDIR/$McaFile $PLOTDIR/susy-1lep/$CutFlowCard "$EXPR" "$BINS" $SYSTS -o $OUT $GO --dummyYieldsForZeroBkg;
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

	grep -q "observation 0.0$" $C && continue # skip empty bins
#       grep -q "observation 0.01$" $C && grep -q "DummyContent" $C && continue #skip bins with only DummyContent as well
	AllC=$((AllC+1))
	CMD="${CMD} $(basename $C .card.txt)=$C ";
    done
    if [[ "$CMD" == "" ]]; then
	echo "Not any card found in $*" 1>&2 ;
    else
	echo "combineCards.py $CMD" >&2
	combineCards.py $CMD
    fi
    if [[ "$DummyC" != "0" ]]; then
	echo "In total $DummyC out of $AllC are empty, but taken into account by adding DummyContent." >&2
    fi
    #echo "In total $DummyC out of $AllC are empty, but taken into account by adding DummyContent." >&2
}

if [[ "$1" == "--pretend" ]]; then
    PRETEND=1; shift;
    echo "# Pretending to run"
fi;

if [[ "$1" == "1l-makeCards" ]]; then

    SYSTS="syst/susyDummy.txt"
    CnC_expr="1" #not used as of now
    CnC_bins="[0.5,1.5]"

    STValue="$2"
    echo "$STValue"

    echo "Making individual datacards"
#old approach
#    for ST in "$STValue"; do for nJ in 45j 68j 6Infj 9Infj; do for nB in 1p 1B 2B 3p; do for HT in HT0 HT1; do for RD in DPhi00 DPhi05 DPhi075 DPhi10; do
#    for ST in ST0 ST1 ST2 ST3 ST4 STInc; do for nJ in 45j 68j 6Infj 9Infj; do for nB in 1p 1B 2B 3p; do for HT in HT0 HT1; do for RD in DPhi00 DPhi05 DPhi075 DPhi10; do

#for baseline analysis:
    for ST in "$STValue"; do for nJ in 45j 68j 6Infj 9Infj; do for nB in 1p 1B 2B 3p; do for HT in HT0 HT1 HT2; do for RD in Def; do
#    for ST in STDynDP0 STDynDP1 STDynDP2 STDynDP3 STDynDP4 STDynDP5; do for nJ in 45j 68j 6Infj 9Infj; do for nB in 1p 1B 2B 3p; do for HT in HT0 HT1 HT2; do for RD in Def; do

#for dphi>0.5, single topness, topness, and soft lepton analysis:
#    for ST in "$STValue"; do for nJ in 45j 68j 6Infj 9Infj; do for nB in 1p 1B 2B 2Btop 3p; do for HT in HT0 HT1 HT2; do for RD in DPhi05 Stop LowLepPtStop; do
#    for ST in ST0 ST1 ST2 ST3 ST4 ST5; do for nJ in 45j 68j 6Infj 9Infj; do for nB in 1p 1B 2B 2Btop 3p; do for HT in HT0 HT1 HT2; do for RD in DPhi05 Stop LowLepPtStop; do
	echo " --- CnC2015X_${nB}_${ST}_${nJ}_${HT}_${RD} ---"
	makeCard_1l $CnC_expr $CnC_bins $SYSTS CnC2015X_${nB}_${ST}_${nJ}_${HT}_${RD} "$OPTIONS";
		done; done; done; done; done;
#    done;
    exit 0

fi

if [[ "$1" == "1l-combine" ]]; then

    if [[ ! $CMSSW_VERSION == *"CMSSW_7_1_"* ]] ;then
	echo "You don't have the correct CMSSW environment!"
	echo "Found: $CMSSW_VERSION, need CMSSW_7_1_X"
	exit 0
    fi

    echo "Making combined datacards"

    if [[ "$PRETEND" == "1" ]]; then
	echo "Pretending to do cards"
	exit 0
    fi

    for D in $OUTDIR/T[0-9]*; do
	echo "Making combined datacards2"
	test -f $D/CnC2015X_2B_ST1_68j_HT1_DPhi05.card.txt || continue
	echo "Making combined datacards3"
	(cd $D && echo "    $D";
	    for nB in 1B 2B 3p; do
		combineCardsSmart CnC2015X_${nB}_{STDynDP0,STDynDP1,STDynDP2,STDynDP3,STDynDP4,STDynDP5}_6Infj_{HT0,HT1,HT2}_Def.card.txt >  CnC2015X_${nB}_standardnJ.card.txt
		combineCardsSmart CnC2015X_${nB}_{STDynDP0,STDynDP1,STDynDP2,STDynDP3,STDynDP4,STDynDP5}_{68j,9Infj}_{HT0,HT1,HT2}_Def.card.txt >  CnC2015X_${nB}_finenJ.card.txt

	    done
	    combineCardsSmart CnC2015X_{1B,2B,3p}_standardnJ.card.txt >  CnC2015X_standardnJ.card.txt # standard nJet-binning; HT-binning
	    combineCardsSmart CnC2015X_{1B,2B,3p}_finenJ.card.txt >  CnC2015X_finenJ.card.txt #fine nJet-binning; HT-binning

	    combineCardsSmart CnC2015X_{1B,2B,3p}_{ST0,ST1,ST2,ST3,ST4,ST5}_{68j,9Infj}_{HT0,HT1,HT2}_DPhi05.card.txt >  CnC2015X_Dphi05.card.txt #fine nJet-binning; HT-binning; DPhi>0.5
	    combineCardsSmart CnC2015X_{1B,2B,3p}_{ST0,ST1,ST2,ST3,ST4,ST5}_{68j,9Infj}_{HT0,HT1,HT2}_Stop.card.txt >  CnC2015X_STop.card.txt #fine nJet-binning; HT-binning; single topness only
	    combineCardsSmart CnC2015X_{1B,2Btop,3p}_{ST0,ST1,ST2,ST3,ST4,ST5}_{68j,9Infj}_{HT0,HT1,HT2}_Stop.card.txt >  CnC2015X_STop_Top2B.card.txt #fine nJet-binning; HT-binning; use topness for 2B
	    combineCardsSmart CnC2015X_{1B,2Btop,3p}_{ST0,ST1,ST2,ST3,ST4,ST5}_{68j,9Infj}_{HT0,HT1,HT2}_LowLepPtStop.card.txt >  CnC2015X_LowLepPtStop.card.txt #fine nJet-binning; HT-binning; use topness for 2B



	    #merge everything to be able to parse easily the datacards
	    combineCardsSmart CnC2015X_{1p,1B,2B,3p}_{ST0,ST1,ST2,ST3,ST4,ST5}_{68j,6Infj,9Infj}_{HT0,HT1,HT2}_DPhi00.card.txt > CnC2015X_ForYieldComparisons_DPhi00.card.txt
	    combineCardsSmart CnC2015X_{1p,1B,2B,3p}_{ST0,ST1,ST2,ST3,ST4,ST5}_{68j,6Infj,9Infj}_{HT0,HT1,HT2}_DPhi05.card.txt > CnC2015X_ForYieldComparisons_DPhi05.card.txt
	    combineCardsSmart CnC2015X_{1p,1B,2B,3p}_{ST0,ST1,ST2,ST3,ST4,ST5}_{68j,6Infj,9Infj}_{HT0,HT1,HT2}_DPhi075.card.txt > CnC2015X_ForYieldComparisons_DPhi075.card.txt
	    combineCardsSmart CnC2015X_{1p,1B,2B,3p}_{ST0,ST1,ST2,ST3,ST4,ST5}_{68j,6Infj,9Infj}_{HT0,HT1,HT2}_DPhi10.card.txt > CnC2015X_ForYieldComparisons_DPhi10.card.txt
	    combineCardsSmart CnC2015X_{1p,1B,2B,3p}_{STDynDP0,STDynDP1,STDynDP2,STDynDP3,STDynDP4,STDynDP5}_{68j,6Infj,9Infj}_{HT0,HT1,HT2}_Def.card.txt > CnC2015X_ForYieldComparisons_DynamicDPhi.card.txt

	    combineCardsSmart CnC2015X_{1p,1B,2B,3p}_{ST0,ST1,ST2,ST3,ST4,ST5}_{68j,6Infj,9Infj}_{HT0,HT1,HT2}_Stop.card.txt > CnC2015X_ForYieldComparisons_Stop.card.txt
	    combineCardsSmart CnC2015X_{1p,1B,2B,3p}_{ST0,ST1,ST2,ST3,ST4,ST5}_{68j,6Infj,9Infj}_{HT0,HT1,HT2}_Top.card.txt > CnC2015X_ForYieldComparisons_Top.card.txt

	)
    done
fi

echo "Done at $(date)";
