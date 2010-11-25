import FWCore.ParameterSet.Config as cms

pfJetLorentzVector = cms.EDAnalyzer(
    "LorentzHistogramsAnalyzer",
    inputCollection = cms.InputTag("cmgPFJetLead")
    )
