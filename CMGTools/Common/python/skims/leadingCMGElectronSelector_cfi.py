import FWCore.ParameterSet.Config as cms

leadingCMGElectronSelector = cms.EDProducer(
    "LeadingCMGElectronSelector",
    inputCollection = cms.untracked.InputTag("cmgElectronSel"),
    index = cms.untracked.int32(1)
    )

