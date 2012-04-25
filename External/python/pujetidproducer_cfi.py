import FWCore.ParameterSet.Config as cms

from CMGTools.External.puJetIDAlgo_cff import PhilV1, full, simple

pileupJetIdProducer = cms.EDProducer('PileupJetIdProducer',
                         produceJetIds = cms.bool(True),
                         jetids = cms.InputTag(""),
                         runMvas = cms.bool(True),
                         jets = cms.InputTag("selectedPatJetsPFlow"),
                         vertexes = cms.InputTag("offlinePrimaryVertices"),
                         algos = cms.VPSet(full,simple,PhilV1)
)

