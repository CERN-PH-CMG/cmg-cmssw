import FWCore.ParameterSet.Config as cms

# cmgTauMuSel = cms.EDFilter(
#     "CmgTauMuSelector",
#     src = cms.InputTag( "cmgTauMu" ),
#     cut = cms.string( "pt()>0" )
#     )

cmgTauMuSel = cms.EDFilter(
    "PATCompositeCandidateSelector",
    src = cms.InputTag( "cmgTauMu" ),
    cut = cms.string( "" )
    )

