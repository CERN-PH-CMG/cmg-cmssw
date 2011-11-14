import FWCore.ParameterSet.Config as cms

tauMuSVFit = cms.EDProducer(
    "DiTauWithSVFitProducer",
    diTauSrc = cms.InputTag("cmgTauMuCorPreSel"),
    verbose = cms.untracked.bool( False )
    )
