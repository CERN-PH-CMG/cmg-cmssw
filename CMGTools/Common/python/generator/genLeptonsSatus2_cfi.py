import FWCore.ParameterSet.Config as cms

genLeptonsStatus2 = cms.EDProducer(
    "GenParticlePruner",
    src = cms.InputTag("genParticles"),
    select = cms.vstring(
    "keep status = 2 & pdgId = {e-}",
    "keep status = 2 & pdgId = {e+}",
    "keep status = 2 & pdgId = {mu-}",
    "keep status = 2 & pdgId = {mu+}",
    "keep status = 2 & pdgId = {tau-}",
    "keep status = 2 & pdgId = {tau+}"
    )
)
