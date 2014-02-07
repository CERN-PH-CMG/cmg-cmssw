import FWCore.ParameterSet.Config as cms

leadingCMGElectronSelector = cms.EDProducer(
    "LeadingCMGElectronSelector",
    inputCollection = cms.InputTag("cmgElectronSel"),
    index = cms.int32(1)
    )

