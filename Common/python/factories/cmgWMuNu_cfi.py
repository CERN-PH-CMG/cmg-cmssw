import FWCore.ParameterSet.Config as cms

wmunuFactory = cms.PSet(
       leg1Collection = cms.InputTag('cmgMuon'),
       leg2Collection = cms.InputTag('cmgMETPFCandidates')
)
from CMGTools.Common.selections.kinematics_cfi import kinematics
cmgWMuNu = cms.EDFilter(
    "WMuNuPOProducer",
    cfg = wmunuFactory.clone(),
    cuts = cms.PSet(
       kinematics = kinematics.clone(),
      ),
    )