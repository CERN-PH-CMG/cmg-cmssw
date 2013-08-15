import FWCore.ParameterSet.Config as cms

# do not really on the default cuts implemented here,
# as they are subject to change. 
# you should override these cuts in your analysis.

cmgL1TriggerObjectSel = cms.EDFilter(
    "CmgTriggerObjectSelector",
    src = cms.InputTag( "cmgL1TriggerObject" ),
    cut = cms.string( "" )
    )


