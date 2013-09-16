import FWCore.ParameterSet.Config as cms

from CMGTools.Common.factories.cmgLepton_cfi import leptonFactory
muonFactory = cms.PSet(
       inputCollection = cms.InputTag("patMuonsWithTrigger"),
       trackType = cms.int32(0),#use the global track,
       muonIDType = cms.string("TMLastStationLoose"),#the flag for a muonID
       leptonFactory = leptonFactory.clone()
       )


from CMGTools.Common.selections.muonIDs_cfi import *

cmgMuon = cms.EDFilter("MuonPOProducer",
    cfg = muonFactory.clone(),
    cuts = cms.PSet(
                
                loosemuon = loosemuon.clone(),
                softmuonNoVtx = softmuonNoVtx.clone(),
                tightmuonNoVtx = tightmuonNoVtx.clone(),
                highptmuonNoVtx = highptmuonNoVtx.clone(),
                vbtfmuon = vbtfmuon.clone(),
         )    
)
