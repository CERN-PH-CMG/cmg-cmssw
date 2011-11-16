import FWCore.ParameterSet.Config as cms

tauMuSVFit = cms.EDProducer(
    "DiTauWithSVFitProducer",
    diTauSrc = cms.InputTag("cmgTauMuCorPreSel"),
    # metSrc = cms.InputTag("cmgPFMET"),
    metsigSrc = cms.InputTag("PFMETSignificanceAK5"),
    verbose = cms.untracked.bool( False )
    )
