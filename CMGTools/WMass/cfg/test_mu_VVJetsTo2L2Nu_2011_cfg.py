import copy
import os 
import CMGTools.RootTools.fwlite.Config as cfg
from CMGTools.RootTools.fwlite.Config import printComps
from CMGTools.WMass.triggerMap import pathsAndFilters

jsonAna = cfg.Analyzer(
    'JSONAnalyzer',
    )

triggerAna = cfg.Analyzer(
    'TriggerAnalyzer',
     keepFailingEvents = False        
    )

vertexAna = cfg.Analyzer(
    'VertexAnalyzer',
    fixedWeight = 1.,
     keepFailingEvents = False        
    )

WAna = cfg.Analyzer(
    'WAnalyzer',
    recoilcut = 1000,
    pfmetcut = 0,
    jetptcut = 1000,
    pt = 30,
    eta = 2.1,
    iso = 0.5,
    savegenp = False,
    verbose = True,
    triggerMap = pathsAndFilters
    )

WtreeProducer = cfg.Analyzer(
    'WTreeProducer'
    )

ZAna = cfg.Analyzer(
    'ZAnalyzer',
    recoilcut = 1000,
    pfmetcut = 0,
    jetptcut = 1000,
    pt = 30,
    eta = 2.1,
    iso = 0.5,
    savegenp = False,
    verbose = True,
    triggerMap = pathsAndFilters
    )

ZtreeProducer = cfg.Analyzer(
    'ZTreeProducer'
    )

sequence = cfg.Sequence( [
    jsonAna,
    triggerAna,
    vertexAna,
    WAna,
    WtreeProducer,
    ZAna,
    ZtreeProducer
   ] )

from CMGTools.H2TauTau.proto.samples.diboson import WWJetsTo2L2Nu, WZJetsTo2L2Q, WZJetsTo3LNu, ZZJetsTo2L2Nu, ZZJetsTo2L2Q, ZZJetsTo4L
from CMGTools.H2TauTau.proto.samples.getFiles import getFiles

WWJetsTo2L2Nu.files = getFiles('/WWJetsTo2L2Nu_TuneZ2_7TeV-madgraph-tauola/Fall11-PU_S6_START42_V14B-v1/AODSIM/PAT_CMG_V5_6_0_B', 'cmgtools', '.*root')
WWJetsTo2L2Nu.triggers = ["HLT_IsoMu24_v1","HLT_IsoMu24_v2","HLT_IsoMu24_v3","HLT_IsoMu24_v4","HLT_IsoMu24_v5","HLT_IsoMu24_v6","HLT_IsoMu24_v7",\
                   "HLT_IsoMu24_v8","HLT_IsoMu24_v9","HLT_IsoMu24_v10","HLT_IsoMu24_v11","HLT_IsoMu24_v12","HLT_IsoMu24_v13","HLT_IsoMu24_v14",\
                   "HLT_IsoMu24_eta2p1_v1","HLT_IsoMu24_eta2p1_v2","HLT_IsoMu24_eta2p1_v3","HLT_IsoMu24_eta2p1_v4","HLT_IsoMu24_eta2p1_v5",\
                   "HLT_IsoMu24_eta2p1_v6","HLT_IsoMu24_eta2p1_v7","HLT_IsoMu24_eta2p1_v8"
                   ]

WZJetsTo3LNu.files = getFiles('/WZJetsTo3LNu_TuneZ2_7TeV-madgraph-tauola/Fall11-PU_S6_START42_V14B-v1/AODSIM/PAT_CMG_V5_6_0_B', 'cmgtools', '.*root')
WZJetsTo3LNu.triggers = ["HLT_IsoMu24_v1","HLT_IsoMu24_v2","HLT_IsoMu24_v3","HLT_IsoMu24_v4","HLT_IsoMu24_v5","HLT_IsoMu24_v6","HLT_IsoMu24_v7",\
                   "HLT_IsoMu24_v8","HLT_IsoMu24_v9","HLT_IsoMu24_v10","HLT_IsoMu24_v11","HLT_IsoMu24_v12","HLT_IsoMu24_v13","HLT_IsoMu24_v14",\
                   "HLT_IsoMu24_eta2p1_v1","HLT_IsoMu24_eta2p1_v2","HLT_IsoMu24_eta2p1_v3","HLT_IsoMu24_eta2p1_v4","HLT_IsoMu24_eta2p1_v5",\
                   "HLT_IsoMu24_eta2p1_v6","HLT_IsoMu24_eta2p1_v7","HLT_IsoMu24_eta2p1_v8"
                   ]

ZZJetsTo2L2Nu.files = getFiles('/ZZJetsTo2L2Nu_TuneZ2_7TeV-madgraph-tauola/Fall11-PU_S6_START42_V14B-v1/AODSIM/V5_B/PAT_CMG_V5_6_0_B', 'cmgtools', '.*root')
ZZJetsTo2L2Nu.triggers = ["HLT_IsoMu24_v1","HLT_IsoMu24_v2","HLT_IsoMu24_v3","HLT_IsoMu24_v4","HLT_IsoMu24_v5","HLT_IsoMu24_v6","HLT_IsoMu24_v7",\
                   "HLT_IsoMu24_v8","HLT_IsoMu24_v9","HLT_IsoMu24_v10","HLT_IsoMu24_v11","HLT_IsoMu24_v12","HLT_IsoMu24_v13","HLT_IsoMu24_v14",\
                   "HLT_IsoMu24_eta2p1_v1","HLT_IsoMu24_eta2p1_v2","HLT_IsoMu24_eta2p1_v3","HLT_IsoMu24_eta2p1_v4","HLT_IsoMu24_eta2p1_v5",\
                   "HLT_IsoMu24_eta2p1_v6","HLT_IsoMu24_eta2p1_v7","HLT_IsoMu24_eta2p1_v8"
                   ]

selectedComponents = [ZZJetsTo2L2Nu,WZJetsTo3LNu,WWJetsTo2L2Nu]

ZZJetsTo2L2Nu.splitFactor = 750
WZJetsTo3LNu.splitFactor = 750
WWJetsTo2L2Nu.splitFactor = 750

config = cfg.Config( components = selectedComponents,
                     sequence = sequence )

printComps(config.components, True)


