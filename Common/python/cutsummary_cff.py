import FWCore.ParameterSet.Config as cms

cutsummary = cms.EDAnalyzer(
    "CutSummaryAnalyzer",
    inputCollection = cms.InputTag("cmgMuon")
)