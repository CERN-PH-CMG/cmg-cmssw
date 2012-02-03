import FWCore.ParameterSet.Config as cms

muEleSVFit = cms.EDProducer(
    "MuEleWithSVFitProducer",
    diTauSrc = cms.InputTag("cmgMuEleCorPreSel"),
    metsigSrc = cms.InputTag("PFMETSignificanceAK5"),
    verbose = cms.untracked.bool( False )
    )
