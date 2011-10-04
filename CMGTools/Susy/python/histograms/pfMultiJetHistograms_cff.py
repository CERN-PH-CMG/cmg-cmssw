import FWCore.ParameterSet.Config as cms

pfMultiJetHistograms = cms.EDAnalyzer(
    "PFMultiJetHistograms",
    inputCollection = cms.InputTag("cmgPFJetSel")
    )