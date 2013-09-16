import FWCore.ParameterSet.Config as cms

genParticlesStatus3 = cms.EDProducer(
    "GenParticlePruner",
    src = cms.InputTag("genParticles"),
    select = cms.vstring(
    "keep status()==3"
    )
)
