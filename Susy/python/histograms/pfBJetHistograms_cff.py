import FWCore.ParameterSet.Config as cms

pfBJetHistograms = cms.EDAnalyzer(
    "PFBJetHistograms",
    inputCollection = cms.InputTag("cmgPFJetSel")
    )