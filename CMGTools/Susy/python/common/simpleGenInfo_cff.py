import FWCore.ParameterSet.Config as cms

simpleGenInfo = cms.EDFilter("GenLeptonsProducer",
                             src = cms.InputTag("genParticlesStatus3")
                             )
