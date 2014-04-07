import FWCore.ParameterSet.Config as cms

patElectronsWithDirectionalIsolation = cms.EDProducer(
    "DirectionalIsolationProducerElectron",
    src = cms.InputTag('selectedPatElectrons'),
    pfCands = cms.InputTag('pfNoPileUp'),
    vertexCollection = cms.InputTag('goodOfflinePrimaryVertices')
    )
