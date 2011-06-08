import FWCore.ParameterSet.Config as cms

from CMGTools.Common.selections.kinematics_cfi import kinematics

baseJetScaler = cms.PSet(
    inputCollection = cms.InputTag("cmgBaseJet"),
    jecUncDirection = cms.double(0.)
    )

cmgBaseJetUp = cms.EDFilter(
    "BaseJetPOScaler",
    cfg = baseJetScaler.clone(jecUncDirection = 2.),
    cuts = cms.PSet(
    jetKinematics = kinematics.clone()
    )
)

cmgBaseJetDown = cms.EDFilter(
    "BaseJetPOScaler",
    cfg = baseJetScaler.clone(jecUncDirection = -2.),
    cuts = cms.PSet(
    jetKinematics = kinematics.clone()
    )
)
