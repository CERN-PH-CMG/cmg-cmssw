import copy
import os 
import CMGTools.RootTools.fwlite.Config as cfg
from CMGTools.RootTools.fwlite.Config import printComps


jsonAna = cfg.Analyzer(
    'JSONAnalyzer',
    )

triggerAna = cfg.Analyzer(
    'TriggerAnalyzer'
    )

vertexAna = cfg.Analyzer(
    'VertexAnalyzer',
    fixedWeight = 1.
    )

WAna = cfg.Analyzer(
    'WAnalyzer',
    # recoilcut = 20,
    # pfmetcut = 25,
    # jetptcut = 30,
    recoilcut = 1000,
    pfmetcut = 0,
    jetptcut = 1000,
    pt = 30,
    eta = 2.1,
    iso = 0.5,
    savegenp = True,
    verbose = True
    )

WtreeProducer = cfg.Analyzer(
    'WTreeProducer'
    )

ZAna = cfg.Analyzer(
    'ZAnalyzer',
    # recoilcut = 25, # rised from 20 to 25 to allow offline MZ/MW scaled analysis cut
    # pfmetcut = 25,
    # jetptcut = 30,
    recoilcut = 1000,
    pfmetcut = 0,
    jetptcut = 1000,
    pt = 30,
    eta = 2.1,
    iso = 0.5,
    savegenp = True,
    verbose = True
    )

ZtreeProducer = cfg.Analyzer(
    'ZTreeProducer'
    )

sequence = cfg.Sequence( [
    jsonAna,
    triggerAna,
    vertexAna,
    WAna,
    # WtreeProducer,
    ZAna,
    # ZtreeProducer
   ] )


from CMGTools.H2TauTau.proto.samples.ewk import WJets, DYJets, TTJets
from CMGTools.H2TauTau.proto.samples.diboson import WWJetsTo2L2Nu, WZJetsTo2L2Q, WZJetsTo3LNu, ZZJetsTo2L2Nu, ZZJetsTo2L2Q, ZZJetsTo4L
from CMGTools.H2TauTau.proto.samples.getFiles import getFiles

DYJets.files = getFiles('/DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola/Fall11-PU_S6_START42_V14B-v1/AODSIM/V5_B/PAT_CMG_V5_6_0_B', 'cmgtools', '.*root')
# DYJets.files = DYJets.files[:10]
DYJets.triggers = ["HLT_IsoMu24_v*"]

WJets.files = getFiles('/WJetsToLNu_TuneZ2_7TeV-madgraph-tauola/Fall11-PU_S6_START42_V14B-v1/AODSIM/V5_B/PAT_CMG_V5_6_0_B', 'cmgtools', '.*root')
# WJets.files = WJets.files[:10]
WJets.triggers = ["HLT_IsoMu24_v*"]

TTJets.files = getFiles('/TTJets_TuneZ2_7TeV-madgraph-tauola/Fall11-PU_S6_START42_V14B-v2/AODSIM/V5_B/PAT_CMG_V5_6_0_B', 'cmgtools', '.*root')
TTJets.triggers = ["HLT_IsoMu24_v*"]

WWJetsTo2L2Nu.files = getFiles('/WWJetsTo2L2Nu_TuneZ2_7TeV-madgraph-tauola/Fall11-PU_S6_START42_V14B-v1/AODSIM/PAT_CMG_V5_6_0_B', 'cmgtools', '.*root')
WWJetsTo2L2Nu.triggers = ["HLT_IsoMu24_v*"]

# WZJetsTo2L2Q.files = getFiles('/WZJetsTo2L2Q_TuneZ2_7TeV-madgraph-tauola/Fall11-PU_S6_START42_V14B-v1/AODSIM/V5_B/PAT_CMG_V5_6_0_B', 'cmgtools', '.*root')
# WZJetsTo2L2Q.triggers = ["HLT_IsoMu24_v*"]

WZJetsTo3LNu.files = getFiles('/WZJetsTo3LNu_TuneZ2_7TeV-madgraph-tauola/Fall11-PU_S6_START42_V14B-v1/AODSIM/PAT_CMG_V5_6_0_B', 'cmgtools', '.*root')
WZJetsTo3LNu.triggers = ["HLT_IsoMu24_v*"]

ZZJetsTo2L2Nu.files = getFiles('/ZZJetsTo2L2Nu_TuneZ2_7TeV-madgraph-tauola/Fall11-PU_S6_START42_V14B-v1/AODSIM/V5_B/PAT_CMG_V5_6_0_B', 'cmgtools', '.*root')
ZZJetsTo2L2Nu.triggers = ["HLT_IsoMu24_v*"]

# ZZJetsTo2L2Q.files = getFiles('/WWJetsTo2L2Nu_TuneZ2_7TeV-madgraph-tauola/Fall11-PU_S6_START42_V14B-v1/AODSIM/V5_B/PAT_CMG_V5_6_0_B', 'cmgtools', '.*root')
# ZZJetsTo2L2Q.triggers = ["HLT_IsoMu24_v*"]

# ZZJetsTo4L.files = getFiles('/WWJetsTo2L2Nu_TuneZ2_7TeV-madgraph-tauola/Fall11-PU_S6_START42_V14B-v1/AODSIM/V5_B/PAT_CMG_V5_6_0_B', 'cmgtools', '.*root')
# ZZJetsTo4L.triggers = ["HLT_IsoMu24_v*"]

# selectedComponents = [DYJets, WJets]
# selectedComponents = [WJets]
# selectedComponents = [DYJets]
# selectedComponents = [TTJets]
selectedComponents = [ZZJetsTo2L2Nu,WZJetsTo3LNu,WWJetsTo2L2Nu]


ZZJetsTo2L2Nu.splitFactor = 750
WZJetsTo3LNu.splitFactor = 750
WWJetsTo2L2Nu.splitFactor = 750
TTJets.splitFactor = 750
DYJets.splitFactor = 750
WJets.splitFactor = 750

config = cfg.Config( components = selectedComponents,
                     sequence = sequence )

                     
printComps(config.components, True)


