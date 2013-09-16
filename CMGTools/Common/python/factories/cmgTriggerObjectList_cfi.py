import FWCore.ParameterSet.Config as cms

triggerObjectListFactory = cms.PSet(
       triggerResults = cms.InputTag("TriggerResults"),
       triggerObjects = cms.InputTag("patTrigger"),
       processName = cms.untracked.string("*"),
       useTriggerObjects = cms.untracked.bool(True),
       saveAllHLTPathsInObjs = cms.untracked.bool(False)
       )

cmgTriggerObjectList = cms.EDFilter("TriggerObjectPOProducer",
    cfg = triggerObjectListFactory.clone(),
    cuts = cms.PSet(
       )
)


