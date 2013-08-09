import FWCore.ParameterSet.Config as cms

cmgCandidates = cms.EDProducer(
    "CMGCandidateProducer",
    inputCollection = cms.InputTag("particleFlow"),
)
