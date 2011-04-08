import FWCore.ParameterSet.Config as cms

leadingCMGMuonSelector = cms.EDProducer(
    "LeadingCMGMuonSelector",
    inputCollection = cms.untracked.InputTag("cmgMuonSel"),
    index = cms.untracked.int32(1)
    )

