import FWCore.ParameterSet.Config as cms

electronLorentzVector = cms.EDAnalyzer(
    "LorentzHistogramsAnalyzer",
    inputCollection = cms.InputTag("cmgElectron")
    )
