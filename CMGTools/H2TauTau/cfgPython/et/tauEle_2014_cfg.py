import PhysicsTools.HeppyCore.framework.config as cfg
from PhysicsTools.HeppyCore.framework.config import printComps
# from CMGTools.RootTools.fwlite.Config import printComps

# import all analysers:

# Heppy analyzers
from PhysicsTools.Heppy.analyzers.core.JSONAnalyzer import JSONAnalyzer
from PhysicsTools.Heppy.analyzers.core.EventSelector import EventSelector
# from PhysicsTools.Heppy.analyzers.examples.TriggerAnalyzer import TriggerAnalyzer
from PhysicsTools.Heppy.analyzers.objects.VertexAnalyzer import VertexAnalyzer
from PhysicsTools.Heppy.analyzers.core.PileUpAnalyzer import PileUpAnalyzer

# Tau-tau analyzers
from CMGTools.H2TauTau.proto.analyzers.JetAnalyzer import JetAnalyzer
from CMGTools.H2TauTau.proto.analyzers.EmbedWeighter import EmbedWeighter
from CMGTools.H2TauTau.proto.analyzers.GenErsatzAnalyzer import GenErsatzAnalyzer
from CMGTools.H2TauTau.proto.analyzers.TauEleAnalyzer import TauEleAnalyzer
from CMGTools.H2TauTau.proto.analyzers.DYJetsFakeAnalyzer import DYJetsFakeAnalyzer
from CMGTools.H2TauTau.proto.analyzers.WNJetsAnalyzer import WNJetsAnalyzer
from CMGTools.H2TauTau.proto.analyzers.NJetsAnalyzer import NJetsAnalyzer
from CMGTools.H2TauTau.proto.analyzers.HiggsPtWeighter import HiggsPtWeighter
from CMGTools.H2TauTau.proto.analyzers.WNJetsTreeAnalyzer import WNJetsTreeAnalyzer
from CMGTools.H2TauTau.proto.analyzers.DYLLReweighterTauEle import DYLLReweighterTauEle
from CMGTools.H2TauTau.proto.analyzers.TauDecayModeWeighter import TauDecayModeWeighter
from CMGTools.H2TauTau.proto.analyzers.TauFakeRateWeighter import TauFakeRateWeighter
from CMGTools.H2TauTau.proto.analyzers.LeptonWeighter import LeptonWeighter
from CMGTools.H2TauTau.proto.analyzers.H2TauTauTreeProducerTauEle import H2TauTauTreeProducerTauEle
from CMGTools.H2TauTau.proto.analyzers.H2TauTauSyncTree import H2TauTauSyncTree

from CMGTools.RootTools.analyzers.VBFSimpleAnalyzer import VBFSimpleAnalyzer


# from CMGTools.Production.getFiles import getFiles
# from CMGTools.H2TauTau.triggerMap import pathsAndFilters
# from CMGTools.H2TauTau.proto.weights.weighttable import ele_id_tauele_2012, ele_iso_tauele_2012
# from CMGTools.H2TauTau.proto.samples.sampleShift import selectShift


# 'Nom', 'Up', 'Down', or None
shift = None
# 1.0, 1.03, 0.97
tauScaleShift = 1.0

syncntuple = False

# Andrew Summer 13 (MC is identical to the previous one)
# puFileMC = '/afs/cern.ch/user/a/agilbert/public/HTT_Pileup/13-09-13/MC_Summer12_PU_S10-600bins.root'
# puFileData = '/afs/cern.ch/user/a/agilbert/public/HTT_Pileup/13-09-13/Data_Pileup_2012_ReRecoPixel-600bins.root'

puFileMC = None
puFileData = None

# hlt_tauEffWeight_mc = 'effTau_eTau_MC_2012ABCDSummer13'
# hlt_tauEffWeight = 'effTau_eTau_Data_2012ABCDSummer13'
# hlt_eleEffWeight_mc = 'effEle_eTau_MC_2012ABCD'
# hlt_eleEffWeight = 'effEle_eTau_Data_2012ABCDSummer13'

hlt_tauEffWeight_mc = None
hlt_tauEffWeight = None
hlt_eleEffWeight_mc = None
hlt_eleEffWeight = None
    
    
eventSelector = cfg.Analyzer(
    EventSelector,
    name='EventSelector',
    toSelect=[
    105104
    ]
    )


jsonAna = cfg.Analyzer(
    JSONAnalyzer,
    name='JSONAnalyzer',
    )

# triggerAna = cfg.Analyzer(
#     TriggerAnalyzer,
#     'TriggerAnalyzer'
#     )

vertexAna = cfg.Analyzer(
    VertexAnalyzer,
    name='VertexAnalyzer',
    goodVertices='goodPVFilter',
    vertexWeight=None,
    fixedWeight=1,
    verbose=False,
    )

embedWeighter = cfg.Analyzer(
    EmbedWeighter,
    name='EmbedWeighter',
    isRecHit=False,
    verbose=False
    )

pileUpAna = cfg.Analyzer(
    PileUpAnalyzer,
    'PileUpAnalyzer',
    true = True
    )

genErsatzAna = cfg.Analyzer(
    GenErsatzAnalyzer,
    'GenErsatzAnalyzer',
    verbose=False
    )

TauEleAna = cfg.Analyzer(
    TauEleAnalyzer,
    'TauEleAnalyzer',
    scaleShift1 = tauScaleShift,
    pt1 = 20,
    eta1 = 2.3,
    iso1 = None,
    pt2 = 24,
    eta2 = 2.1,
    iso2 = 0.1,
    m_min = 10,
    m_max = 99999,
    # triggerMap = pathsAndFilters,
    verbose = False
    )

