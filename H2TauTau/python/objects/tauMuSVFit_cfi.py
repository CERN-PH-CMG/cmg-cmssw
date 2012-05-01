import FWCore.ParameterSet.Config as cms

tauMuSVFit = cms.EDProducer(
    "TauMuWithSVFitProducer",
    diTauSrc = cms.InputTag("cmgTauMuCorPreSel"),
    # metSrc = cms.InputTag("cmgPFMET"),
    metsigSrc = cms.InputTag("PFMETSignificance"),
    verbose = cms.untracked.bool( False )
    )
