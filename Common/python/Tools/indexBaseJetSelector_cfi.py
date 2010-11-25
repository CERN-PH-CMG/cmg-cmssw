import FWCore.ParameterSet.Config as cms

indexBaseJetSelector = cms.EDProducer("BaseJetCollectionSelector",
                                      inputCollection = cms.untracked.InputTag("cmgPFBaseJet"),
                                      #copy all entries
                                      index = cms.untracked.int32(3)
                                      )

