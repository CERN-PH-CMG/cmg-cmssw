import FWCore.ParameterSet.Config as cms

cmgMuESel = cms.EDFilter(
    "CmgMuESelector",
    src = cms.InputTag( "cmgMuE" ),
    cut = cms.string( "pt()>0" )
    )


