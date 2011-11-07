import FWCore.ParameterSet.Config as cms

from RecoParticleFlow.PostProcessing.pfPostProcessingEventContent_cff import pfPostProcessingEventContent

eventCleaning = pfPostProcessingEventContent
eventCleaning += cms.untracked.vstring(
    'keep *_ecalDeadCellTPfilter_*_*',
    'keep *_HBHENoiseFilterResultProducer*_*_*',
    'keep *_BeamHaloSummary_*_*',
    'keep *_recovRecHitFilter_*_*',
    'keep *_eeNoiseFilter_*_*',
    'keep *_trackingFailureFilter_*_*',
    'keep *_goodPrimaryVertexFilter_*_*',
    'keep *_scrapingFilter_*_*',
    'keep *_totalKinematicsFilterCMG_*_*'
    )

