import FWCore.ParameterSet.Config as cms

#adds lots of event wise cleaning filters and enables them
boolToIntFilter = cms.EDFilter(
    "BoolToIntProducer",
    src = cms.InputTag('dummy'),
    filter = cms.untracked.bool(True)
)

ecalDeadCellTPfilter = boolToIntFilter.clone(src = cms.InputTag('ecalDeadCellTPfilter'))
HBHENoiseFilterResultProducer2010 = boolToIntFilter.clone(src = cms.InputTag('HBHENoiseFilterResultProducer2010','HBHENoiseFilterResult'))
HBHENoiseFilterResultProducer2011IsoDefault = boolToIntFilter.clone(src = cms.InputTag('HBHENoiseFilterResultProducer2011IsoDefault','HBHENoiseFilterResult'))
HBHENoiseFilterResultProducer2011NonIsoRecommended = boolToIntFilter.clone(src = cms.InputTag('HBHENoiseFilterResultProducer2011NonIsoRecommended','HBHENoiseFilterResult'))
eeNoiseFilter = boolToIntFilter.clone(src = cms.InputTag('eeNoiseFilter','Result'))
goodPrimaryVertexFilter = boolToIntFilter.clone(src = cms.InputTag('goodPrimaryVertexFilter','Result'))
greedyMuonsTagging = boolToIntFilter.clone(src = cms.InputTag('greedyMuonsTagging','Result'))
inconsistentMuonsTagging = boolToIntFilter.clone(src = cms.InputTag('inconsistentMuonsTagging','Result'))
recovRecHitFilter = boolToIntFilter.clone(src = cms.InputTag('recovRecHitFilter','Result'))
scrapingFilter = boolToIntFilter.clone(src = cms.InputTag('scrapingFilter','Result'))
trackingFailureFilter = boolToIntFilter.clone(src = cms.InputTag('trackingFailureFilter','Result'))

boolToIntSequence = cms.Sequence(
                    ecalDeadCellTPfilter+
                    HBHENoiseFilterResultProducer2010+
                    HBHENoiseFilterResultProducer2011IsoDefault+
                    HBHENoiseFilterResultProducer2011NonIsoRecommended+
                    eeNoiseFilter+
                    goodPrimaryVertexFilter+
                    greedyMuonsTagging+
                    inconsistentMuonsTagging+
                    recovRecHitFilter+
                    scrapingFilter+
                    trackingFailureFilter
                         )
