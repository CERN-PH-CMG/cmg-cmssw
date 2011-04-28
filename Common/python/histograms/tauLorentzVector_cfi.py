import FWCore.ParameterSet.Config as cms

from CMGTools.Common.histograms.cmgGenericPOHistograms_cfi import cmgGenericPOHistograms
tauLorentzVector = cmgGenericPOHistograms.clone(
    inputCollection = cms.InputTag("cmgTauSel")
    )
