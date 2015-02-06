
import FWCore.ParameterSet.Config as cms

pfEventFilter = cms.EDFilter(
  "PFEventFilter",
  PFCandSource = cms.InputTag('particleFlow'),
  MuonSource = cms.InputTag('muons'),
  VtxSource = cms.InputTag('offlinePrimaryVertices'),
  Debug = cms.bool(False),
  taggingMode = cms.bool( False )
)
