import FWCore.ParameterSet.Config as cms

from CMGTools.External.pujetidproducer_cfi import pileupJetIdProducer
from CMGTools.External.puJetIDAlgo_cff import PhilV1, PuJetIdOptMVA, PuJetIdMinMVA

puJetId = pileupJetIdProducer.clone(
    produceJetIds = cms.bool(True),
    jetids = cms.InputTag(""),
    runMvas = cms.bool(False),
    jets = cms.InputTag("selectedPatJets"),
    vertexes = cms.InputTag("offlinePrimaryVertices"),
    algos = cms.VPSet(PuJetIdMinMVA)
    )

puJetMva = pileupJetIdProducer.clone(
    produceJetIds = cms.bool(False),
    jetids = cms.InputTag("puJetId"),
    runMvas = cms.bool(True),
    jets = cms.InputTag("selectedPatJets"),
    vertexes = cms.InputTag("offlinePrimaryVertices"),
    algos = cms.VPSet(PuJetIdMinMVA,
                      PhilV1,
                      PuJetIdOptMVA)
    )

puJetIdSqeuence = cms.Sequence(puJetId*puJetMva)
