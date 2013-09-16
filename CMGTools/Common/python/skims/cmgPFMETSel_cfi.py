import FWCore.ParameterSet.Config as cms

# do not really on the default cuts implemented here,
# as they are subject to change. 
# you should override these cuts in your analysis.

cmgCandSel = cms.EDFilter(
    "CandViewSelector",
    src = cms.InputTag(""),
    cut = cms.string( "pt()>0" )
    )


