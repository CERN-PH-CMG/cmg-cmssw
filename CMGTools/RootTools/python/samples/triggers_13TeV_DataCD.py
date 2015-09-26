################## 
## Based on /frozen/2015/25ns14e33/v4.2/HLT/V1 /frozen/2015/25ns14e33/v4.3/HLT/V3

triggers_mumu_iso    = [ "HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_v*", "HLT_Mu17_TrkIsoVVL_TkMu8_TrkIsoVVL_DZ_v*" ]
triggers_mumu_noniso = [ "HLT_Mu30_TkMu11_v*" ]
triggers_mumu_ss = [ "HLT_Mu17_Mu8_SameSign_v*" ]
triggers_mumu = triggers_mumu_iso

triggers_ee = [ "HLT_Ele17_Ele12_CaloIdL_TrackIdL_IsoVL_DZ_v*", "HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL_DZ_v*" ]
triggers_mue   = [ "HLT_Mu23_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL_v*", 
                   "HLT_Mu8_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL_v*" ]

triggers_mumu_ht =  [ "HLT_DoubleMu8_Mass8_PFHT300_v*" ]
triggers_ee_ht =  [ "HLT_DoubleEle8_CaloIdM_TrackIdM_Mass8_PFHT300_v*" ]
triggers_mue_ht = [ "HLT_Mu8_Ele8_CaloIdM_TrackIdM_Mass8_PFHT300_v*" ]

triggers_3e = [ "HLT_Ele16_Ele12_Ele8_CaloIdL_TrackIdL_v*" ]
triggers_3mu = [ "HLT_TripleMu_12_10_5_v*" ]
triggers_3mu_alt = [ "HLT_TrkMu15_DoubleTrkMu5NoFiltersNoVtx_v*" ]
triggers_2mu1e = [ "HLT_DiMu9_Ele9_CaloIdL_TrackIdL_v*" ]
triggers_2e1mu = [ "HLT_Mu8_DiEle12_CaloIdL_TrackIdL_v*" ]

triggers_1mu_iso_r  = [ 'HLT_IsoMu24_eta2p1_v*', 'HLT_IsoTkMu24_eta2p1_v*'  ]
triggers_1mu_iso_w  = [ 'HLT_IsoMu27_v*', 'HLT_IsoTkMu27_v*'  ]
triggers_1mu_noniso = [ 'HLT_Mu45_eta2p1_v*', 'HLT_Mu50_v*' ]


triggers_1mu_iso      = triggers_1mu_iso_r + triggers_1mu_iso_w

# note: here the WP75 is th name in MC, WPLoose and WPTight should be in data
triggers_1e      = [ "HLT_Ele32_eta2p1_WP75_Gsf_v*", "HLT_Ele32_eta2p1_WPLoose_Gsf_v*", "HLT_Ele32_eta2p1_WPTight_Gsf_v*" ]
triggers_1e_noniso      = [ "HLT_Ele105_CaloIdVT_GsfTrkIdT_v*"]

# Lepton fake rate triggers (prescaled)
triggers_FR_1mu_iso = [ "HLT_Mu%d_TrkIsoVVL_v*" % pt for pt in (8,17,24,34) ]
triggers_FR_1mu_noiso = [ "HLT_Mu%d_v*" % pt for pt in (8,17,24,34) ]
triggers_FR_1e_noiso = [ "HLT_Ele%d_CaloIdM_TrackIdM_PFJet30_v*" % pt for pt in (8,12,18,23,33) ]
triggers_FR_1e_iso   = [ "HLT_Ele%d_CaloIdL_TrackIdL_IsoVL_PFJet30_v*" % pt for pt in (12,18,23,33) ] #note: no 8


### Mike ---> for the VV analysis 
triggers_dijet_fat=["HLT_PFHT650_WideJetMJJ900DEtaJJ1p5_v*","HLT_PFHT650_WideJetMJJ950DEtaJJ1p5_v*"]
### ----> for the MT2 analysis

triggers_HT900 = ["HLT_PFHT900_v*"]
triggers_HT800 = ["HLT_PFHT800_v*"]
triggers_MET170_Noise = ["HLT_PFMET170_NoiseCleaned_v*"]
triggers_MET170_HBHE = ["HLT_PFMET170_HBHECleaned_v*"]
triggers_MET170_JetId = ["HLT_PFMET170_JetIdCleaned_v*"]
triggers_MET170 = ["HLT_PFMET170_v*"]
triggers_HTMET100 = ["HLT_PFHT350_PFMET100_JetIdCleaned_v*"]
triggers_ht350 = ["HLT_PFHT350_v*"] # prescaled
triggers_ht475 = ["HLT_PFHT475_v*"] # prescaled
triggers_ht600 = ["HLT_PFHT600_v*"] # prescaled

triggers_dijet = ["HLT_DiPFJetAve40_v*", "HLT_DiPFJetAve60_v*"] #HLT_DiPFJetAve40_v* :Prescaled

triggers_dijet55met110 = [ "DiCentralPFJet55_PFMET110_JetIdCleaned_v*" ]

