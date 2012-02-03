import FWCore.ParameterSet.Config as cms

cmgMuEleSel = cms.EDFilter(
    "CmgMuEleSelector",
    src = cms.InputTag( "cmgMuEle" ),
    cut = cms.string( "pt()>0" )
    )


