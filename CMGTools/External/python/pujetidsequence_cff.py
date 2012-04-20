import FWCore.ParameterSet.Config as cms

from CMGTools.External.pujetidproducer_cfi import pileupJetIdProducer
from CMGTools.External.puJetIDAlgo_cff import PhilV1, PuJetIdOptMVA, PuJetIdMinMVA

puJetId = pileupJetIdProducer.clone(
    produceJetIds = cms.bool(True),
    jetids = cms.InputTag(""),
    runMvas = cms.bool(False),
    jets = cms.InputTag("selectedPatJetsPFlow"),
    vertexes = cms.InputTag("offlinePrimaryVertices"),
    algos = cms.VPSet(PuJetIdMinMVA)
    )

puJetMva = pileupJetIdProducer.clone(
    produceJetIds = cms.bool(False),
    jetids = cms.InputTag("puJetId"),
    runMvas = cms.bool(True),
    jets = cms.InputTag("selectedPatJetsPFlow"),
    vertexes = cms.InputTag("offlinePrimaryVertices"),
    algos = cms.VPSet(PuJetIdMinMVA,
                      PuJetIdOptMVA,
                      PhilV1)
    )

puJetIdSqeuence = cms.Sequence(puJetId*puJetMva)
