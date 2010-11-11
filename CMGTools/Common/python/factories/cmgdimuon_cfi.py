import FWCore.ParameterSet.Config as cms
from CMGTools.Common.factories.cmgmuon_cfi import cmgMuon

dimuonFactory = cms.PSet(
        leg1 = cms.PSet(
       inputCollection = cms.InputTag("cmgMuon")
       ),
        leg2 = cms.PSet(
       inputCollection = cms.InputTag("cmgMuon")
       )
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


