import FWCore.ParameterSet.Config as cms

jetCounting = cms.EDAnalyzer(
    "JetCountingHistogramsAnalyzer",
    inputCollection = cms.InputTag("cmgPFJet")
)
