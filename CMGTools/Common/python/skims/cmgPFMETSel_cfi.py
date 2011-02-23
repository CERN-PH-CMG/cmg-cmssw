import FWCore.ParameterSet.Config as cms

# do not really on the default cuts implemented here,
# as they are subject to change. 
# you should override these cuts in your analysis.

cmgPFMETSel = cms.EDFilter(
    "CandViewSelector",
    src = cms.InputTag("patMETsPFlow"),
    cut = cms.string( "pt()>0" )
    )


