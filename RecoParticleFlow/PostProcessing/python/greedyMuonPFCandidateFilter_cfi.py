import FWCore.ParameterSet.Config as cms

greedyMuonPFCandidateFilter = cms.EDFilter(
    "GreedyMuonPFCandidateFilterPF",
    PFCandidates = cms.InputTag("particleFlow"),
    eOverPMax = cms.double(1.),
    verbose = cms.untracked.bool( False ),
    taggingMode = cms.bool( False )
    )
