import FWCore.ParameterSet.Config as cms

from PhysicsTools.PatAlgos.triggerLayer1.triggerProducer_cfi import patTrigger

patTrigger = patTrigger.clone()

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

cmgTriggerObjectSequence = cms.Sequence(patTrigger+cmgTriggerObject)