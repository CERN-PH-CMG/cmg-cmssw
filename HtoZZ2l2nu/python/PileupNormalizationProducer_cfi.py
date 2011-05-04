import FWCore.ParameterSet.Config as cms

from CMGTools.HtoZZ2l2nu.StandardSelections_cfi import *
puWeights = cms.EDProducer("PileupNormalizationProducer",
                           Vertices = BaseVertexSelection.clone(),
                           useVertexDistribution = cms.bool(False)
                           )
