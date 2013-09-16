import FWCore.ParameterSet.Config as cms

baseMETScaler = cms.PSet(
    jetLabel = cms.InputTag("cmgPFJetSel"),
    metLabel = cms.InputTag("cmgPFMET"),
    jecUncDirection = cms.double(0.),
    doType1Correction = cms.bool(False)
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

cmgPFMETType1Corrected = cms.EDFilter(
    "METPOScaler",
    cfg = baseMETScaler.clone(doType1Correction = True, jetLabel = "cmgPFJetType1MET"),
    cuts = cms.PSet(
    )
)
