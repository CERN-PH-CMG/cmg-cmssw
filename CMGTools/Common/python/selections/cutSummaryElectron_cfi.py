import FWCore.ParameterSet.Config as cms

cutSummaryElectron = cms.EDAnalyzer(
    "CutSummaryAnalyzer",
    inputCollection = cms.InputTag("cmgElectronSel")
)
