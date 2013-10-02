import FWCore.ParameterSet.Config as cms

tauMuSVFit = cms.EDProducer(
    "TauMuWithSVFitProducer",
    diTauSrc = cms.InputTag("cmgTauMuCorPreSel"),
    # metsigSrc = cms.InputTag("pfMetSignificance"),
    # metsigSrc = cms.InputTag("mvaMETTauMu"),
    SVFitVersion =  cms.int32(2), # 1 for 2011 version , 2 for new 2012 (slow) version
    verbose = cms.untracked.bool( False )    
    )
