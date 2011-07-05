import FWCore.ParameterSet.Config as cms

from CMGTools.Common.factories.cmgLepton_cfi import leptonFactory
muonFactory = cms.PSet(
       inputCollection = cms.InputTag("selectedPatMuonsAK5"),
       trackType = cms.int32(0),#use the global track,
       muonIDType = cms.string("TMLastStationLoose"),#the flag for muonID
       leptonFactory = leptonFactory.clone()
       )

from CMGTools.Common.selections.isolation_cfi import isolation
from CMGTools.Common.selections.vbtfmuon_cfi import vbtfmuon

cmgMuon = cms.EDFilter("MuonPOProducer",
    cfg = muonFactory.clone(),
    cuts = cms.PSet(
                isomuon = isolation.clone(),
                vbtfmuon = vbtfmuon.clone()
       )    
)
