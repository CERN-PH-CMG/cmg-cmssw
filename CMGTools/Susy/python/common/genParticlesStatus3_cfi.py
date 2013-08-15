import FWCore.ParameterSet.Config as cms

genParticlesStatus3 = cms.EDProducer(
   "GenParticlePruner",
   src = cms.InputTag("genParticlesPruned"),
   select = cms.vstring(
       "drop  *  ",
       "keep status()==3",
       "keep+ status()==3"
   )
)
