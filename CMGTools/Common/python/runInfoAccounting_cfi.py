import FWCore.ParameterSet.Config as cms

runInfoAccounting = cms.EDAnalyzer(
    "RunInfoAccountingAnalyzer",
    name = cms.untracked.string("RunInfoAccounting")
)
