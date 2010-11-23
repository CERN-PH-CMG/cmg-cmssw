import FWCore.ParameterSet.Config as cms

traditionalJets = cms.untracked.vstring(
    'keep *_cmgCaloBaseJet_*_*'
    # here add all PF jet objects, even the base ones. 
    )

traditionalMET = cms.untracked.vstring(
    'keep  *_cmgMHTCaloJets*_*_*',
    'keep  patMETs_patMETs_*_*'
    # here add the other PF-based MET / MHT objects, even the base ones. 
    )



traditional = traditionalJets + traditionalMET 


traditionalBase = cms.untracked.vstring(
    'keep *_cmgCaloBaseJet_*_*'
    # here add the other generic PF objects
    )


