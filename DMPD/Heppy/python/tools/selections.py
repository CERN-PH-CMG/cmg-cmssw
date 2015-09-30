
selection = {
  "monoX" : "met_pt>200 && jet1_pt>110 && abs(jet1_dPhi_met)>2 && jet1_chf > 0.2 && jet1_nhf < 0.7 && jet1_phf < 0.7 && (nJets==1 || (nJets==2 && jet2_nhf < 0.7 && jet2_phf < 0.9 && abs(jet2_dPhi_jet1) < 2.5)) && nElectrons==0 && nTaus==0 && nPhotons==0",
  "monoB" : "met_pt>200 && jet1_pt>50 && jet1_chf>0.2 && jet1_nhf<0.7 && jet1_phf<0.7 && jet1_CSV>0.814 && ((nJets==1 && abs(jet1_dPhi_met)>2) || (nJets==2 && jet2_nhf<0.7 && jet2_phf<0.9 && abs(jet2_dPhi_jet1)<2.5 && jet2_CSV>0.814)) && nElectrons==0 && nTaus==0 && nPhotons==0", # && abs(H_dPhi_met)>2
  "singleMu" : "HLT_BIT_HLT_IsoMu24_eta2p1_v && lepton1_isMuon && lepton1_tightId && lepton1_pt>30 && abs(lepton1_eta)<2.1 && lepton1_relIso04<0.12",
  "doubleMu" : "HLT_BIT_HLT_IsoMu24_eta2p1_v && lepton1_isMuon && lepton1_tightId && lepton1_pt>20 && lepton1_relIso04<0.12 && lepton2_isMuon && lepton2_pt>15 && Z_mass>70 && Z_mass<110",
  "orMuoEle" : "HLT_BIT_HLT_IsoMu24_eta2p1_v && lepton1_isMuon!=lepton2_isMuon && lepton1_pt>30 && lepton2_pt>30 && lepton1_tightId && (lepton1_isElectron || lepton1_relIso04<0.12)",
  "singleEle" : "HLT_BIT_HLT_Ele105_CaloIdVT_GsfTrkIdT_v && lepton1_isElectron && lepton1_tightId && lepton1_pt>110 && abs(lepton1_eta)<2.1",
  "doubleEle" : "HLT_BIT_HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL_DZ_v && lepton1_isElectron && lepton1_tightId && lepton1_pt>30 && abs(lepton1_eta)<2.1 && lepton2_pt>20 && Z_mass>70 && Z_mass<110",
  "doubleLep" : "(HLT_BIT_HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL_DZ_v || HLT_BIT_HLT_IsoMu24_eta2p1_v) && lepton1_tightId && (lepton1_isElectron || lepton1_relIso04<0.12) && lepton1_pt>25 && lepton2_pt>20 && Z_mass>70 && Z_mass<110",
  # AZh (outdated)
  "triEle" : "HLT_BIT_HLT_Ele105_CaloIdVT_GsfTrkIdT_v",
  "preEle" : "isZtoEE && lepton1_pt>115 && lepton2_pt>20 && lepton1_miniIso<0.1 && lepton2_miniIso<0.1",
  "triMuo" : "HLT_BIT_HLT_Mu45_eta2p1_v",
  "preMuo" : "isZtoMM && (lepton1_highPtId || lepton2_highPtId) && lepton1_looseId && lepton2_looseId && lepton1_pt>50 && lepton1_miniIso<0.1 && lepton2_pt>20 && lepton2_miniIso<0.1",
  "preLep" : "(HLT_BIT_HLT_Ele105_CaloIdVT_GsfTrkIdT_v && isZtoEE && lepton1_pt>115 && lepton2_pt>20 && lepton1_miniIso<0.1 && lepton2_miniIso<0.1) || (HLT_BIT_HLT_Mu45_eta2p1_v && isZtoMM && (lepton1_highPtId || lepton2_highPtId) && lepton1_looseId && lepton2_looseId && lepton1_pt>50 && lepton1_miniIso<0.1 && lepton2_pt>20 && lepton2_miniIso<0.1)",
  # XZh
  "Zcut" : "Z_pt>200 && Z_mass>70 && Z_mass<110",
  "Hcut" : "fatjet1_pt>200 && (fatjet1_prunedMass>95 && fatjet1_prunedMass<130)",
  "Bcut" : "(fatjet1_dR>0.3 ? fatjet1_CSV1>0.605 && fatjet1_CSV2>0.605 : fatjet1_CSV1>0.605 || fatjet1_CSV2>0.605)",
  "XZhllbb" : "Z_pt>200 && Z_mass>70 && Z_mass<110 && fatjet1_pt>200 && (fatjet1_prunedMass>95 && fatjet1_prunedMass<130) && (fatjet1_dR>0.3 ? fatjet1_CSV1>0.605 && fatjet1_CSV2>0.605 : fatjet1_CSV1>0.605 || fatjet1_CSV2>0.605)",
  "XZheebb" : "isZtoEE && Z_pt>200 && Z_mass>70 && Z_mass<110 && fatjet1_pt>200 && (fatjet1_prunedMass>95 && fatjet1_prunedMass<130) && (fatjet1_dR>0.3 ? fatjet1_CSV1>0.605 && fatjet1_CSV2>0.605 : fatjet1_CSV1>0.605 || fatjet1_CSV2>0.605)",
  "XZhmmbb" : "isZtoMM && Z_pt>200 && Z_mass>70 && Z_mass<110 && fatjet1_pt>200 && (fatjet1_prunedMass>95 && fatjet1_prunedMass<130) && (fatjet1_dR>0.3 ? fatjet1_CSV1>0.605 && fatjet1_CSV2>0.605 : fatjet1_CSV1>0.605 || fatjet1_CSV2>0.605)",
  "XZhnnbb" : "HLT_BIT_HLT_PFMET170_NoiseCleaned_v && met_pt>200 && fatjet1_pt>200 && (fatjet1_prunedMass>95 && fatjet1_prunedMass<130)",
}
