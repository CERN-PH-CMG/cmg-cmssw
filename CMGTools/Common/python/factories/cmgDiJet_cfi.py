import FWCore.ParameterSet.Config as cms
diJetFactory = cms.PSet(
       leg1Collection = cms.InputTag("cmgPFJet"),
       leg2Collection = cms.InputTag("cmgPFJet"),
       )
from CMGTools.Common.selections.kinematics_cfi import kinematics
cmgDiJet = cms.EDFilter(
    "DiJetPOProducer",
    cfg = diJetFactory.clone(),
    cuts = cms.PSet(
       jetKinematics = kinematics.clone()
       ),
    verbose = cms.untracked.bool( False )
)