import FWCore.ParameterSet.Config as cms

particleFlowJets = cms.untracked.vstring(
    'keep *_cmgPFJet_*_*',
    'keep *_cmgPFBaseJet_*_*',
    'keep *_cmgDiJet_*_*'
    # here add all PF jet objects, even the base ones. 
    )

particleFlowMET = cms.untracked.vstring(
    'keep  *_cmgMHTPFJets_*_*',
    'keep  *_cmgMETPFCandidates_*_*',
    'keep  patMETs_patMETsPFlow_*_*',
    # here add the other PF-based MET / MHT objects, even the base ones. 
    )

particleFlowMuons = cms.untracked.vstring(
    'keep *_cmgMuon_*_*'
    )

particleFlowElectrons = cms.untracked.vstring(
    'keep *_cmgElectron_*_*'
    )

particleFlow = particleFlowJets + particleFlowMET + particleFlowMuons + particleFlowElectrons


particleFlowBase = cms.untracked.vstring(
    'keep *_cmgPFBaseJet_*_*',
    'keep *_cmgPFCand_*_*'
    # here add the other generic PF objects
    )


