import FWCore.ParameterSet.Config as cms

tauEleSVFit = cms.EDProducer(
    "TauEleWithSVFitProducer",
    diTauSrc = cms.InputTag("cmgTauEleCorPreSel"),
    # metSrc = cms.InputTag("cmgPFMET"),
    metsigSrc = cms.InputTag("pfMetSignificance"),
    verbose = cms.untracked.bool( False )
    )
