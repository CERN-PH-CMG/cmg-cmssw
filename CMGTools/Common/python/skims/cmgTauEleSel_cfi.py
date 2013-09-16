import FWCore.ParameterSet.Config as cms

cmgTauEleSel = cms.EDFilter(
    "CmgTauEleSelector",
    src = cms.InputTag( "cmgTauEle" ),
    cut = cms.string("pt()>0")
    )




