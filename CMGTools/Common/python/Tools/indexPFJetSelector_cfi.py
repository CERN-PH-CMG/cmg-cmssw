import FWCore.ParameterSet.Config as cms

indexPFJetSelector = cms.EDProducer("PFJetCollectionSelector",
                                    inputCollection = cms.untracked.InputTag("cmgPFJet"),
                                    #copy all entries
                                    index = cms.untracked.int32(3)
                                    )

