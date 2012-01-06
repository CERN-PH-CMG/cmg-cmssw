import FWCore.ParameterSet.Config as cms

boolToIntFilter = cms.EDFilter(
    "BoolToIntProducer",
    src = cms.InputTag('dummy')
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

#Branch 4 of Events tree: bool_ecalDeadCellTPfilter__PAT. Total size = 63782
#Branch 5 of Events tree: bool_HBHENoiseFilterResultProducer2010_HBHENoiseFilterResult_PAT. Total size = 64904
#Branch 6 of Events tree: bool_HBHENoiseFilterResultProducer2011IsoDefault_HBHENoiseFilterResult_PAT. Total size = 65234
#Branch 7 of Events tree: bool_HBHENoiseFilterResultProducer2011NonIsoRecommended_HBHENoiseFilterResult_PAT. Total size = 65465
#Branch 8 of Events tree: bool_eeNoiseFilter_Result_PAT. Total size = 63749
#Branch 9 of Events tree: bool_goodPrimaryVertexFilter_Result_PAT. Total size = 64079
#Branch 10 of Events tree: bool_greedyMuonsTagging_Result_PAT. Total size = 63914
#Branch 11 of Events tree: bool_inconsistentMuonsTagging_Result_PAT. Total size = 64112
#Branch 12 of Events tree: bool_recovRecHitFilter_Result_PAT. Total size = 63881
#Branch 13 of Events tree: bool_scrapingFilter_Result_PAT. Total size = 63782
#Branch 14 of Events tree: bool_trackingFailureFilter_Result_PAT. Total size = 64013
