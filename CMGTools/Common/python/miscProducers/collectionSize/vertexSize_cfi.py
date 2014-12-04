import FWCore.ParameterSet.Config as cms

vertexSize = cms.EDFilter(
    "VertexSizeProducer",
    src = cms.InputTag('offlinePrimaryVertices')
    )
