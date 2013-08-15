
import FWCore.ParameterSet.Config as cms

goodPrimaryVertexFilter = cms.EDFilter(
  "GoodPrimaryVertexFilter",
   VertexSource = cms.InputTag('offlinePrimaryVertices'),
   debugOn     = cms.untracked.bool(False),
   TaggingMode = cms.bool(True)
)
