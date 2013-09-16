import FWCore.ParameterSet.Config as cms

from CMGTools.Common.selections.kinematics_cfi import kinematics

cmgPFCandidate = cms.EDFilter(
    "PFCandidatePOProducer",
    cfg = cms.PSet(
      inputCollection = cms.InputTag("particleFlow")
      ),
    cuts = cms.PSet(
      kinematics = kinematics.clone()
      ),    
)
