import FWCore.ParameterSet.Config as cms

indexCMGPFJetSelector = cms.EDProducer(
    "IndexCMGPFJetSelector",
    inputCollection = cms.InputTag("cmgPFJetSel"),
    #copy all entries
    min = cms.int32(-1),
    max = cms.int32(-1),
    verbose = cms.untracked.bool(False)
    )
