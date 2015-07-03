################## Triggers (FIXME: update to the PHYS14 Trigger Menu)

triggers_ref_mu = ["HLT_IsoMu27_v*"]
triggers_ref_el = ["HLT_Ele27_eta2p1_WP75_Gsf_v*","HLT_Ele32_eta2p1_WP75_Gsf_v*"]

### ----> for the 1L dPhi analysis

## muons
triggers_mu_ht600 = ["HLT_Mu15_IsoVVVL_PFHT600_v*"]
triggers_mu_ht400_met70 = ["HLT_Mu15_IsoVVVL_PFHT400_PFMET70_v*"]
triggers_mu_met120 = ["HLT_PFMET120_NoiseCleaned_Mu5_v*"]

## electrons
triggers_el_ht600 = ["HLT_Ele15_IsoVVVL_PFHT600_v*"]
triggers_el_ht400_met70 = ["HLT_Ele15_IsoVVVL_PFHT400_PFMET70_v*"]
triggers_el_ht200 = ["HLT_Ele27_eta2p1_WP85_Gsf_HT200_v*"]

## hadronic
triggers_HT350 = ["HLT_PFHT350_v*"]
triggers_MET170 = ["HLT_PFMET170_NoiseCleaned_v*"]

### ----> for the MT2 analysis

triggers_HT900 = ["HLT_PFHT900_v*"]
#triggers_MET170 = ["HLT_PFMET170_NoiseCleaned_v*"]
triggers_HTMET = ["HLT_PFHT350_PFMET120_NoiseCleaned_v*"]

#### Combined paths
