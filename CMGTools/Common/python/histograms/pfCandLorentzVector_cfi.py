import FWCore.ParameterSet.Config as cms

from CMGTools.Common.histograms.cmgGenericPOHistograms_cfi import cmgGenericPOHistograms
pfCandidateLorentzVector = cmgGenericPOHistograms.clone(
    inputCollection = cms.InputTag("cmgPFCandidate")
    )
