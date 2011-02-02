import FWCore.ParameterSet.Config as cms

particleFlow = cms.EDProducer(
    "GenToPFConverter",
    src = cms.InputTag( 'genParticles' )
    )
