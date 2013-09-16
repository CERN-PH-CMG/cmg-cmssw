import FWCore.ParameterSet.Config as cms


# creates a collection of PFCandidates containing only the muons

allPFMuons = cms.EDFilter(
    "GenericPFCandidateSelector",
    src = cms.InputTag('particleFlow'),
    cut = cms.string("abs(pdgId())==13")
    )





