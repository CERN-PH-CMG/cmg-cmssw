import FWCore.ParameterSet.Config as cms

from CMGTools.Common.histograms.diTauLorentzVector_cfi import *

tauMuHistogramSkim = diTauLorentzVector.clone( inputCollection='cmgTauMuCorPreSel') 

histogramSkimSequence = cms.Sequence(
    tauMuHistogramSkim
    )

