import FWCore.ParameterSet.Config as cms

leadingCMGPFJetSelector = cms.EDProducer(
    "LeadingCMGPFJetSelector",
    inputCollection = cms.untracked.InputTag("cmgPFJet"),
    #copy all entries
    index = cms.untracked.int32(3)
    )

