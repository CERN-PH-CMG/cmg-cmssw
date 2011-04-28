import FWCore.ParameterSet.Config as cms

# do not rely on the default cuts implemented here,
# as they are subject to change. 
# you should override these cuts in your analysis.


cmgTauSel = cms.EDFilter("CmgTauSelector",
                         src = cms.InputTag( "cmgTau" ),
                         cut = cms.string( "pt() > 0" )
                         )




