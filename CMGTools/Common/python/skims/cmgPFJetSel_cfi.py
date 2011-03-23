import FWCore.ParameterSet.Config as cms

# do not really on the default cuts implemented here,
# as they are subject to change. 
# you should override these cuts in your analysis.

cmgPFJetSel = cms.EDFilter(
    "CmgPFJetSelector",
    src = cms.InputTag("cmgPFJet"),
    cut = cms.string( "pt()>0" )
    )


