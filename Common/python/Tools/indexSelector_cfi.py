import FWCore.ParameterSet.Config as cms

indexSelector = cms.EDProducer(
                                "POCollectionSelector",
                                inputCollection = cms.InputTag("cmgMuon"),
                                index = cms.untracked.int32(-1)
                                )

