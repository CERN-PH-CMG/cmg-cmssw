import FWCore.ParameterSet.Config as cms

cutSummaryPFJet = cms.EDAnalyzer(
    "CutSummaryAnalyzer",
    inputCollection = cms.InputTag("cmgPFJetSel")
)
