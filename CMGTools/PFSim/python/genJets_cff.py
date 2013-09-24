from PhysicsTools.HepMCCandAlgos.genParticles_cfi import *
from RecoJets.Configuration.GenJetParticles_cff import *

genParticlesForJetsNoNu.excludeFromResonancePids = cms.vuint32(12, 14, 16)


genJets = cms.EDProducer(
    'PFSimFastJetProducer',
    jetPtThreshold = cms.double(5.),
    particleSrc = cms.InputTag('genParticlesForJetsNoNu'),
    verbose = cms.untracked.bool( False ) 
    )

genJetsSequence = cms.Sequence(
    genParticles +
    genParticlesForJetsNoNu +
    genJets
    )
