import FWCore.ParameterSet.Config as cms

baseMETScaler = cms.PSet(
    jetLabel = cms.InputTag("cmgPFJet"),
    metLabel = cms.InputTag("cmgPFMET"),
    jecUncDirection = cms.double(0.)
    )


cmgMETUp = cms.EDFilter(
    "METPOScaler",
    cfg = baseMETScaler.clone(jecUncDirection = +1.),
    cuts = cms.PSet(
    )
)

cmgMETDown = cms.EDFilter(
    "METPOScaler",
    cfg = baseMETScaler.clone(jecUncDirection = -1.),
    cuts = cms.PSet(
    )
)
