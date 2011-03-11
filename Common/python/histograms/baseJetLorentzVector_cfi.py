import FWCore.ParameterSet.Config as cms

from CMGTools.Common.histograms.cmgGenericPOHistograms_cfi import cmgGenericPOHistograms
baseJetLorentzVector = cmgGenericPOHistograms.clone(
                                                    inputCollection = cms.InputTag("cmgBaseJet")
                                                    )
