#!/bin/bash

if [[ "$1" == "SingleLepAFS" ]]; then
    shift # shift register
    T="/afs/cern.ch/work/k/kirschen/public/PlotExampleSamples/V3";
    FT="/afs/cern.ch/work/a/alobanov/public/SUSY/CMG/CMGtuples/FriendTrees/phys14_v3_btagCSVv2"
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
else
    echo "Didn't specify location!"
    echo "Usage: ./make_cards.sh location analysis "
    exit 0
fi

LUMI=$2
OUTDIR="RCScards_lumi$2"
OPTIONS=" -P $T -j $J -l $LUMI -f --s2v --tree treeProducerSusySingleLepton --od $OUTDIR  --asimov"

# Get current plotter dir
PLOTDIR=$(pwd -P)
PLOTDIR=${PLOTDIR/plotter/plotterX}
PLOTDIR=$(echo $PLOTDIR |  cut -d 'X' -f 1 )

# Append FriendTree dir
OPTIONS=" $OPTIONS -F sf/t $FT/evVarFriend_{cname}.root "

function makeCard_1l {
    local EXPR=$1; local BINS=$2; local SYSTS=$3; local OUT=$4; local GO=$5

    CutFlowCard="1l_CardsFullCutFlow.txt"

    # b-jet cuts
    case $nB in
        0B)  GO="${GO} -R 1nB 0nB nBJetMedium30==0 " ;;
        1B)  GO="${GO} -R 1nB 1nB nBJetMedium30==1 " ;;
	123B)  GO="${GO} -R 1nB 1nB nBJetMedium30>=1 " ;;
        2B)  GO="${GO} -R 1nB 2nB nBJetMedium30==2 " ;;
	23B)GO="${GO} -R 1nB 2nB nBJetMedium30>=2 " ;;
        3B)  GO="${GO} -R 1nB 3nBp nBJetMedium30>=3 " ;;
    esac;
    #define signal and control regions according to variable dphi cut
    case $ST in
        ST0SR)  GO="${GO} -R st200 st200250 ST>200&&ST<250 " ;;
        ST1SR)  GO="${GO} -R st200 st250350 ST>250&&ST<350 -R dp1 dp1 fabs(DeltaPhiLepW)>1" ;;
        ST2SR)  GO="${GO} -R st200 st350450 ST>350&&ST<450 -R dp1 dp75 fabs(DeltaPhiLepW)>0.75" ;;
        ST3SR)  GO="${GO} -R st200 st450600 ST>450&&ST<600 -R dp1 dp75 fabs(DeltaPhiLepW)>0.75" ;;
        ST4SR)  GO="${GO} -R st200 st600Inf ST>600 -R dp1 dp05 fabs(DeltaPhiLepW)>0.5" ;;
        ST34SR)  GO="${GO} -R st200 st450600 ST>450 -R dp1 dp75 fabs(DeltaPhiLepW)>0.75" ;;

        ST1CR)  GO="${GO} -R st200 st250350 ST>250&&ST<350 -R dp1 dp1CR fabs(DeltaPhiLepW)<1" ;;
        ST2CR)  GO="${GO} -R st200 st350450 ST>350&&ST<450 -R dp1 dp75CR fabs(DeltaPhiLepW)<0.75" ;;
        ST3CR)  GO="${GO} -R st200 st450600 ST>450&&ST<600 -R dp1 dp75CR fabs(DeltaPhiLepW)<0.75" ;;
        ST4CR)  GO="${GO} -R st200 st600Inf ST>600 -R dp1 dp05CR fabs(DeltaPhiLepW)<0.5" ;;
        ST34CR)  GO="${GO} -R st200 st450600 ST>450 -R dp1 dp75 fabs(DeltaPhiLepW)<0.75" ;;


    esac;

    # jet multiplicities
    case $nJ in
        45j)  GO="${GO} -R geq6j 45j nCentralJet30>=4&&nCentralJet30<=5"  ;;
        68j)  GO="${GO} -R geq6j 67j nCentralJet30>=6&&nCentralJet30<=8"  ;;
        6Infj)  GO="${GO} -R geq6j geq6j nCentralJet30>=6"  ;;
        9Infj)  GO="${GO} -R geq6j geq8j nCentralJet30>=9"  ;;
    esac;

    # HT categories and combined bins
    case $HT in
        HT0) GO="${GO} -R ht500 ht500750 HT>500&&HT<750"  ;;
        HT1) GO="${GO} -R ht500 ht7501250 HT>750&&HT<1250"  ;;
        HT2) GO="${GO} -R ht500 ht1250Inf HT>=1250"  ;;
	
	HT01) GO="${GO} -R ht500 ht5001250 HT>500&&HT<1250" ;;
	HT12) GO="${GO} -R ht500 ht750 HT>750" ;;
	HT012) GO="${GO} -R ht500 ht500 HT>500" ;;
    esac;

    if [[ "$PRETEND" == "1" ]]; then
        echo "making datacard $OUT from makeShapeCardsSusy.py mca-Phys14_1l_RCS.txt $CutFlowCard \"$EXPR\" \"$BINS\" $SYSTS $GO --dummyYieldsForZeroBkg --ignoreEmptySignal;"
    else
        echo "making datacard $OUT from makeShapeCardsSusy.py mca-Phys14_1l_RCS.txt $CutFlowCard \"$EXPR\" \"$BINS\" $SYSTS $GO --dummyYieldsForZeroBkg --ignoreEmptySignal;"
        python $PLOTDIR/makeShapeCardsSusy.py mca-Phys14_1l_RCS.txt $PLOTDIR/susy-1lep/$CutFlowCard "$EXPR" "$BINS" $SYSTS -o $OUT $GO --dummyYieldsForZeroBkg --ignoreEmptySignal;
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
#       echo "combineCards.py $CMD" >&2
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

    SYSTS="../limits/syst/susyDummy.txt"
    CnC_expr="1" #not used as of now
    CnC_bins="[0.5,1.5]"


    echo "Making individual datacards"
