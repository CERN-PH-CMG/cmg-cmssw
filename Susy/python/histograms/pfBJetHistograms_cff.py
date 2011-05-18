import FWCore.ParameterSet.Config as cms

pfJetHistograms = cms.EDAnalyzer(
    "PFBJetHistograms",
    inputCollection = cms.InputTag("cmgPFJetSel")
    )