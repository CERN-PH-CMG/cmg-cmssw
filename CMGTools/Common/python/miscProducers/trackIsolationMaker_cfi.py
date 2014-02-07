import FWCore.ParameterSet.Config as cms

trackIsolationMaker = cms.EDProducer("TrackIsolationMaker",
                                     pfCandidatesTag     = cms.InputTag("particleFlow"),
                                     vertexInputTag      = cms.InputTag("offlinePrimaryVertices"),
                                     dR_ConeSize         = cms.double(0.3),
                                     dz_CutValue         = cms.double(0.05),
                                     minPt_PFCandidate   = cms.double(0.0)
)
