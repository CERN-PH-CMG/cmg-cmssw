import FWCore.ParameterSet.Config as cms

from CMGTools.Common.selections.kinematics_cfi import kinematics

baseJetScaler = cms.PSet(
    inputCollection = cms.InputTag("cmgPFJet"),
    jecUncDirection = cms.double(0.)
    )

cmgPFJetUp = cms.EDFilter(
    "PFJetPOScaler",
    cfg = baseJetScaler.clone(jecUncDirection = 2.),
    cuts = cms.PSet(
    jetKinematics = kinematics.clone()
    )
)

cmgPFJetDown = cms.EDFilter(
    "PFJetPOScaler",
    cfg = baseJetScaler.clone(jecUncDirection = -2.),
    cuts = cms.PSet(
    jetKinematics = kinematics.clone()
    )
)
