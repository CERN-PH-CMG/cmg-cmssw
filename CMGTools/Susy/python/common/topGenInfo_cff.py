import FWCore.ParameterSet.Config as cms

topGenInfo = cms.EDFilter("GenPtProducer",
                             src = cms.InputTag("genParticlesStatus3")
                             )
