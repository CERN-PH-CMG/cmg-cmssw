import FWCore.ParameterSet.Config as cms

from CMGTools.Common.histograms.diTauLorentzVector_cfi import *

tauMuHistogramSkim = diTauLorentzVector.clone( inputCollection='cmgTauMuSel') 

histogramSkimSequence = cms.Sequence(
    tauMuHistogramSkim
    )