#45 jets, CR cards
    for ST in ST1SR ST2SR ST3SR ST4SR ST1CR ST2CR ST3CR ST4CR; do for nJ in 45j; do for nB in 1B 2B 3B; do for HT in HT012; do 
        echo " --- CnC2015X_${nB}_${ST}_${nJ}_${HT} ---"
        makeCard_1l $CnC_expr $CnC_bins $SYSTS CnC2015X_${nB}_${ST}_${nJ}_${HT} "$OPTIONS";

                done; done; done; done

    for ST in ST1SR ST2SR ST3SR ST4SR ST1CR ST2CR ST3CR ST4CR; do for nJ in 45j; do for nB in 123B; do for HT in HT012; do 
        echo " --- CnC2015X_${nB}_${ST}K_${nJ}_${HT} ---"
        makeCard_1l $CnC_expr $CnC_bins $SYSTS CnC2015X_${nB}_${ST}K_${nJ}_${HT} "$OPTIONS";

                done; done; done; done
#make kappa factor regions 
    for ST in ST1SR ST1CR; do for nJ in 68j ; do for nB in 123B; do for HT in HT0 HT12; do 
        echo " --- CnC2015X_${nB}_${ST}K_${nJ}_${HT} ---"
        makeCard_1l $CnC_expr $CnC_bins $SYSTS CnC2015X_${nB}_${ST}K_${nJ}_${HT} "$OPTIONS";
                done; done; done; done
    for ST in ST2SR ST2CR; do for nJ in 68j ; do for nB in 123B; do for HT in HT0 HT1 HT2; do 
        echo " --- CnC2015X_${nB}_${ST}K_${nJ}_${HT} ---"
        makeCard_1l $CnC_expr $CnC_bins $SYSTS CnC2015X_${nB}_${ST}K_${nJ}_${HT} "$OPTIONS";
                done; done; done; done
    for ST in ST3SR ST3CR ST4SR ST4CR; do for nJ in 68j ; do for nB in 123B; do for HT in HT01 HT2; do 
        echo " --- CnC2015X_${nB}_${ST}K_${nJ}_${HT} ---"
        makeCard_1l $CnC_expr $CnC_bins $SYSTS CnC2015X_${nB}_${ST}K_${nJ}_${HT} "$OPTIONS";
                done; done; done; done

    for ST in ST1SR ST1CR ST2SR ST2CR; do for nJ in 9Infj ; do for nB in 123B; do for HT in HT01 HT2; do 
        echo " --- CnC2015X_${nB}_${ST}K_${nJ}_${HT} ---"
        makeCard_1l $CnC_expr $CnC_bins $SYSTS CnC2015X_${nB}_${ST}K_${nJ}_${HT} "$OPTIONS";
                done; done; done; done

    for ST in ST34SR ST34CR; do for nJ in 9Infj; do for nB in 123B; do for HT in HT012; do 
        echo " --- CnC2015X_${nB}_${ST}K_${nJ}_${HT} ---"
        makeCard_1l $CnC_expr $CnC_bins $SYSTS CnC2015X_${nB}_${ST}K_${nJ}_${HT} "$OPTIONS";
                done; done; done; done
