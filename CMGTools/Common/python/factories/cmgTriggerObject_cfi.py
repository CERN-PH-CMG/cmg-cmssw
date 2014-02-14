import FWCore.ParameterSet.Config as cms

triggerObjectFactory = cms.PSet(
       triggerResults = cms.InputTag("TriggerResults"),
       triggerObjects = cms.InputTag("patTrigger"),
       processName = cms.untracked.string("HLT"), # FIXME: "*" doesn't work anymore
       useTriggerObjects = cms.untracked.bool(False)
       )

cmgTriggerObject = cms.EDFilter("TriggerObjectPOProducer",
    cfg = triggerObjectFactory.clone(),
    cuts = cms.PSet(
       )
)

