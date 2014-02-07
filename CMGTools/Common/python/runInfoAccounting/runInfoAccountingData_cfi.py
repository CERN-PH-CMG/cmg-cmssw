import FWCore.ParameterSet.Config as cms

runInfoAccountingData = cms.EDAnalyzer(
    "RunInfoAccountingAnalyzer",
    name = cms.untracked.string("RunInfoAccounting"),
    genFilterInfoSrc = cms.InputTag("genFilterEfficiencyProducer::SIM"),
    filterInfoSrc = cms.InputTag("preSelFilterEfficiencyProducer"),
    runOnMC = cms.bool(False)
)
