import FWCore.ParameterSet.Config as cms

pfsim = cms.EDProducer(
    'PFSimParticleProducer',
    hepmcSrc = cms.InputTag('generator'),
    genSrc = cms.InputTag('genParticles'),
    verbose = cms.untracked.bool( False )
    )

jets = cms.EDProducer(
    'PFSimFastJetProducer',
    particleSrc = cms.InputTag('pfsim'),
    jetPtThreshold = cms.double(5.),
    )

pfsimSequence = cms.Sequence(
    pfsim +
    jets
    ) 
