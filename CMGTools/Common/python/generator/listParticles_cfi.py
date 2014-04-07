import FWCore.ParameterSet.Config as cms

listParticles = cms.EDAnalyzer(
    "ParticleListDrawer",
    maxEventsToPrint = cms.untracked.int32(1),
    printVertex = cms.untracked.bool(False),
    src = cms.InputTag("genParticlesPruned")
    )
