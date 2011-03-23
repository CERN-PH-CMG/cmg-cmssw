import FWCore.ParameterSet.Config as cms

leadingCMGBaseJetSelector = cms.EDProducer(
    "LeadingCMGBaseJetSelector",
    inputCollection = cms.untracked.InputTag("cmgPFBaseJetSel"),
    #copy all entries
    index = cms.untracked.int32(3)
    )

