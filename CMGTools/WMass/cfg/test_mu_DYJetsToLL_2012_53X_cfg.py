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
    allVertices = 'offlinePrimaryVertices',
    goodVertices = 'offlinePrimaryVertices',
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
    keepFailingEvents = False
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
    savegenp = True,
    verbose = True,
    triggerBits = {'SingleMu' : triggers_mu},
    keepFailingEvents = False
    )

ZtreeProducer = cfg.Analyzer(
    'ZTreeProducer',
    storeNeutralCMGcandidates = False,
    # storeCMGcandidates = True,
    )

genAna = cfg.Analyzer(
'GenParticleAnalyzerFSR',
src = 'genParticlesPruned'
)

sequence = cfg.Sequence( [
    genAna,
    jsonAna,
    triggerAna,
    vertexAna,
    # WAna,
    # WtreeProducer,
    ZAna,
    ZtreeProducer,
   ] )

from CMGTools.H2TauTau.proto.samples.ewk import DYJets
from CMGTools.H2TauTau.proto.samples.getFiles import getFiles

# DYJets.files = getFiles('/DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola/Fall11-PU_S6_START42_V14B-v1/AODSIM/V5_B/PAT_CMG_V5_6_0_B', 'cmgtools', '.*root')
DYJets.files = getFiles('/DYJetsToLL_M-50_TuneZ2Star_8TeV-madgraph-tarball/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/PAT_CMG_V5_17_0', 'cmgtools', '.*root')
DYJets.triggers = triggers_mu


DYJets.splitFactor = 750

DYJets2 = copy.deepcopy(DYJets)

# selectedComponents = [DYJets,DYJets2]
# DYJets.files = DYJets.files[:1250]
# DYJets2.files = DYJets2.files[1250:]

# ONE AT THE TIME
selectedComponents = [DYJets]
# selectedComponents = [DYJets2]

# # TEST
# selectedComponents = [DYJets]
# DYJets.files = DYJets.files[:10]
# DYJets.splitFactor = 1


config = cfg.Config( components = selectedComponents,
                     sequence = sequence )
                     
printComps(config.components, True)


