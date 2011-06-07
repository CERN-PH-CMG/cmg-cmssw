import FWCore.ParameterSet.Config as cms

cmgTauESel = cms.EDFilter(
    "CmgTauESelector",
    src = cms.InputTag( "cmgTauE" ),
    cut = cms.string("pt()>0")
    )




