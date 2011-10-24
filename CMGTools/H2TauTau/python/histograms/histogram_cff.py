import FWCore.ParameterSet.Config as cms

from CMGTools.Common.histograms.diTauLorentzVector_cfi import *

tauMuHistogram = diTauLorentzVector.clone( inputCollection='cmgTauMu') 


histogramSequence = cms.Sequence(
    tauMuHistogram
    )