dyJetsFakeAna = cfg.Analyzer(
    DYJetsFakeAnalyzer,
    'DYJetsFakeAnalyzer',
    leptonType = 13,
    src = 'prunedGenParticles',
    )

dyLLReweighterTauEle = cfg.Analyzer(
    DYLLReweighterTauEle,
    'DYLLReweighterTauEle',
    # 2012 
    W1p0PB = 1., #1.37, # weight for 1 prong 0 Pi Barrel
    W1p0PE = 1., #1.11,
    W1p1PB = 1., #2.18,
    W1p1PE = 1., #0.47,
    verbose = False
    )

WNJetsAna = cfg.Analyzer(
    WNJetsAnalyzer,
    'WNJetsAnalyzer',
    verbose = False
    )

NJetsAna = cfg.Analyzer(
    NJetsAnalyzer,
    'NJetsAnalyzer',
    fillTree = True,
    verbose = False
    )

WNJetsTreeAna = cfg.Analyzer(
    WNJetsTreeAnalyzer,
    'WNJetsTreeAnalyzer'
    )

higgsWeighter = cfg.Analyzer(
    HiggsPtWeighter,
    'HiggsPtWeighter',
    src = 'genParticlesPruned',
    )

tauDecayModeWeighter = cfg.Analyzer(
    TauDecayModeWeighter,
    'TauDecayModeWeighter',
    )

tauFakeRateWeighter = cfg.Analyzer(
    TauFakeRateWeighter,
    'TauFakeRateWeighter'
    )

tauWeighter = cfg.Analyzer(
    LeptonWeighter,
    'LeptonWeighter_tau',
    effWeight = None,
    effWeightMC = None,
    lepton = 'leg1',
    verbose = False,
    disable = True,
    )

eleWeighter = cfg.Analyzer(
    LeptonWeighter,
    'LeptonWeighter_ele',
    effWeight = None,
    effWeightMC = None,
    lepton = 'leg2',
    verbose = False,
    disable = True,
    idWeight = None,
    isoWeight = None    
    )

jetAna = cfg.Analyzer(
    JetAnalyzer,
    'JetAnalyzer',
    jetCol = 'slimmedJets', # <- These are CHS jets
    # jetCol = 'patJetsAK4PF', # <- These are plain PF jets
    jetPt = 20.,
    jetEta = 4.7,
    btagSFseed = 123456,
    relaxJetId = False, 
    jerCorr = False,
    puJetIDDisc = 'pileupJetIdFull:full53xDiscriminant'
    #jesCorr = 1.,
    )

vbfSimpleAna = cfg.Analyzer(
    VBFSimpleAnalyzer,
    'VBFSimpleAnalyzer',
    vbfMvaWeights = '',
    cjvPtCut = 30.,
    Mjj = 500.,
    deltaEta = 3.5
    )

treeProducer = cfg.Analyzer(
    H2TauTauTreeProducerTauEle,
    'H2TauTauTreeProducerTauEle'
    )

treeProducerXCheck = cfg.Analyzer(
    H2TauTauSyncTree,
    'H2TauTauSyncTree',
    pt20 = False
    )

#########################################################################################

from CMGTools.H2TauTau.proto.samples.csa2014.tauMu_Sync_Jan import MC_list, mc_higgs, mc_dict

#########################################################################################

# Use the following two lines for local testing
# HiggsVBF125.files = ['file:/afs/cern.ch/user/s/steggema/work/CMSSW_7_0_6_patch1/src/CMGTools/H2TauTau/prod/tauMu_fullsel_tree_CMG.root']
# HiggsVBF125.splitFactor = 1


for mc in MC_list:
    mc.puFileMC = puFileMC
    mc.puFileData = puFileData


selectedComponents = [mc_dict['HiggsTTHInclusive125']]

sequence = cfg.Sequence( [
    # eventSelector,
    jsonAna, 
    # triggerAna,
    vertexAna, 
    TauEleAna,
    dyJetsFakeAna,
    dyLLReweighterTauEle,
    # WNJetsAna,
    # WNJetsTreeAna,
    NJetsAna,
    higgsWeighter, 
    jetAna,
    vbfSimpleAna,
    pileUpAna,
    embedWeighter,
    tauDecayModeWeighter,
    tauFakeRateWeighter,
    tauWeighter, 
    eleWeighter, 
    treeProducer,
    # treeProducerXCheck
   ] )

if syncntuple:
    sequence.append( treeProducerXCheck) #Yes!

# selectedComponents = [comp for comp in selectedComponents if comp.dataset_entries > 0]

mc_dict['HiggsTTHInclusive125'].files = ['/afs/cern.ch/user/s/steggema/work/CMSSW_7_2_3/src/CMGTools/H2TauTau/prod/tauEle_fullsel_tree_CMG.root']

test = 1
if test==1:
    comp = mc_dict['HiggsTTHInclusive125']
    # comp = data_Run2012A
    selectedComponents = [comp]
    comp.splitFactor = 1
    # comp.files = comp.files[:10]
    # comp.files = ['tauMu_fullsel_tree_CMG.root']
elif test==2:
    selectedComponents = selectedComponents[:12]
    for comp in selectedComponents:
        comp.splitFactor = 1
        comp.files = comp.files[:5]


# the following is declared in case this cfg is used in input to the heppy.py script
from PhysicsTools.HeppyCore.framework.eventsfwlite import Events
config = cfg.Config( components = selectedComponents,
                     sequence = sequence,
                     services = [],  
                     events_class = Events
                     )

printComps(config.components, True)

def modCfgForPlot(config):
    config.components = []

