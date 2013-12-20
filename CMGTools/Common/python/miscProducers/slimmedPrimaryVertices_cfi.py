import FWCore.ParameterSet.Config as cms

slimmedPrimaryVertices = cms.EDProducer("CMGVertexSlimmer",
    src = cms.InputTag("offlinePrimaryVertices"),
)

