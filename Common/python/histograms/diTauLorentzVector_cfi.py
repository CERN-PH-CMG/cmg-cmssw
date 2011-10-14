import FWCore.ParameterSet.Config as cms

from CMGTools.Common.histograms.cmgGenericPOHistograms_cfi import cmgGenericPOHistograms
diTauLorentzVector = cmgGenericPOHistograms.clone(
    inputCollection = cms.InputTag("cmgDiTauSel")
    )
