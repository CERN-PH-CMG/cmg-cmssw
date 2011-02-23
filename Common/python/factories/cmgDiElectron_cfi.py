import FWCore.ParameterSet.Config as cms
from CMGTools.Common.factories.cmgmuon_cfi import cmgMuon

dimuonFactory = cms.PSet(
       leg1Collection = cms.InputTag("cmgElectron"),
       leg2Collection = cms.InputTag("cmgElectron")
)
from CMGTools.Common.selections.kinematics_cfi import kinematics

from CMGTools.Common.selections.zee_cfi import zee
cmgDiElectron = cms.EDFilter(
    "DiElectronPOProducer",
    cfg = dimuonFactory.clone(),
    cuts = cms.PSet(
       kinematics = kinematics.clone(),
       zee = zee.clone()
      ),
    )


