import FWCore.ParameterSet.Config as cms

leadingCMGPFJetSelector = cms.EDProducer(
    "LeadingCMGPFJetSelector",
    inputCollection = cms.InputTag("cmgPFJetSel"),
    #copy all entries
    index = cms.int32(2)
    )

