import FWCore.ParameterSet.Config as cms

muontrigger = cms.PSet(
    match = cms.string('sourcePtr().get().triggerObjectMatches().size() > 0'),
    jet30U = cms.string('sourcePtr().get().triggerObjectMatchesByPath(\"HLT_Jet30U\").size() > 0'),
    jet50U = cms.string('sourcePtr().get().triggerObjectMatchesByPath(\"HLT_Jet50U\").size() > 0'),
    jet70U = cms.string('sourcePtr().get().triggerObjectMatchesByPath(\"HLT_Jet70U\").size() > 0'),
    jet100U = cms.string('sourcePtr().get().triggerObjectMatchesByPath(\"HLT_Jet100U\").size() > 0'),
    jet140U = cms.string('sourcePtr().get().triggerObjectMatchesByPath(\"HLT_Jet140U\").size() > 0'),
    )
