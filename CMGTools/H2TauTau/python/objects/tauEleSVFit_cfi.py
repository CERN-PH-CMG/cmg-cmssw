import FWCore.ParameterSet.Config as cms

tauEleSVFit = cms.EDProducer(
    "TauEleWithSVFitProducer",
    diTauSrc = cms.InputTag("cmgTauEleCorPreSel"),
    # metSrc = cms.InputTag("cmgPFMET"),
    # metsigSrc = cms.InputTag("pfMetSignificance"),
    # metsigSrc = cms.InputTag("mvaMETTauEle"),
    SVFitVersion =  cms.int32(2), # 1 for 2011 version , 2 for new 2012 (slow) version
    verbose = cms.untracked.bool( False )    
    )
