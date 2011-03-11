import FWCore.ParameterSet.Config as cms

from CMGTools.Common.histograms.cmgGenericPOHistograms_cfi import cmgGenericPOHistograms

wenuLorentzVector = cmgGenericPOHistograms.clone(
                                                    inputCollection = cms.InputTag("cmgWENuSel")
                                                    )

wmunuLorentzVector = cmgGenericPOHistograms.clone(
                                                    inputCollection = cms.InputTag("cmgWMuNuSel")
                                                    )
