import FWCore.ParameterSet.Config as cms

from CMGTools.Common.factories.cmgPFCand_cfi import cmgPFCand
from CMGTools.Common.histograms.pfcandsLorentzVector_cfi import pfcandsLorentzVector

pfcandsSequence = cms.Sequence(
    cmgPFCand +
    pfcandsLorentzVector
    )
