import FWCore.ParameterSet.Config as cms

patEventContentCMG = cms.untracked.vstring(
    # all the commented lines are needed at CMG-tuple step if we do a simple cloning of the sequences. Need to see if we can gain time / disk space later by organizing the CMG sequences properly
    'keep *_selectedPatMuons*_*_*',
    'keep *_selectedPatElectrons*_*_*',
    'keep *_selectedPatTausAK5*_*_*',
    'drop *_selectedPatMuonsAK7_*_*',
    'drop *_selectedPatElectronsAK7_*_*',
    'drop *_selectedPatMuonsAK5NoPUSub_*_*',
    'drop *_selectedPatElectronsAK5NoPUSub_*_*',
    'drop CaloTowers_*_*_*',
    'drop patMETs_*_*_*',
    'keep patMETs_patMETsAK5_*_*',
    'drop patPFParticles_*_*_*',
    'keep recoVertexs_*_*_*',
    'keep recoPFCandidates_particleFlow_*_*',
    'keep recoGenParticles_*_*_*',
    'keep recoBeamSpot_*_*_*',
    'keep L1GlobalTriggerObjectMapRecord_*_*_*',
    'keep L1GlobalTriggerReadoutRecord_gtDigis_*_*',
    'keep *_TriggerResults_*_*',
    'keep *_hltTriggerSummaryAOD_*_*',
    # dropping AK7 taus
    'drop *_selectedPatTausAK7_*_*',
    'drop recoVertexs_pfTauPileUpVerticesAK7_*_*',
    'drop recoBaseTagInfosOwned_selectedPatJetsAK7_tagInfos_*',
    # this guys is already present in the AK5 sequence
    'drop recoVertexs_goodOfflinePrimaryVerticesLC_*_*',
    'keep *_PFMETSignificance*_*_*',
    'keep double_kt6PFJets*_rho_*',
    )

