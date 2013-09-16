import FWCore.ParameterSet.Config as cms

particleFlow = cms.EDProducer("PFVertexProducer",
                              PFCandidate = cms.InputTag("particleFlow"),
                              Vertices = cms.InputTag("offlinePrimaryVertices"),
                              checkClosestZVertex = cms.bool(True)
                              )
