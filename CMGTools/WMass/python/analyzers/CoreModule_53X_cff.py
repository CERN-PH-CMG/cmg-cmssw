import copy
import os
import CMGTools.RootTools.fwlite.Config as cfg
from CMGTools.RootTools.fwlite.Config import printComps
from CMGTools.WMass.triggerMap import triggers_mu

jsonAna = cfg.Analyzer(
    'JSONAnalyzer',
    )

triggerAna = cfg.Analyzer(
     'triggerBitFilter',
     # verbose = True,
    )

vertexAna = cfg.Analyzer(
    'VertexAnalyzer',
    allVertices = 'slimmedPrimaryVertices',
    goodVertices = 'slimmedPrimaryVertices',
    vertexWeight = None,
    fixedWeight = 1,
    verbose = False,
    keepFailingEvents = False,
    )

WAna = cfg.Analyzer(
    'WAnalyzer',
    recoilcut = 1000,
    pfmetcut = 0,
    jetptcut = 1000,
    pt = 30,
    eta = 2.1,
    iso = 0.5,
    savegenp = True,
    verbose = True,
    triggerBits = {'SingleMu' : triggers_mu},
    keepFailingEvents = False,
    storeLHE_weight = False,
    doMad = True,
    )

WtreeProducer = cfg.Analyzer(
    'WTreeProducer',
    storeLHE_weight = False,
    additionalTKMet = False,
    superslimNtuples = False,
    )

ZAna = cfg.Analyzer(
    'ZAnalyzer',
    recoilcut = 1000,
    pfmetcut = 0,
    jetptcut = 1000,
    pt = 30,
    eta = 2.1,
    iso = 0.5,
    savegenp = True,
    verbose = True,
    triggerBits = {'SingleMu' : triggers_mu},
    # keepFailingEvents = True,
    keepFailingEvents = False,
    storeLHE_weight = False,
    # use_newWeights = True,
    doMad = True,
    )

ZtreeProducer = cfg.Analyzer(
    'ZTreeProducer',
    storeNeutralCMGcandidates = False,
    # storeCMGcandidates = True,
    storeLHE_weight = False,
    additionalTKMet = False,
    )

genAna = cfg.Analyzer(
    'GenParticleAnalyzerFSR',
    src = 'genParticlesPruned'
)

CoreWsequence = cfg.Sequence( [
    genAna,
    jsonAna,
    triggerAna,
    vertexAna,
    WAna,
    WtreeProducer,
    # ZAna,
    # ZtreeProducer,
   ] )

CoreZsequence = cfg.Sequence( [
    genAna,
    jsonAna,
    triggerAna,
    vertexAna,
    # WAna,
    # WtreeProducer,
    ZAna,
    ZtreeProducer,
   ] )

CoreWZsequence = cfg.Sequence( [
    genAna,
    jsonAna,
    triggerAna,
    vertexAna,
    WAna,
    WtreeProducer,
    ZAna,
    ZtreeProducer,
   ] )

CoreDATAsequence = cfg.Sequence( [
    jsonAna,
    triggerAna,
    vertexAna,
    WAna,
    WtreeProducer,
    ZAna,
    ZtreeProducer
    ] )

CoreDATAZsequence = cfg.Sequence( [
    jsonAna,
    triggerAna,
    vertexAna,
    # WAna,
    # WtreeProducer,
    ZAna,
    ZtreeProducer
    ] )
