import FWCore.ParameterSet.Config as cms

pfCandLorentzVector = cms.EDAnalyzer(
    "LorentzHistogramsAnalyzer",
    inputCollection = cms.InputTag("cmgPFCand")
    )
