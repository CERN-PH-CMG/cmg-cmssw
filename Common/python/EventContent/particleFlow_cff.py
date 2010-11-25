import FWCore.ParameterSet.Config as cms

particleFlowJets = cms.untracked.vstring(
    'keep cmgPFJets_*_*_*',
    'keep cmgBaseJets_*PF*Jet*_*_*',
    'keep cmgAbstractPhysicsObjects_*PF*Jet*_*_*'
    # here add all PF jet objects, even the base ones. 
    )

particleFlowMET = cms.untracked.vstring(
    'keep  *_cmgMETPFCandidates*_*_*',
    'keep  patMETs_patMETsPFlow_*_*',
    # here add the other PF-based MET objects, even the base ones. 
    )

# to be added to the output module if needed
particleFlowMHT = cms.untracked.vstring(
    'keep  *_cmgMHTPFJets*_*_*'
    )

particleFlowMuons = cms.untracked.vstring(
    'keep *_cmgMuon_*_*',
    'keep *_cmgDiMuon_*_*'
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


