import FWCore.ParameterSet.Config as cms

diTauSVFit = cms.EDProducer(
    "TauTauWithSVFitProducer",
    diTauSrc = cms.InputTag("cmgDiTauCorPreSel"),
    # metSrc = cms.InputTag("cmgPFMET"),
    metsigSrc = cms.InputTag("pfMetSignificance"),
    verbose = cms.untracked.bool( False )
    )
