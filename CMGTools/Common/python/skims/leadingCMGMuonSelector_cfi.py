import FWCore.ParameterSet.Config as cms

leadingCMGMuonSelector = cms.EDProducer(
    "LeadingCMGMuonSelector",
    inputCollection = cms.InputTag("cmgMuonSel"),
    index = cms.int32(1)
    )

