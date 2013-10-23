import FWCore.ParameterSet.Config as cms

baseMETModifier = cms.PSet(
       inputCollection = cms.InputTag("particleFlow"),
       metCollection = cms.InputTag("cmgPFMET"),
       ptThreshold = cms.double( -1.0 ), #not used
       operator = cms.string('-')
       )
from CMGTools.Common.selections.kinematics_cfi import kinematics
cmgBaseMETModifier = cms.EDFilter(
    "BaseMETModifier",
    cuts = cms.PSet(),
    cfg = baseMETModifier.clone()
)
