import PhysicsTools.HeppyCore.framework.config as cfg
from PhysicsTools.HeppyCore.framework.config import printComps
# from CMGTools.RootTools.fwlite.Config import printComps

# import all analysers:

# Heppy analyzers
from PhysicsTools.Heppy.analyzers.core.JSONAnalyzer import JSONAnalyzer
from PhysicsTools.Heppy.analyzers.core.EventSelector import EventSelector
from PhysicsTools.Heppy.analyzers.objects.VertexAnalyzer import VertexAnalyzer
from PhysicsTools.Heppy.analyzers.core.PileUpAnalyzer import PileUpAnalyzer
from PhysicsTools.Heppy.analyzers.gen.GeneratorAnalyzer import GeneratorAnalyzer

# Tau-tau analyzers
from CMGTools.H2TauTau.proto.analyzers.TriggerAnalyzer import TriggerAnalyzer
from CMGTools.H2TauTau.proto.analyzers.JetAnalyzer import JetAnalyzer
from CMGTools.H2TauTau.proto.analyzers.EmbedWeighter import EmbedWeighter
from CMGTools.H2TauTau.proto.analyzers.GenErsatzAnalyzer import GenErsatzAnalyzer
from CMGTools.H2TauTau.proto.analyzers.TauMuAnalyzer import TauMuAnalyzer
from CMGTools.H2TauTau.proto.analyzers.DYJetsFakeAnalyzer import DYJetsFakeAnalyzer
from CMGTools.H2TauTau.proto.analyzers.WNJetsAnalyzer import WNJetsAnalyzer
from CMGTools.H2TauTau.proto.analyzers.NJetsAnalyzer import NJetsAnalyzer
from CMGTools.H2TauTau.proto.analyzers.HiggsPtWeighter import HiggsPtWeighter
from CMGTools.H2TauTau.proto.analyzers.WNJetsTreeAnalyzer import WNJetsTreeAnalyzer
from CMGTools.H2TauTau.proto.analyzers.TauDecayModeWeighter import TauDecayModeWeighter
from CMGTools.H2TauTau.proto.analyzers.TauFakeRateWeighter import TauFakeRateWeighter
from CMGTools.H2TauTau.proto.analyzers.LeptonWeighter import LeptonWeighter
from CMGTools.H2TauTau.proto.analyzers.H2TauTauTreeProducerTauMu import H2TauTauTreeProducerTauMu
from CMGTools.H2TauTau.proto.analyzers.H2TauTauSyncTree import H2TauTauSyncTree

from CMGTools.RootTools.analyzers.VBFSimpleAnalyzer import VBFSimpleAnalyzer


# from CMGTools.Production.getFiles import getFiles
# from CMGTools.H2TauTau.triggerMap import pathsAndFilters
# from CMGTools.H2TauTau.proto.weights.weighttable import mu_id_taumu_2012, mu_iso_taumu_2012
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

# mc_tauEffWeight_mc = 'effTau_muTau_MC_2012ABCDSummer13'
# mc_muEffWeight_mc = 'effMu_muTau_MC_2012ABCD'
# mc_tauEffWeight = 'effTau_muTau_Data_2012ABCDSummer13'
# mc_muEffWeight = 'effMu_muTau_Data_2012ABCDSummer13'

mc_tauEffWeight_mc = None
mc_muEffWeight_mc = None
mc_tauEffWeight = None
mc_muEffWeight = None
    
    
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

triggerAna = cfg.Analyzer(
    TriggerAnalyzer,
    'TriggerAnalyzer',
    addTriggerObjects=True,
    requireTrigger=True,
    usePrescaled=False,
    )

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

genAna = GeneratorAnalyzer.defaultConfig

genErsatzAna = cfg.Analyzer(
    GenErsatzAnalyzer,
    'GenErsatzAnalyzer',
    verbose=False
    )

TauMuAna = cfg.Analyzer(
    TauMuAnalyzer,
    'TauMuAnalyzer',
    scaleShift1 = tauScaleShift,
    pt1 = 20,
    eta1 = 2.3,
    iso1 = None,
    pt2 = 20,
    eta2 = 2.1,
    iso2 = 0.1,
    m_min = 10,
    m_max = 99999,
    dR_min = 0.5,
    # triggerMap = pathsAndFilters,
    verbose = False
    )

dyJetsFakeAna = cfg.Analyzer(
    DYJetsFakeAnalyzer,
    'DYJetsFakeAnalyzer',
    leptonType = 13,
    src = 'prunedGenParticles',
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

muonWeighter = cfg.Analyzer(
    LeptonWeighter,
    'LeptonWeighter_mu',
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
    H2TauTauTreeProducerTauMu,
    'H2TauTauTreeProducerTauMu'
    )

treeProducerXCheck = cfg.Analyzer(
    H2TauTauSyncTree,
    'H2TauTauSyncTree',
    pt20 = False
    )

#########################################################################################

from CMGTools.H2TauTau.proto.samples.phys14.tauMu_Jan_Feb13 import MC_list, mc_higgs, mc_dict

#########################################################################################

# Use the following two lines for local testing
# HiggsVBF125.files = ['file:/afs/cern.ch/user/s/steggema/work/CMSSW_7_0_6_patch1/src/CMGTools/H2TauTau/prod/tauMu_fullsel_tree_CMG.root']
# HiggsVBF125.splitFactor = 1


for mc in MC_list:
    mc.puFileMC = puFileMC
    mc.puFileData = puFileData

selectedComponents = [mc_dict['HiggsGGH125']]

sequence = cfg.Sequence( [
    # eventSelector,
    jsonAna, 
    triggerAna,
    vertexAna, 
    TauMuAna,
    genAna,
    dyJetsFakeAna,
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
    muonWeighter, 
    treeProducer,
    # treeProducerXCheck
   ] )

if syncntuple:
    sequence.append( treeProducerXCheck) #Yes!

# selectedComponents = [comp for comp in selectedComponents if comp.dataset_entries > 0]

#mc_dict['HiggsTTHInclusive125'].files = ['/afs/cern.ch/user/s/steggema/work/CMSSW_7_2_3/src/CMGTools/H2TauTau/prod/tauMu_fullsel_tree_CMG.root']
# mc_dict['HiggsTTHInclusive125'].files = ['/afs/cern.ch/work/m/manzoni/diTau2015/CMSSW_7_2_3/src/CMGTools/H2TauTau/prod/diTau_fullsel_tree_CMG.root']

test = 1
if test==1:
    comp = mc_dict['HiggsGGH125']
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

