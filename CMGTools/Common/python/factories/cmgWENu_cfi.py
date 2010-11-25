import FWCore.ParameterSet.Config as cms

wenuFactory = cms.PSet(
       leg1Collection = cms.InputTag('cmgElectron'),
       leg2Collection = cms.InputTag('cmgMETPFCandidates')
)
from CMGTools.Common.selections.kinematics_cfi import kinematics
cmgWENu = cms.EDFilter(
    "WENuPOProducer",
    cfg = wenuFactory.clone(),
    cuts = cms.PSet(
       kinematics = kinematics.clone(),
      ),
    )