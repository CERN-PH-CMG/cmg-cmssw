import FWCore.ParameterSet.Config as cms
from CMGTools.Common.factories.cmgBaseJet_cfi import baseJetFactory
pfJetFactory = cms.PSet(
       inputCollection = cms.InputTag("selectedPatJetsPFlow"),
       baseJetFactory = baseJetFactory.clone()
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