#last one for 9 jets already done with all b combined

#6jets
    for ST in ST1SR ST1CR; do for nJ in 68j; do for nB in 1B 2B 3B; do for HT in HT0 HT12; do 
        echo " --- CnC2015X_${nB}_${ST}_${nJ}_${HT} ---"
        makeCard_1l $CnC_expr $CnC_bins $SYSTS CnC2015X_${nB}_${ST}_${nJ}_${HT} "$OPTIONS";
                done; done; done; done

    for ST in ST2SR ST2CR; do for nJ in 68j; do for nB in 1B; do for HT in HT0 HT1 HT2; do 
        echo " --- CnC2015X_${nB}_${ST}_${nJ}_${HT} ---"
        makeCard_1l $CnC_expr $CnC_bins $SYSTS CnC2015X_${nB}_${ST}_${nJ}_${HT} "$OPTIONS";
                done; done; done; done

    for ST in ST2SR ST2CR; do for nJ in 68j; do for nB in 2B 3B; do for HT in HT0 HT1; do 
        echo " --- CnC2015X_${nB}_${ST}_${nJ}_${HT} ---"
        makeCard_1l $CnC_expr $CnC_bins $SYSTS CnC2015X_${nB}_${ST}_${nJ}_${HT} "$OPTIONS";
                done; done; done; done

    for ST in ST2SR ST2CR; do for nJ in 68j; do for nB in 23B; do for HT in HT2; do 
        echo " --- CnC2015X_${nB}_${ST}_${nJ}_${HT} ---"
        makeCard_1l $CnC_expr $CnC_bins $SYSTS CnC2015X_${nB}_${ST}_${nJ}_${HT} "$OPTIONS";
                done; done; done; done

    for ST in ST3SR ST3CR; do for nJ in 68j; do for nB in 1B 2B 3B; do for HT in HT01 HT2; do 
        echo " --- CnC2015X_${nB}_${ST}_${nJ}_${HT} ---"
        makeCard_1l $CnC_expr $CnC_bins $SYSTS CnC2015X_${nB}_${ST}_${nJ}_${HT} "$OPTIONS";
                done; done; done; done

    for ST in ST4SR ST4CR; do for nJ in 68j; do for nB in 1B 23B; do for HT in HT01 HT2; do 
        echo " --- CnC2015X_${nB}_${ST}_${nJ}_${HT} ---"
        makeCard_1l $CnC_expr $CnC_bins $SYSTS CnC2015X_${nB}_${ST}_${nJ}_${HT} "$OPTIONS";
                done; done; done; done
#9jets
    for ST in ST1SR ST2SR ST1CR ST2CR; do for nJ in 9Infj; do for nB in 1B 2B; do for HT in HT01 HT2; do 
        echo " --- CnC2015X_${nB}_${ST}_${nJ}_${HT} ---"
        makeCard_1l $CnC_expr $CnC_bins $SYSTS CnC2015X_${nB}_${ST}_${nJ}_${HT} "$OPTIONS";
                done; done; done; done

    for ST in ST1SR ST2SR ST1CR ST2CR; do for nJ in 9Infj; do for nB in 3B; do for HT in HT012; do 
        echo " --- CnC2015X_${nB}_${ST}_${nJ}_${HT} ---"
        makeCard_1l $CnC_expr $CnC_bins $SYSTS CnC2015X_${nB}_${ST}_${nJ}_${HT} "$OPTIONS";
                done; done; done; done

    for ST in ST34SR ST34CR; do for nJ in 9Infj; do for nB in 123B; do for HT in HT012; do 
        echo " --- CnC2015X_${nB}_${ST}_${nJ}_${HT} ---"
        makeCard_1l $CnC_expr $CnC_bins $SYSTS CnC2015X_${nB}_${ST}_${nJ}_${HT} "$OPTIONS";
                done; done; done; done

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
#        test -f $D/CnC2015X_2B_ST0_68j_HT0_0T.card.txt || continue
        (cd $D && echo "    $D";

            combineCardsSmart CnC2015X_{1B,2B,3B,123B,23B}_{ST1SR,ST2SR,ST3SR,ST4SR,ST34SR}_{68j,9Infj}_{HT0,HT1,HT2,HT01,HT12,HT012}.card.txt >  CnC2015X_all3fbBins.card.txt

        )
    done
fi

echo "Done at $(date)";
