##################
## Triggers for HLT_MC_SPRING15 and Run II
## Based on HLT_MC_SPRING15 and /frozen/2015/25ns14e33/v2.1/HLT/V1 and /frozen/2015/50ns_5e33/v2.1/HLT/V5

### ----> for the 1L dPhi analysis

## single lepton
triggers_1mu = ["HLT_IsoMu27_v*"]
triggers_1el = ["HLT_Ele32_eta2p1_WP75_Gsf_v*"]

### non-iso single lepton
trigger_1mu_noiso_r = ['HLT_Mu45_eta2p1_v*']
trigger_1mu_noiso_w = ['HLT_Mu50_v*']
trigger_1el_noiso = ['HLT_Ele105_CaloIdVT_GsfTrkIdT_v*']

## muons
triggers_mu_ht600 = ["HLT_Mu15_IsoVVVL_PFHT600_v*"]
triggers_mu_ht400_met70 = ["HLT_Mu15_IsoVVVL_PFHT400_PFMET70_v*"]
triggers_mu_met120 = ["HLT_PFMET120_NoiseCleaned_Mu5_v*"]
triggers_mu_ht400_btag = ["HLT_Mu15_IsoVVVL_BTagCSV07_PFHT400_v*"]

## electrons
triggers_el_ht600 = ["HLT_Ele15_IsoVVVL_PFHT600_v*"]
triggers_el_ht400_met70 = ["HLT_Ele15_IsoVVVL_PFHT400_PFMET70_v*"]
triggers_el_ht200 = ["HLT_Ele27_eta2p1_WP85_Gsf_HT200_v*"]
triggers_el_ht400_btag = ["HLT_Ele15_IsoVVVL_BTagtop8CSV07_PFHT400_v*"]

## hadronic
triggers_HT350 = ["HLT_PFHT350_v*"] # prescaled!
triggers_HT600 = ["HLT_PFHT600_v*"] # prescaled!
triggers_HT900 = ["HLT_PFHT900_v*"]
triggers_MET170 = ["HLT_PFMET170_NoiseCleaned_v*"]
triggers_HTMET = ["HLT_PFHT350_PFMET120_NoiseCleaned_v*"]

#### Combined paths

triggers_muhad = triggers_mu_ht600 + triggers_mu_ht400_met70 + triggers_mu_met120 + triggers_mu_ht400_btag
triggers_elhad = triggers_el_ht600 + triggers_el_ht400_met70 + triggers_el_ht200  + triggers_el_ht400_btag
triggers_had = triggers_HT900 + triggers_MET170 + triggers_HTMET
