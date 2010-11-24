import FWCore.ParameterSet.Config as cms

from CMGTools.Common.factories.cmgmuon_cfi import cmgMuon
from CMGTools.Common.histograms.muonLorentzVector_cfi import muonLorentzVector
from CMGTools.Common.histograms.muonHistograms_cfi import muonHistograms

muonSequence = cms.Sequence(
    cmgMuon +
    muonLorentzVector +
    muonHistograms
    )
