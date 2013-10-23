import FWCore.ParameterSet.Config as cms

pfRecHitsToClusters = cms.EDProducer("PFRecHitToClusterConvertor",
  source = cms.InputTag("particleFlowRecHitECAL", "Cleaned")
)
