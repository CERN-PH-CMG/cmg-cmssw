import FWCore.ParameterSet.Config as cms

from CMGTools.Common.selections.kinematics_cfi import kinematics

cmgPFCand = cms.EDFilter("PFCandidatePOProducer",
    cfg = cms.PSet(
                 inputCollection = cms.InputTag("selectedPatPFParticlesPFlow")
       ),
    cuts = cms.PSet(
                kinematics = kinematics.clone()
           ),    
)