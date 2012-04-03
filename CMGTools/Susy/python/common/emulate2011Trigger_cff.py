import FWCore.ParameterSet.Config as cms

emulate2011Trigger = cms.EDFilter("TriggerDataMenu",
                             triggerObjects = cms.InputTag("cmgTriggerObjectListSel"),
                             trigger = cms.InputTag("cmgTriggerObjectSel")
                             )
