import FWCore.ParameterSet.Config as cms

cmgTauMu = cms.EDProducer(
    "TauMuPOProducer",
    leg1Collection=cms.InputTag('tauPreSelection'),
    leg2Collection=cms.InputTag('muonPreSelection'),
    metCollection=cms.InputTag('pfMVAMEt'),
    )
