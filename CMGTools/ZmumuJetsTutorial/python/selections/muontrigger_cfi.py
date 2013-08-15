import FWCore.ParameterSet.Config as cms

muontrigger = cms.PSet(
    match = cms.string('sourcePtr().triggerObjectMatches().size() > 0'),
    hltMu9 = cms.string('sourcePtr().triggerObjectMatchesByPath(\"HLT_Mu9\").size() > 0'),
    hltMu11 = cms.string('sourcePtr().triggerObjectMatchesByPath(\"HLT_Mu11\").size() > 0'),
    hltMu13 = cms.string('sourcePtr().triggerObjectMatchesByPath(\"HLT_Mu13_v1\").size() > 0'),
    hltMu15 = cms.string('sourcePtr().triggerObjectMatchesByPath(\"HLT_Mu15_v1\").size() > 0 ||'+
                         'sourcePtr().triggerObjectMatchesByPath(\"HLT_Mu15_v2\").size() > 0 ||'+
                         'sourcePtr().triggerObjectMatchesByPath(\"HLT_Mu15_v3\").size() > 0'
                         ),
    hltMu17 = cms.string('sourcePtr().triggerObjectMatchesByPath(\"HLT_Mu17_v1\").size() > 0'),
    hltMu19 = cms.string('sourcePtr().triggerObjectMatchesByPath(\"HLT_Mu19_v1\").size() > 0'),
    hltMu20 = cms.string('sourcePtr().triggerObjectMatchesByPath(\"HLT_Mu20_v1\").size() > 0 ||'+
                         'sourcePtr().triggerObjectMatchesByPath(\"HLT_Mu20_v2\").size() > 0'
                         ),
    hltMu21 = cms.string('sourcePtr().triggerObjectMatchesByPath(\"HLT_Mu21_v1\").size() > 0'),
    )
