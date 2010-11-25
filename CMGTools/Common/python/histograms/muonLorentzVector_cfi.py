import FWCore.ParameterSet.Config as cms

muonLorentzVector = cms.EDAnalyzer(
    "LorentzHistogramsAnalyzer",
    inputCollection = cms.InputTag("cmgMuon")
    )
