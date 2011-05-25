import FWCore.ParameterSet.Config as cms

# muon histograms

from CMGTools.Common.histograms.cmgMuonHistograms_cfi import *
from CMGTools.Common.histograms.muonLorentzVector_cfi import *
from CMGTools.Common.histograms.cmgDiMuonHistograms_cfi import *

muonHistogramSequence = cms.Sequence(
    cmgMuonHistograms +
    muonLorentzVector +
    cmgDiMuonHistograms
    )
