import FWCore.ParameterSet.Config as cms

genLeptonsStatus1 = cms.EDProducer(
    "GenParticlePruner",
    src = cms.InputTag("genParticles"),
    select = cms.vstring(
    "keep status = 1 & pdgId = {e-}",
    "keep status = 1 & pdgId = {e+}",
    "keep status = 1 & pdgId = {mu-}",
    "keep status = 1 & pdgId = {mu+}"
    )
)

