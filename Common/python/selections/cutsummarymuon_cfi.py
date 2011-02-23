import FWCore.ParameterSet.Config as cms

cutsummarymuon = cms.EDAnalyzer(
    "CutSummaryAnalyzer",
    inputCollection = cms.InputTag("cmgMuon")
)
