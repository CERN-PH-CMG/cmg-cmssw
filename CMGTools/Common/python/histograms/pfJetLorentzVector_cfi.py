import FWCore.ParameterSet.Config as cms

pfJetLorentzVector = cms.EDAnalyzer(
    "LorentzHistogramsAnalyzer",
    inputCollection = cms.InputTag("cmgPFJet"),
    name = cms.string("PFJet")
)
