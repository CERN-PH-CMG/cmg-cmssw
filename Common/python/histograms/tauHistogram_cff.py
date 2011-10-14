import FWCore.ParameterSet.Config as cms

# tau histograms

from CMGTools.Common.histograms.cmgTauHistograms_cfi import *
from CMGTools.Common.histograms.tauLorentzVector_cfi import *

tauHistogramSequence = cms.Sequence(
    cmgTauHistograms +
    tauLorentzVector # +
    )
