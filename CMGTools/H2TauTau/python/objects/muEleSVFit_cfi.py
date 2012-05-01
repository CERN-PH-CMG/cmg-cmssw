import FWCore.ParameterSet.Config as cms

muEleSVFit = cms.EDProducer(
    "MuEleWithSVFitProducer",
    diTauSrc = cms.InputTag("cmgMuEleCorPreSel"),
    metsigSrc = cms.InputTag("PFMETSignificance"),
    verbose = cms.untracked.bool( False )
    )
