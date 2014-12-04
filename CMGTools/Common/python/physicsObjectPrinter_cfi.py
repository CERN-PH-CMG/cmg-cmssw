import FWCore.ParameterSet.Config as cms

physicsObjectPrinter = cms.EDAnalyzer(
    "PhysicsObjectPrinter",
    inputCollection = cms.untracked.InputTag("cmgPFJet"),
    printSelections = cms.untracked.bool(False),
    reportEvery = cms.untracked.uint32(100)
)
