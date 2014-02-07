
import FWCore.ParameterSet.Config as cms

recovRecHitFilter = cms.EDFilter(
  "RecovRecHitFilter",
  EERecHitSource = cms.InputTag('reducedEcalRecHitsEE'),
  # EERecHitSource = cms.InputTag("ecalRecHit:EcalRecHitsEE"),
  MinRecovE = cms.double(30),
  TaggingMode = cms.bool(True)
)
