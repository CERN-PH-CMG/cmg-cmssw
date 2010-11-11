import FWCore.ParameterSet.Config as cms

from CMGTools.Common.factories.cmglepton_cfi import leptonFactory
electronFactory = cms.PSet(
       inputCollection = cms.InputTag("selectedPatElectronsPFlow","","PAT"),
       leptonFactory = leptonFactory.clone()
       )

from CMGTools.Common.selections.kinematics_cfi import kinematics

cmgElectron = cms.EDFilter("ElectronPOProducer",
    cfg = electronFactory.clone(),
    cuts = cms.PSet(
                kinematics = kinematics.clone()
       )    
)
