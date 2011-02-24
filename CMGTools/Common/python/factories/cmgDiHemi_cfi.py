import FWCore.ParameterSet.Config as cms

from CMGTools.Common.selections.kinematics_cfi import kinematics

diHemiFactory = cms.PSet(
    leg1Collection = cms.InputTag("dummy"),
    leg2Collection = cms.InputTag("dummy")
    )

cmgDiHemi = cms.EDFilter("DiHemispherePOProducer",
                         cfg = diHemiFactory.clone(
    leg1Collection = cms.InputTag("cmgHemi"),
    leg2Collection = cms.InputTag("cmgHemi")
    ),
                         cuts = cms.PSet(kinematics = kinematics.clone())
                         )
