
import FWCore.ParameterSet.Config as cms

eeNoiseFilter = cms.EDFilter(
  "EENoiseFilter",
  EERecHitSource = cms.InputTag('reducedEcalRecHitsEE'),
  MaxNrRecHits = cms.uint32(2000),
  TaggingMode = cms.bool(True)
)
