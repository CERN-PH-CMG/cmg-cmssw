import FWCore.ParameterSet.Config as cms

from CMGTools.Common.histograms.cmgGenericPOHistograms_cfi import cmgGenericPOHistograms
pfCandLorentzVector = cmgGenericPOHistograms.clone(
                                                    inputCollection = cms.InputTag("cmgPFCand")
                                                    )
