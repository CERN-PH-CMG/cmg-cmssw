import FWCore.ParameterSet.Config as cms

pfJetHistograms = cms.EDAnalyzer(
    "PFJetHistogramsAnalyzer",
    inputCollection = cms.InputTag("cmgPFJet"),
    name = cms.string("PFJet")
)
