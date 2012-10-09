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
    pt = 20,
    eta = 2.4,
    iso = 0.5,
    verbose = True
    )

treeProducer = cfg.Analyzer(
    'WTreeProducer'
    )

sequence = cfg.Sequence( [
    jsonAna,
    triggerAna,
    vertexAna,
    WAna,
    treeProducer
   ] )


from CMGTools.H2TauTau.proto.samples.ewk import WJets, DYJets
from CMGTools.H2TauTau.proto.samples.getFiles import getFiles

DYJets.files = getFiles('/DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola/Fall11-PU_S6_START42_V14B-v1/AODSIM/V5_B/PAT_CMG_V5_6_0_B', 'cmgtools', '.*root')
WJets.files = getFiles('/WJetsToLNu_TuneZ2_7TeV-madgraph-tauola/Fall11-PU_S6_START42_V14B-v1/AODSIM/V5_B/PAT_CMG_V5_6_0_B', 'cmgtools', '.*root')

selectedComponents = [DYJets, WJets]


DYJets.splitFactor = 5
WJets.splitFactor = 5

config = cfg.Config( components = selectedComponents,
                     sequence = sequence )

printComps(config.components, True)
