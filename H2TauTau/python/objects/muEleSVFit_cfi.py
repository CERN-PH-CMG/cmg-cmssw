import FWCore.ParameterSet.Config as cms

muEleSVFit = cms.EDProducer(
    "MuEleWithSVFitProducer",
    diTauSrc = cms.InputTag("cmgMuEleCorPreSel"),
    metsigSrc = cms.InputTag("pfMetSignificance"),
    verbose = cms.untracked.bool( False )
    )
