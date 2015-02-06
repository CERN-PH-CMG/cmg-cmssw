import FWCore.ParameterSet.Config as cms

from CMGTools.Common.selections.kinematics_cfi import kinematics

baseJetScaler = cms.PSet(
    inputCollection = cms.InputTag("cmgPFJet"),
    nSigma = cms.double(0.),
    uncertainty = cms.double(-1)
    )

cmgPFJetUp = cms.EDFilter(
    "PFJetPOScaler",
    #COLIN why 2 and not 1?
    cfg = baseJetScaler.clone(nSigma = 2.),
    cuts = cms.PSet(
    jetKinematics = kinematics.clone()
    )
)

cmgPFJetDown = cms.EDFilter(
    "PFJetPOScaler",
    cfg = baseJetScaler.clone(nSigma = -2.),
    cuts = cms.PSet(
    jetKinematics = kinematics.clone()
    )
)
