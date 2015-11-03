################################
#  use mcEfficiencies.py to make plots of efficiencies and fake rates
################################

T="/afs/cern.ch/work/b/botta/TREES_74X_100815_MiniIso_25nsMC"
T="/tmp/gpetrucc/TREES_74X_100815_MiniIso_25nsMC"
T="/data1/p/peruzzi/TREES_74X_230915_MiniIso_7_4_12"
BCORE=" --s2v --tree treeProducerSusyMultilepton susy-multilepton/lepton-mca.txt susy-multilepton/lepton-perlep.txt -P ${T}  "
#BCORE="${BCORE}  --Fs {P}/0_leptonMVA_v1_NW --Fs {P}/0_leptonMVA_v2pt_NW "
#BCORE="${BCORE}  --Fs {P}/37_gioLepMVATests_v1 "
PBASE="plots/74X/multiIso/effs"
BG=" "
if [[ "$1" == "-b" ]]; then BG=" & "; shift; else BG=" -j 2 "; fi

what=$1;
case $what in
signal)
    for W in L M T VT; do 
    EFFS="mcEfficiencies.py -p TT_true,T1tttt_true ${BCORE} susy-multilepton/lepton-sels.txt -X iso ttH-multilepton/lepton-xvars.txt --groupBy process,cut --legend=BR";
    echo "( python $EFFS --yrange 0 1.  -o $PBASE/$what/mu_eta_00_12_${W}.root  -R pt25 eta 'abs(LepGood_eta)<1.2' --sP pt_fine --sP 'multiIso.*_${W}'  ${BG} )"
    echo "( python $EFFS --yrange 0 1.  -o $PBASE/$what/mu_eta_12_24_${W}.root  -R pt25 eta 'abs(LepGood_eta)>1.2' --sP pt_fine --sP 'multiIso.*_${W}'  ${BG} )"
    EFFS="mcEfficiencies.py -p TT_true,T1tttt_true ${BCORE} susy-multilepton/lepton-sels.txt -X muon -X iso ttH-multilepton/lepton-xvars.txt --groupBy process,cut --legend=BR";
    echo "( python $EFFS --yrange 0 1.  -o $PBASE/$what/el_eta_00_15_${W}.root  -R pt25 eta 'abs(LepGood_eta)<1.479' --sP pt_fine --sP 'multiIso.*_${W}'  ${BG} )"
    echo "( python $EFFS --yrange 0 1.  -o $PBASE/$what/el_eta_15_25_${W}.root  -R pt25 eta 'abs(LepGood_eta)>1.479' --sP pt_fine --sP 'multiIso.*_${W}'  ${BG} )"
    done
    ;;
fakes)
    EFFS="mcEfficiencies.py ${BCORE} susy-multilepton/lepton-sels.txt susy-multilepton/lepton-xvars.txt --groupBy process --legend=TL";
    echo "( python $EFFS --yrange 0 0.05 -p TT_bjets -o $PBASE/$what/mu_b_eta_00_12_T.root  -R pt20 eta 'abs(LepGood_eta)<1.2' --sP pt_fine --sP m.*_T  ${BG} )"
    echo "( python $EFFS --yrange 0 0.1  -p TT_bjets -o $PBASE/$what/mu_b_eta_00_12_M.root  -R pt20 eta 'abs(LepGood_eta)<1.2' --sP pt_fine --sP m.*_M  ${BG} )"
    echo "( python $EFFS --yrange 0 0.05 -p TT_bjets -o $PBASE/$what/mu_b_eta_12_24_T.root  -R pt20 eta 'abs(LepGood_eta)>1.2' --sP pt_fine --sP m.*_T  ${BG} )"
    echo "( python $EFFS --yrange 0 0.1  -p TT_bjets -o $PBASE/$what/mu_b_eta_12_24_M.root  -R pt20 eta 'abs(LepGood_eta)>1.2' --sP pt_fine --sP m.*_M  ${BG} )"
    echo "( python $EFFS --yrange 0 0.05 -p TT_bjets -o $PBASE/$what/mu_b_eta_00_12_T_zoom.root  -R pt20 eta 'abs(LepGood_eta)<1.2' --sP pt_zfine --sP m.*_T  ${BG} )"
    echo "( python $EFFS --yrange 0 0.1  -p TT_bjets -o $PBASE/$what/mu_b_eta_00_12_M_zoom.root  -R pt20 eta 'abs(LepGood_eta)<1.2' --sP pt_zfine --sP m.*_M  ${BG} )"
    echo "( python $EFFS --yrange 0 0.05 -p TT_bjets -o $PBASE/$what/mu_b_eta_12_24_T_zoom.root  -R pt20 eta 'abs(LepGood_eta)>1.2' --sP pt_zfine --sP m.*_T  ${BG} )"
    echo "( python $EFFS --yrange 0 0.1  -p TT_bjets -o $PBASE/$what/mu_b_eta_12_24_M_zoom.root  -R pt20 eta 'abs(LepGood_eta)>1.2' --sP pt_zfine --sP m.*_M  ${BG} )"
    echo "( python $EFFS --yrange 0 0.5  -p TT_ljets -o $PBASE/$what/mu_b_eta_00_12_T.root  -R pt20 eta 'abs(LepGood_eta)<1.2' --sP pt_coarse --sP m.*_T  ${BG} )"
    echo "( python $EFFS --yrange 0 0.5  -p TT_ljets -o $PBASE/$what/mu_b_eta_00_12_M.root  -R pt20 eta 'abs(LepGood_eta)<1.2' --sP pt_coarse --sP m.*_M  ${BG} )"
    echo "( python $EFFS --yrange 0 0.5  -p TT_ljets -o $PBASE/$what/mu_b_eta_12_24_T.root  -R pt20 eta 'abs(LepGood_eta)>1.2' --sP pt_coarse --sP m.*_T  ${BG} )"
    echo "( python $EFFS --yrange 0 0.5  -p TT_ljets -o $PBASE/$what/mu_b_eta_12_24_M.root  -R pt20 eta 'abs(LepGood_eta)>1.2' --sP pt_coarse --sP m.*_M  ${BG} )"
    ;;
fakes2)
    EFFS="mcEfficiencies.py ${BCORE} susy-multilepton/lepton-sels.txt susy-multilepton/lepton-xvars.txt --groupBy process --legend=TR";
    echo "( python $EFFS --yrange 0 0.07 -p TT_red -o $PBASE/$what/mu_eta_00_12_T.root  -R pt20 eta 'abs(LepGood_eta)<1.2' --sP pt_zfine --sP 'mvaTTH_[TM],m.*V2_M'  ${BG} )"
    echo "( python $EFFS --yrange 0 0.07 -p TT_red -o $PBASE/$what/mu_eta_00_12_T.root  -R pt20 eta 'abs(LepGood_eta)<1.2' --sP pt_fine --sP 'mvaTTH_[TM],m.*V2_M'  ${BG} )"
    echo "( python $EFFS --yrange 0 0.14 -p TT_red -o $PBASE/$what/mu_eta_00_12_T_f.root  -R pt20 eta 'abs(LepGood_eta)<1.2' --sP pt_zfine --sP 'mvaTTH_[TM],m.*V2_M'  ${BG} )"
    echo "( python $EFFS --yrange 0 0.14 -p TT_red -o $PBASE/$what/mu_eta_00_12_T_f.root  -R pt20 eta 'abs(LepGood_eta)<1.2' --sP pt_fine --sP 'mvaTTH_[TM],m.*V2_M'  ${BG} )"
    ;;
esac

