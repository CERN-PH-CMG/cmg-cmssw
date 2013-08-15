import FWCore.ParameterSet.Config as cms

simpleParticleFlow = cms.EDProducer(
    "SimpleParticleProducer",
    src = cms.InputTag('particleFlow')
    )
