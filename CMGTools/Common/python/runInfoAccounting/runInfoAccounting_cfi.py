import FWCore.ParameterSet.Config as cms

runInfoAccounting = cms.EDAnalyzer(
    "RunInfoAccountingAnalyzer",
    name = cms.untracked.string("RunInfoAccounting"),
    genFilterInfoSrc = cms.InputTag("genFilterEfficiencyProducer::SIM"),
    filterInfoSrc = cms.InputTag("preSelFilterEfficiencyProducer"),
    runOnMC = cms.bool(True)
)