triggers_photon75ps = ["HLT_Photon75_v*"] #prescaled
triggers_photon90ps = ["HLT_Photon90_v*"] #prescaled
triggers_photon120ps = ["HLT_Photon120_v*"] # prescaled
triggers_photon125ps = ["HLT_Photon125_v*"]
triggers_photon30 = ["HLT_Photon30_R9Id90_HE10_IsoM_v*"]
triggers_photon50 = ["HLT_Photon50_R9Id90_HE10_IsoM_v*"]
triggers_photon75 = ["HLT_Photon75_R9Id90_HE10_IsoM_v*"]
triggers_photon90 = ["HLT_Photon90_R9Id90_HE10_IsoM_v*"]
triggers_photon120ps = ["HLT_Photon120_v*"]
triggers_photon120 = ["HLT_Photon120_R9Id90_HE10_IsoM_v*"] #prescaled in 5e33
triggers_photon120 = ["HLT_Photon120_R9Id90_HE10_Iso40_EBOnly_VBF_v*"] 
triggers_photon150 = ["HLT_Photon150_v*"]
triggers_photon175 = ["HLT_Photon175_v*"]
triggers_photon165_HE10 = ["HLT_Photon165_HE10_v*"]


triggers_MT2_mumu = ["HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_v*", "HLT_Mu17_TrkIsoVVL_TkMu8_TrkIsoVVL_DZ_v*"]

triggers_MT2_ee = ["HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL_DZ_v*","HLT_Ele17_Ele12_CaloIdL_TrackIdL_IsoVL_DZ_v*"]

triggers_MT2_mue = triggers_mue + ["HLT_Mu17_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL_v*", "HLT_Mu8_TrkIsoVVL_Ele17_CaloIdL_TrackIdL_IsoVL_v*"]

triggers_MT2_mu = ["HLT_IsoMu17_eta2p1_v*","HLT_IsoMu20_eta2p1_v*", "HLT_IsoMu20_v*", "HLT_IsoTkMu20_v*"]
triggers_MT2_e = ["HLT_Ele23_WPLoose_Gsf_v*", "HLT_Ele27_eta2p1_WPLoose_Gsf_v*", "HLT_Ele32_eta2p1_WPLoose_Gsf_v*"]


triggers_Jet80MET90 = ["HLT_MonoCentralPFJet80_PFMETNoMu90_JetIdCleaned_PFMHTNoMu90_IDTight_v*"]
triggers_Jet80MET120 = ["HLT_MonoCentralPFJet80_PFMETNoMu120_JetIdCleaned_PFMHTNoMu120_IDTight_v*"]
triggers_MET120Mu5 = ["HLT_PFMET120_JetIdCleaned_Mu5_v*"]

### ----> for the edgeZ analysis. 
### we want them separately for detailed trigger efficiency studies
triggers_mu17mu8      = ['HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_v*']
triggers_mu17mu8_dz   = ['HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_v*']
triggers_mu17tkmu8_dz = ['HLT_Mu17_TrkIsoVVL_TkMu8_TrkIsoVVL_DZ_v*']
triggers_mu17el12     = ['HLT_Mu17_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL_v*']
triggers_el17el12_dz  = ['HLT_Ele17_Ele12_CaloIdL_TrackIdL_IsoVL_DZ_v*']
triggers_el23el12_dz  = ['HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL_DZ_v*']
triggers_mu8el17      = ['HLT_Mu8_TrkIsoVVL_Ele17_CaloIdL_TrackIdL_IsoVL_v*']
triggers_mu8el23      = ['HLT_Mu8_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL_v*']
triggers_pfht200      = ['HLT_PFHT200_v*']
triggers_pfht250      = ['HLT_PFHT250_v*']
triggers_pfht300      = ['HLT_PFHT300_v*']
triggers_pfht350      = ['HLT_PFHT350_v*']
triggers_pfht400      = ['HLT_PFHT400_v*']
triggers_pfht475      = ['HLT_PFHT475_v*']
triggers_pfht600      = ['HLT_PFHT600_v*']
triggers_pfht650      = ['HLT_PFHT650_v*']
triggers_pfht800      = ['HLT_PFHT800_v*']
triggers_pfht900      = ['HLT_PFHT900_v*']
triggers_at57         = ['HLT_PFHT200_DiPFJetAve90_PFAlphaT0p57_v*'] # prescaled in 1e34
triggers_at63         = ['HLT_PFHT200_DiPFJetAve90_PFAlphaT0p63_v*']
triggers_at55         = ['HLT_PFHT250_DiPFJetAve90_PFAlphaT0p55_v*']  # prescaled in 1e34
triggers_at58         = ['HLT_PFHT250_DiPFJetAve90_PFAlphaT0p58_v*']
triggers_at53         = ['HLT_PFHT300_DiPFJetAve90_PFAlphaT0p53_v*'] # prescaled in 1e34
triggers_at54         = ['HLT_PFHT300_DiPFJetAve90_PFAlphaT0p54_v*']
triggers_at52         = ['HLT_PFHT350_DiPFJetAve90_PFAlphaT0p52_v*']
triggers_HT350at53         = ['HLT_PFHT350_DiPFJetAve90_PFAlphaT0p53_v*']
triggers_at51         = ['HLT_PFHT400_DiPFJetAve90_PFAlphaT0p51_v*']
triggers_HT400at52         = ['HLT_PFHT400_DiPFJetAve90_PFAlphaT0p52_v*']
triggers_htmet        = ['HLT_PFHT350_PFMET100_JetIdCleaned_v*']
triggers_htjet        = ['HLT_PFHT550_4Jet_v*', 'HLT_PFHT650_4Jet_v*', 'HLT_PFHT750_4JetPt50_v*'] # HLT_PFHT550_4Jet_v*', 'HLT_PFHT650_4Jet_v*' prescaled
