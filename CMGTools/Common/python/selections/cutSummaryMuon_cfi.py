import FWCore.ParameterSet.Config as cms

cutSummaryMuon = cms.EDAnalyzer(
    "CutSummaryAnalyzer",
    inputCollection = cms.InputTag("cmgMuonSel")
)
