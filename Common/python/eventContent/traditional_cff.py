import FWCore.ParameterSet.Config as cms

traditionalJets = cms.untracked.vstring(
    'keep cmgBaseJets_*Calo*JetSel_*_*',
    # 'keep cmgAbstractPhysicsObjects_*Calo*Jet_*_*'
    # here add all PF jet objects, even the base ones. 
    )

traditionalMET = cms.untracked.vstring(
    # 'keep  patMETs_patMETs_*_*'
    'keep  *_cmgCaloMET_*_*'                                 
    )

# to be added to the output module if needed
traditionalMHT = cms.untracked.vstring(
    'keep  *_cmgMHTCaloJet*_*_*'
    )


traditionalVBF = cms.untracked.vstring(
    'keep *_vbfEventJetJetCalo_*_*'                                 
    )

traditional = traditionalJets + traditionalMET + traditionalVBF


traditionalBase = cms.untracked.vstring(
    'keep *_cmgCaloBaseJetSel_*_*'
    # here add the other generic PF objects
    )


