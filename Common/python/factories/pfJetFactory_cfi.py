import FWCore.ParameterSet.Config as cms

pfJetFactory = cms.PSet(
       inputCollection = cms.InputTag("selectedPatJetsAK5")
       )
from CMGTools.Common.selections.kinematics_cfi import kinematics
cmgPFJet = cms.EDFilter(
    "PFJetPOProducer",
    cfg = pfJetFactory.clone(),
    cuts = cms.PSet(
       jetKinematics = kinematics.clone()
       ),
    verbose = cms.untracked.bool( False )
)
