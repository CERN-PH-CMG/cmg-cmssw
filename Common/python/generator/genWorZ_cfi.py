import FWCore.ParameterSet.Config as cms

genWorZ = cms.EDProducer(
    "GenParticlePruner",
    src = cms.InputTag("genParticlesStatus3"),
    select = cms.vstring(
    "keep status()==3 & pdgId = {W+}",
    "keep status()==3 & pdgId = {W-}",
    "keep status()==3 & pdgId = {Z0}",
    "keep status()==3 & pdgId = {gamma}",
    "keep status()==3 & pdgId = {h0}",
    )
)
