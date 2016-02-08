#!/bin/bash


# RUN Z ANALYSIS ONLY
sed -i "s/runZanalysis =.*/runZanalysis = 0/g" configdir/*.py
sed -i "s/mergeSigEWKbkg =.*/mergeSigEWKbkg = 0/g" configdir/*.py
sed -i "s/runPrepareDataCardsFast =.*/runPrepareDataCardsFast = 0/g" configdir/*.py
sed -i "s/runClosureTestLikeLihoodRatio =.*/runClosureTestLikeLihoodRatio = 0/g" configdir/*.py
# sed -i "s/DataCards_templateFromFolder=.*/DataCards_templateFromFolder=\"${DataCards_templateFromFolder}\"/g" configdir/*.py
sed -i "s/mergeResults =.*/mergeResults = 1/g" configdir/*.py


# COMMENT UNWANTED SYSTEMATICS

# CENTRAL
python launch_analysis.py config

# SYST EFFICIENCIES
python launch_analysis.py config_eff_13
python launch_analysis.py config_eff_14
python launch_analysis.py config_eff_15
python launch_analysis.py config_eff_16

# SYST MUON +1 sigma
python launch_analysis.py config_muscale_minus1
python launch_analysis.py config_mustat_minus1

# SYST MUON +1 sigma
python launch_analysis.py config_muscale_plus1
python launch_analysis.py config_mustat_plus1

# SYST ALTERNATIVE RECOIL MODEL
python launch_analysis.py config_recoilmodel

# SYST RECOIL STAT +1 sigma
python launch_analysis.py config_recoilstat_p1_plus1
python launch_analysis.py config_recoilstat_p2_plus1
python launch_analysis.py config_recoilstat_p3_plus1
python launch_analysis.py config_recoilstat_p4_plus1
python launch_analysis.py config_recoilstat_p5_plus1
python launch_analysis.py config_recoilstat_p6_plus1
python launch_analysis.py config_recoilstat_p7_plus1
python launch_analysis.py config_recoilstat_p8_plus1
python launch_analysis.py config_recoilstat_p9_plus1
python launch_analysis.py config_recoilstat_p10_plus1
python launch_analysis.py config_recoilstat_p11_plus1
python launch_analysis.py config_recoilstat_p12_plus1

# SYST RECOIL STAT -1 sigma
python launch_analysis.py config_recoilstat_p1_minus1
python launch_analysis.py config_recoilstat_p2_minus1
python launch_analysis.py config_recoilstat_p3_minus1
python launch_analysis.py config_recoilstat_p4_minus1
python launch_analysis.py config_recoilstat_p5_minus1
python launch_analysis.py config_recoilstat_p7_minus1
python launch_analysis.py config_recoilstat_p8_minus1
python launch_analysis.py config_recoilstat_p6_minus1
python launch_analysis.py config_recoilstat_p9_minus1
python launch_analysis.py config_recoilstat_p10_minus1
python launch_analysis.py config_recoilstat_p11_minus1
python launch_analysis.py config_recoilstat_p12_minus1

