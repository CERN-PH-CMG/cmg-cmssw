import FWCore.ParameterSet.Config as cms

from CMGTools.Common.histograms.cmgGenericPOHistograms_cfi import cmgGenericPOHistograms
electronLorentzVector = cmgGenericPOHistograms.clone(
                                                    inputCollection = cms.InputTag("cmgElectronSel")
                                                    )
