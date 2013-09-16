import FWCore.ParameterSet.Config as cms

patMuonsWithDirectionalIsolation = cms.EDProducer(
    "DirectionalIsolationProducerMuon",
    src = cms.InputTag('patMuonsWithMVA'),
    pfCands = cms.InputTag('pfNoPileUp'),
    vertexCollection = cms.InputTag('goodOfflinePrimaryVertices')
    )

patElectronsWithDirectionalIsolation = cms.EDProducer(
    "DirectionalIsolationProducerElectron",
    src = cms.InputTag('patElectronsWithMVA'),
    pfCands = cms.InputTag('pfNoPileUp'),
    vertexCollection = cms.InputTag('goodOfflinePrimaryVertices')
    )
