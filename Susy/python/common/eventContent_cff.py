import FWCore.ParameterSet.Config as cms

leptons = [
    'keep *_susyMuon_*_*',
    'keep *_susyElectron_*_*'
    ]

trigger = [
    'keep *_TriggerResults_*_*',
    ]

jetId = [
    'drop cmgPFJets_cmgPFJet*Failed_*_*',
    'keep cmgPFJets_cmgPFJetTightJetIdFailed_*_*'
    ]

gen = [
    'keep LHEEventProduct_*_*_*'   
    ]

reco = [
    'drop recoBeamHaloSummary_BeamHaloSummary_*_*',
    'drop recoVertexs_offlinePrimaryVertices_*_*'  
]

pat = [
    'drop recoPFCandidates_greedyMuonsTagging_*_*',
    'drop recoPFCandidates_inconsistentMuonsTagging_*_*',
    'drop patElectrons_selectedPatElectronsAK5LC_*_*'
]

cmg = [
    'drop cmgBaseJets_cmgPFBaseJetSel_*_*',
    'drop cmgElectroncmgElectroncmgDiObjects_cmgDiElectronSel_*_*',
    'drop cmgMuoncmgMuoncmgDiObjects_cmgDiMuonSel_*_*',
    'drop cmgMETSignificance_PFMETSignificanceAK5LC_*_*'
]

eventContent = trigger + gen + reco + pat + cmg + jetId
