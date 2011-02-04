import FWCore.ParameterSet.Config as cms

# do not really on the default cuts implemented here,
# as they are subject to change. 
# you should override these cuts in your analysis.

cmgPFBaseJetSel = cms.EDFilter(
    "CmgBaseJetSelector",
    src = cms.InputTag("cmgPFJet"),
    cut = cms.string( "30" )
    )


