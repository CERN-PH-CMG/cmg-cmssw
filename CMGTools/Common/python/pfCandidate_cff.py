import FWCore.ParameterSet.Config as cms

from CMGTools.Common.factories.cmgPFCandidate_cfi import cmgPFCandidate
from CMGTools.Common.histograms.pfCandLorentzVector_cfi import pfCandidateLorentzVector

pfcandsSequence = cms.Sequence(
    cmgPFCandidate +
    pfCandidateLorentzVector
    )
