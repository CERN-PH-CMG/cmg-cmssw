import FWCore.ParameterSet.Config as cms

# do not really on the default cuts implemented here,
# as they are subject to change. 
# you should override these cuts in your analysis.

cmgDiTauSel = cms.EDFilter(
    "CmgDiTauSelector",
    src = cms.InputTag( "cmgDiTau" ),
    cut = cms.string( " pt()>0 " )
    )


