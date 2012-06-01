import FWCore.ParameterSet.Config as cms

diTauSVFit = cms.EDProducer(
    "TauTauWithSVFitProducer",
    diTauSrc = cms.InputTag("cmgDiTauCorPreSel"),
    # metSrc = cms.InputTag("cmgPFMET"),
    #metsigSrc = cms.InputTag("pfMetSignificance"),
    metsigSrc = cms.InputTag("mvaMETDiTau"),
    SVFitVersion =  cms.int32(1), # 1 for 2011 version , 2 for new 2012 (slow) version
    verbose = cms.untracked.bool( False )
    )
