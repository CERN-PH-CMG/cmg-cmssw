import FWCore.ParameterSet.Config as cms

from CMGTools.Common.factories.cmgPFCand_cfi import cmgPFCand
from CMGTools.Common.histograms.pfCandLorentzVector_cfi import pfCandLorentzVector

pfcandsSequence = cms.Sequence(
    cmgPFCand +
    pfCandLorentzVector
    )
