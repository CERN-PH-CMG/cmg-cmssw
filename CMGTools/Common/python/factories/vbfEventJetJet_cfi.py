import FWCore.ParameterSet.Config as cms

vbfEventJetJet = cms.EDProducer(
    "VBFEventJetJetProducer",
    ForwardJets = cms.InputTag('selectedPatJetsPFlow'),
    CentralObjects = cms.InputTag('selectedPatJetsPFlow'),
    etaMargin = cms.double(0.5)
    )
