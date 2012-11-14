import FWCore.ParameterSet.Config as cms

l1TriggerObjectFactory = cms.PSet(
       L1Results = cms.InputTag("gtDigis"),
       )

cmgL1TriggerObject = cms.EDFilter("L1TriggerObjectPOProducer",
    cfg = l1TriggerObjectFactory.clone(),
    cuts = cms.PSet(
       )
)

