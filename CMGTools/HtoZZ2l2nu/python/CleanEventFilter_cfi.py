import FWCore.ParameterSet.Config as cms

cleanEventFilter = cms.EDFilter("DileptonPlusMETEventFilter",
                                source = cms.InputTag("cleanEvent"),
                                selectionPaths = cms.vint32(1,2,3),
                                selectionStep = cms.int32(3)
                                )
