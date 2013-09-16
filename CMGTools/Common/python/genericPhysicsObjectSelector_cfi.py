import FWCore.ParameterSet.Config as cms

genericPhysicsObjectSelector = cms.EDAnalyzer(
    "GenericPhysicsObjectSelector",
    src = cms.InputTag("cmgPFJets"),
    cut = cms.string("RunInfoAccounting"),
)
