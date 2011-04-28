import FWCore.ParameterSet.Config as cms

from CMGTools.Common.histograms.cmgGenericPOHistograms_cfi import cmgGenericPOHistograms
ditauLorentzVector = cmgGenericPOHistograms.clone(inputCollection = cms.InputTag("cmgDiTauSel"))
