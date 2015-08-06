import FWCore.ParameterSet.Config as cms

fixedGridSigmaFastjetAll = cms.EDProducer("FixedGridSigmaProducerFastjet",
    pfCandidatesTag = cms.InputTag("packedPFCandidates"),
    maxRapidity = cms.double(5.0),
    gridSpacing = cms.double(0.55)
)

