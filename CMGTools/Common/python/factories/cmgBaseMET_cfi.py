import FWCore.ParameterSet.Config as cms

baseMETFactory = cms.PSet(
       inputCollection = cms.InputTag("selectedPatJetsPFlow")
       )
from CMGTools.Common.selections.kinematics_cfi import kinematics
cmgBaseMET = cms.EDFilter(
    "BaseMETPOProducer",
    cuts = cms.PSet(),
    cfg = baseMETFactory.clone()
)
