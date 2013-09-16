import FWCore.ParameterSet.Config as cms

leadingCMGBaseJetSelector = cms.EDProducer(
    "LeadingCMGBaseJetSelector",
    inputCollection = cms.InputTag("cmgPFBaseJetSel"),
    #copy all entries
    index = cms.int32(2)
    )

