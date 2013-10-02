import FWCore.ParameterSet.Config as cms

# do not really on the default cuts implemented here,
# as they are subject to change. 
# you should override these cuts in your analysis.

cmgL1TriggerObjectCount = cms.EDFilter(
    "CandViewCountFilter",
    src = cms.InputTag("cmgL1TriggerObjectSel"),
    minNumber = cms.uint32(0),
    )


