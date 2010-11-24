import FWCore.ParameterSet.Config as cms

indexSelector = cms.EDProducer("POCollectionSelector",
                               inputCollection = cms.untracked.InputTag("cmgPFCand"),
                               #copy only the first event
                               index = cms.untracked.int32(1)
                            )

