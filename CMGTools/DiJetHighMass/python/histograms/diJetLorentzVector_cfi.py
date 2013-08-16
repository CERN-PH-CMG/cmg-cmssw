import FWCore.ParameterSet.Config as cms

cmgDiJetLorentzVector = cms.EDAnalyzer(
    "DiJetHistogramsAnalyser",
    inputCollection = cms.InputTag("cmgBaseJet")
)
