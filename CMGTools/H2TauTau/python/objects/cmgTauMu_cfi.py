import FWCore.ParameterSet.Config as cms

cmgTauMu = cms.EDProducer(
    "TauMuPOProducer",
    leg1Collection=cms.InputTag('slimmedTaus'),
    leg2Collection=cms.InputTag('slimmedMuons'),
    metCollection=cms.InputTag('pfMVAMEt'),
    )
