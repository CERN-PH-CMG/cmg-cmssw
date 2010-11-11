import FWCore.ParameterSet.Config as cms

from CMGTools.Common.factories.cmglepton_cfi import leptonFactory
muonFactory = cms.PSet(
       inputCollection = cms.InputTag("selectedPatMuonsPFlow","","PAT"),
       trackType = cms.int32(0),#use the global track,
       muonIDType = cms.string("TMLastStationLoose"),#the flag for muonID
       leptonFactory = leptonFactory.clone()
       )

from CMGTools.Common.selections.kinematics_cfi import kinematics
from CMGTools.Common.selections.vbtfmuon_cfi import vbtfmuon

cmgMuon = cms.EDFilter("MuonPOProducer",
    cfg = muonFactory.clone(),
    cuts = cms.PSet(
                kinematics = kinematics.clone(),
                vbtfmuon = vbtfmuon.clone()
       )    
)
