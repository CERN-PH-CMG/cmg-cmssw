import FWCore.ParameterSet.Config as cms

tauMuSVFit = cms.EDProducer(
    "DiTauWithSVFitProducer",
    diTauSrc = cms.InputTag("cmgTauMuCorSel"),
    verbose = cms.untracked.bool( False )
    )
