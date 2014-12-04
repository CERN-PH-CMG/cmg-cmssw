import FWCore.ParameterSet.Config as cms

baseMETFactory = cms.PSet(
       inputCollection = cms.InputTag("pfMet"),
       ptThreshold = cms.double( -1.0 ) #not used
       )
from CMGTools.Common.selections.kinematics_cfi import kinematics
cmgBaseMETFromPFMET = cms.EDFilter(
    "PFMETPOProducer",
    cuts = cms.PSet(),
    cfg = baseMETFactory.clone()
)
