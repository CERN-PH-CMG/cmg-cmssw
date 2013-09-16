import FWCore.ParameterSet.Config as cms

triggerObjectFactory = cms.PSet(
       triggerResults = cms.InputTag("TriggerResults"),
       triggerObjects = cms.InputTag("patTrigger"),
       processName = cms.untracked.string("*"),
       useTriggerObjects = cms.untracked.bool(False)
       )

cmgTriggerObject = cms.EDFilter("TriggerObjectPOProducer",
    cfg = triggerObjectFactory.clone(),
    cuts = cms.PSet(
       )
)

