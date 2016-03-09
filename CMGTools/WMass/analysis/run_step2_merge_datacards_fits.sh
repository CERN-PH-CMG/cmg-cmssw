#!/bin/bash

# DEFINE FOLDER PREFIX
outfolder_prefix="PREFIX"
#WlikeCharge=1 # 1, -1
WlikeCharge="${1}"

# DEFINE TEMPLATE FOLDER
#DataCards_templateFromFolder=""

if [ $WlikeCharge -eq 1 ]; then
    DataCards_templateFromFolder="${outfolder_prefix}_muPos_tkmet_ewk0_polariz1_KalmanCorrParam_RecoilCorr2_EffHeinerSFCorr_PtSFCorr0_PileupSFCorr"
    # DataCards_templateFromFolder="${outfolder_prefix}_muPos_tkmet_ewk0_polariz1_KalmanCorrParam_DataLike1_RecoilCorr2_EffHeinerSFCorr_PtSFCorr0_PileupSFCorr"
fi
if [ $WlikeCharge -eq -1 ]; then
    DataCards_templateFromFolder="${outfolder_prefix}_muNeg_tkmet_ewk0_polariz1_KalmanCorrParam_RecoilCorr2_EffHeinerSFCorr_PtSFCorr0_PileupSFCorr"
    # DataCards_templateFromFolder="${outfolder_prefix}_muNeg_tkmet_ewk0_polariz1_KalmanCorrParam_DataLike1_RecoilCorr2_EffHeinerSFCorr_PtSFCorr0_PileupSFCorr"
fi

sed_and_run()
{
    # RUN MERGE CHUNCKS, PREPARE DATACARDS, RUN FITS
    sed -i "s/useBatch =.*/useBatch = 1/g;\
            s/outfolder_prefix =.*/outfolder_prefix = \"${outfolder_prefix}\"/g;\
            s/runZanalysis =.*/runZanalysis = 0/g;\
            s/mergeSigEWKbkg =.*/mergeSigEWKbkg = 1/g;\
            s/runPrepareDataCardsFast =.*/runPrepareDataCardsFast = 1 # TEMPLATES ARE IN THE TEMPLATE FOLDER, PSEUDO-DATA IN THE CURRENT FOLDER/g;\
            s/DataCards_templateFromFolder=.*/DataCards_templateFromFolder=\"${DataCards_templateFromFolder}\"  # evaluate systematics wrt folder (or leave it empty) -- full template folder/g;\
            s/runClosureTestLikeLihoodRatio =.*/runClosureTestLikeLihoodRatio = 1  # 1: also executes merge if not using batch jobs/g;\
            s/mergeResults =.*/mergeResults = 0/g;\
            s/WlikeCharge =.*/WlikeCharge = ${WlikeCharge}  # Charge of the Wlike (+1,-1)/g" configdir/${1}.py
    python launch_analysis.py ${1}
}

# SET ENVIRONMENT VARIABLES BEFORE TO LAUNCH. CAVEAT: IT ASSUMES BASH
eval `scramv1 runtime -sh`
source /afs/cern.ch/sw/lcg/app/releases/ROOT/5.34.24/x86_64-slc6-gcc47-opt/root/bin/thisroot.sh

# COMMENT UNWANTED SYSTEMATICS

# CENTRAL
sed_and_run config_nominal
sed_and_run config_unblinding
sed_and_run config_Data_nominal

# SYST PDF NNPDF2.3
sed_and_run config_pdf_nnpdf23

# SYST EFFICIENCIES
sed_and_run config_eff_13
sed_and_run config_eff_14
sed_and_run config_eff_15
sed_and_run config_eff_16

# SYST MUON -1 sigma
sed_and_run config_muscale_minus1
sed_and_run config_mustat_minus1
sed_and_run config_mustatData_minus1

# SYST MUON +1 sigma
sed_and_run config_muscale_plus1
sed_and_run config_mustat_plus1
sed_and_run config_mustatData_plus1

# SYST ALTERNATIVE RECOIL MODEL
sed_and_run config_recoilmodel

# SYST RECOIL STAT +1 sigma
sed_and_run config_recoilstat_p1_plus1
sed_and_run config_recoilstat_p2_plus1
sed_and_run config_recoilstat_p3_plus1
sed_and_run config_recoilstat_p4_plus1
sed_and_run config_recoilstat_p5_plus1
sed_and_run config_recoilstat_p6_plus1
sed_and_run config_recoilstat_p7_plus1
sed_and_run config_recoilstat_p8_plus1
sed_and_run config_recoilstat_p9_plus1
sed_and_run config_recoilstat_p10_plus1
sed_and_run config_recoilstat_p11_plus1
sed_and_run config_recoilstat_p12_plus1

# SYST RECOIL STAT -1 sigma
sed_and_run config_recoilstat_p1_minus1
sed_and_run config_recoilstat_p2_minus1
sed_and_run config_recoilstat_p3_minus1
sed_and_run config_recoilstat_p4_minus1
sed_and_run config_recoilstat_p5_minus1
sed_and_run config_recoilstat_p7_minus1
sed_and_run config_recoilstat_p8_minus1
sed_and_run config_recoilstat_p6_minus1
sed_and_run config_recoilstat_p9_minus1
sed_and_run config_recoilstat_p10_minus1
sed_and_run config_recoilstat_p11_minus1
sed_and_run config_recoilstat_p12_minus1

# CLOSURE POW to MAD
sed_and_run config_powToMadKeys
sed_and_run config_powToMad3G
sed_and_run config_madNoCorr

sed_and_run config_powToMad_p1_plus1
sed_and_run config_powToMad_p2_plus1
sed_and_run config_powToMad_p3_plus1
sed_and_run config_powToMad_p4_plus1
sed_and_run config_powToMad_p5_plus1
sed_and_run config_powToMad_p6_plus1
sed_and_run config_powToMad_p7_plus1
sed_and_run config_powToMad_p8_plus1
sed_and_run config_powToMad_p9_plus1
sed_and_run config_powToMad_p10_plus1
sed_and_run config_powToMad_p11_plus1
sed_and_run config_powToMad_p12_plus1

