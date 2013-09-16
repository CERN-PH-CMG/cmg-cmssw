import FWCore.ParameterSet.Config as cms


# to run this module, you need in input:
# particleFlow : the collection of reco::PFCandidates
# muons : the collection of reco::Muons


# filtering events with no greedy muon

from RecoParticleFlow.PostProcessing.greedyMuonPFCandidateFilter_cfi import greedyMuonPFCandidateFilter

# filtering events which could be improved to reduce the MET.

from  RecoParticleFlow.PostProcessing.pfEventFilter_cfi import pfEventFilter

# Inconsistent muon filter

from  RecoParticleFlow.PostProcessing.inconsistentMuonPFCandidateFilter_cfi import inconsistentMuonPFCandidateFilter

# filter events with no bad muon (of any type)

inconsistentMuons = inconsistentMuonPFCandidateFilter.clone()
greedyMuons = greedyMuonPFCandidateFilter.clone()

selectGoodPFEventsSequence = cms.Sequence(
    inconsistentMuons +
    greedyMuons  
    )

inconsistentMuonsTagging = inconsistentMuons.clone()
inconsistentMuonsTagging.taggingMode = True
greedyMuonsTagging = greedyMuons.clone()
greedyMuonsTagging.taggingMode = True
pfEventFilterTagging = pfEventFilter.clone()
pfEventFilterTagging.taggingMode = True

selectGoodPFEventsTaggingSequence = cms.Sequence(
    inconsistentMuonsTagging +
    greedyMuonsTagging
    # +
    # pfEventFilterTagging
    )



