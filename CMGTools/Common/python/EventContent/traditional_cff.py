import FWCore.ParameterSet.Config as cms

traditionalJets = cms.untracked.vstring(
    'keep cmgBaseJets_*Calo*Jet_*_*',
    'keep cmgAbstractPhysicsObjects_*Calo*Jet_*_*'
    # here add all PF jet objects, even the base ones. 
    )

traditionalMET = cms.untracked.vstring(
    'keep  patMETs_patMETs_*_*'
    # here add the other PF-based MET objects, even the base ones. 
    )

# to be added to the output module if needed
traditionalMHT = cms.untracked.vstring(
    'keep  *_cmgMHTCaloJets*_*_*'
    )

traditional = traditionalJets + traditionalMET 


traditionalBase = cms.untracked.vstring(
    'keep *_cmgCaloBaseJet_*_*'
    # here add the other generic PF objects
    )


