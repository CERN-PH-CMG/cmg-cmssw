import FWCore.ParameterSet.Config as cms

# do not really on the default cuts implemented here,
# as they are subject to change. 
# you should override these cuts in your analysis.

cmgStructuredPFJetSel = cms.EDFilter(
    "CmgStructuredPFJetSelector",
    src = cms.InputTag("cmgStructuredPFJet"),
    cut = cms.string( "pt()>0" )
    )
