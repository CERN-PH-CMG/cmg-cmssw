import FWCore.ParameterSet.Config as cms
from CMGTools.Common.factories.cmgMuon_cfi import cmgMuon

dimuonFactory = cms.PSet(
       leg1Collection = cms.InputTag("cmgMuon"),
       leg2Collection = cms.InputTag("cmgMuon")
)
from CMGTools.Common.selections.kinematics_cfi import kinematics
from CMGTools.Common.selections.zmumu_cfi import zmumu
cmgDiMuon = cms.EDFilter(
    "DiMuonPOProducer",
    cfg = dimuonFactory.clone(),
    cuts = cms.PSet(
       kinematics = kinematics.clone(),
       zmumu = zmumu.clone()
      ),
    )


