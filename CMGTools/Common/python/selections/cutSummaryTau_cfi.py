import FWCore.ParameterSet.Config as cms


cutSummaryTau = cms.EDAnalyzer(
    "CutSummaryAnalyzer",
    inputCollection = cms.InputTag("cmgTauSel")
)


