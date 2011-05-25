import FWCore.ParameterSet.Config as cms

vbfEventJetJet = cms.EDProducer(
    "VBFEventJetJetProducer",
    ForwardJets = cms.InputTag('selectedPatJetsAK5'),
    CentralObjects = cms.InputTag('selectedPatJetsAK5'),
    etaMargin = cms.double(0.5)
    )
