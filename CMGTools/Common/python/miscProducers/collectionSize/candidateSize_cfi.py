import FWCore.ParameterSet.Config as cms

candidateSize = cms.EDFilter(
    "CandidateSizeProducer",
    src = cms.InputTag('')
    )
