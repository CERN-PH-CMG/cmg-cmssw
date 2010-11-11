import FWCore.ParameterSet.Config as cms

baseJetLorentzVector = cms.EDAnalyzer(
    "LorentzHistogramsAnalyzer",
    inputCollection = cms.InputTag("cmgBaseJet"),
    name = cms.string("BaseJet")
)
