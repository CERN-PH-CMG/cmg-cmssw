import FWCore.ParameterSet.Config as cms

from CMGTools.Common.histograms.cmgGenericPOHistograms_cfi import cmgGenericPOHistograms
pfJetLorentzVector = cmgGenericPOHistograms.clone(
                                                    inputCollection = cms.InputTag("cmgPFJetSel")
                                                    )
