import FWCore.ParameterSet.Config as cms

indexSelector = cms.EDProducer("POCollectionSelector",
                               inputCollection = cms.untracked.InputTag("cmgPFCand"),
                               #copy all entries
                               index = cms.untracked.int32(-1)
                               )

