import FWCore.ParameterSet.Config as cms

# do not really on the default cuts implemented here,
# as they are subject to change. 
# you should override these cuts in your analysis.

cmgMuonSel = cms.EDFilter(
    "CmgMuonSelector",
    src = cms.InputTag( "cmgMuon" ),
    cut = cms.string( "pt()>0" )
    )


