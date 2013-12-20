import FWCore.ParameterSet.Config as cms

cmgTriggerPrescales = cms.EDProducer("CMGTriggerPrescaleProducer",
    triggerResults = cms.InputTag("TriggerResults"),
    processName = cms.string("*"),
)

