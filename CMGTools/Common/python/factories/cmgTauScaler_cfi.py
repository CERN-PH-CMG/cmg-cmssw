import FWCore.ParameterSet.Config as cms

from CMGTools.Common.selections.kinematics_cfi import kinematics

tauScaler = cms.PSet(
    inputCollection = cms.InputTag("cmgTauSel"),
    nSigma = cms.double(0.),
    # 3% uncertainty
    uncertainty = cms.double(0.03)
    )

cmgTauScaler = cms.EDFilter(
    "TauPOScaler",
    cfg = tauScaler.clone(nSigma = 0.),
    cuts = cms.PSet()
)

