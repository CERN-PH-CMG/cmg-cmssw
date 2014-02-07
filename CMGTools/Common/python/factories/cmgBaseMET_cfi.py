import FWCore.ParameterSet.Config as cms

baseMETFactory = cms.PSet(
       inputCollection = cms.InputTag("patMETs"),
       ptThreshold = cms.double( -1.0 ) #not used
       )
from CMGTools.Common.selections.kinematics_cfi import kinematics
cmgBaseMET = cms.EDFilter(
    "PATMETPOProducer",
    cuts = cms.PSet(),
    cfg = baseMETFactory.clone()
)
