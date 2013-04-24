import FWCore.ParameterSet.Config as cms

isrWeight = cms.EDFilter("SusyISRWeightProducer",
                         src = cms.InputTag("genParticlesPruned")
                         )
