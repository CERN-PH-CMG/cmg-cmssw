import FWCore.ParameterSet.Config as cms

# do not really on the default cuts implemented here,
# as they are subject to change. 
# you should override these cuts in your analysis.

cmgWENuSel = cms.EDFilter(
    "CmgWENuSelector",
    src = cms.InputTag( "cmgWENu" ),
    cut = cms.string( "pt()>0" )
    )


