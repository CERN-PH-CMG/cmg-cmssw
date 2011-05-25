import FWCore.ParameterSet.Config as cms

baseMETFactory = cms.PSet(
       inputCollection = cms.InputTag("selectedPatJetsAK5"),
       ptThreshold = cms.double( 0.0 )
       )
from CMGTools.Common.selections.kinematics_cfi import kinematics
cmgBaseCandMET = cms.EDFilter(
    "BaseMETPOProducer",
    cuts = cms.PSet(),
    cfg = baseMETFactory.clone()
)
