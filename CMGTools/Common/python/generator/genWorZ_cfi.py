import FWCore.ParameterSet.Config as cms

genWorZ = cms.EDProducer(
    "GenParticlePruner",
    src = cms.InputTag("genParticlesPruned"),
    select = cms.vstring(
    "keep status()==3 & pdgId = {W+}",
    "keep status()==3 & pdgId = {W-}",
    "keep status()==3 & pdgId = {Z0}",
    "keep status()==3 & pdgId = {gamma}",
    "keep status()==3 & pdgId = {h0}",
    "keep status()==3 & pdgId = 35", #SUSY H0
    "keep status()==3 & pdgId = 36", #SUSY A0
    )
)
