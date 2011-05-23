import FWCore.ParameterSet.Config as cms

patEventContentCMG = cms.untracked.vstring(
    'drop *_selectedPatMuonsAK7_*_*',
    'drop *_selectedPatElectronsAK7_*_*',
    'drop *_selectedPatTausAK7_*_*',
    'drop CaloTowers_*_*_*',
    'drop patMETs_*_*_*',
    'keep patMETs_patMETsAK5_*_*',
    'drop patPFParticles_*_*_*',
    'keep recoVertexs_*_*_*',
    'keep recoBeamSpot_*_*_*',
    'keep L1GlobalTriggerObjectMapRecord_*_*_*',
    'keep *_TriggerResults_*_*',
    'keep *_hltTriggerSummaryAOD_*_*'
    )
