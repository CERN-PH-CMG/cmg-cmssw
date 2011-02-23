import FWCore.ParameterSet.Config as cms

vbfEventGenJetGenJet = cms.EDProducer(
    "VBFEventGenJetGenJetProducer",
    ForwardJets = cms.InputTag('ak5GenJets'),
    CentralObjects = cms.InputTag('ak5GenJets'),
    etaMargin = cms.double(0.5)
    )
