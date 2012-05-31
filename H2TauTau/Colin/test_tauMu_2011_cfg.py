import copy
import os 
import CMGTools.RootTools.fwlite.Config as cfg
from CMGTools.H2TauTau.triggerMap import pathsAndFilters


period = 'Period_2011AB'

baseDir = '2011'

mc_vertexWeight = None
mc_tauEffWeight = None
mc_muEffWeight = None
mc_tauEffWeight_mc = 'effLooseTau15MC'
mc_muEffWeight_mc = 'effIsoMu15MC'
if period == 'Period_2011A':
    mc_vertexWeight = 'vertexWeightFall112invfb'
    mc_tauEffWeight = 'effTau2011A'
    mc_muEffWeight = 'effMu2011A'
elif period == 'Period_2011B':
    mc_vertexWeight = 'vertexWeightFall112011B'
    mc_tauEffWeight = 'effTau2011B'
    mc_muEffWeight = 'effMu2011B'
elif period == 'Period_2011AB':
    mc_vertexWeight = 'vertexWeightFall112011AB'
    mc_tauEffWeight = 'effTau2011AB'
    mc_muEffWeight = 'effMu2011AB'


triggerAna = cfg.Analyzer(
    'TriggerAnalyzer'
    )

TauMuAna = cfg.Analyzer(
    'TauMuAnalyzer',
    pt1 = 20,
    eta1 = 2.3,
    iso1 = 999,
    pt2 = 17,
    eta2 = 2.1,
    iso2 = 0.1,
    m_min = 10,
    m_max = 99999,
    # diLeptonCutString = 'cuts_baseline',
    triggerMap = pathsAndFilters
    )

tauWeighter = cfg.Analyzer(
    'LeptonWeighter_tau',
    effWeight = mc_tauEffWeight,
    effWeightMC = mc_tauEffWeight_mc,
    lepton = 'leg1',
    verbose = False
    )

muonWeighter = cfg.Analyzer(
    'LeptonWeighter_mu',
    effWeight = mc_muEffWeight,
    effWeightMC = mc_muEffWeight_mc,
    lepton = 'leg2',
    verbose = False
    )

vertexAna = cfg.Analyzer(
    'VertexAnalyzer',
    # fixedWeight = 1,
    vertexWeight = mc_vertexWeight,
    verbose = False
    )

# defined for vbfAna and eventSorter
vbfKwargs = dict( Mjj = 400,
                  deltaEta = 4.0    
                  )

vbfAna = cfg.Analyzer(
    'VBFAnalyzer',
    jetCol = 'cmgPFJetSelCHS',
    jetPt = 30,
    jetEta = 4.5,
    **vbfKwargs
    )

eventSorter = cfg.Analyzer(
    'H2TauTauEventSorter',
    # vertexWeight = mc_vertexWeight,
    leg1 = 'tau',
    leg2 = 'mu',
    MT_low = 40,
    MT_high = 60,
    Boosted_JetPt = 150,
    **vbfKwargs
    )

treeProducer = cfg.Analyzer(
    'H2TauTauTreeProducerTauMu'
    )

#########################################################################################

from CMGTools.H2TauTau.proto.samples.tauMu_ColinMay18_CHS import * 
# from CMGTools.H2TauTau.proto.samples.tauMu_ColinMay18 import * 
# from CMGTools.H2TauTau.proto.samples.tauMu_ColinMay15 import * 

#########################################################################################

mc_jet_scale = 1.
mc_jet_smear = 0.
for mc in MC:
    # could handle the weights in the same way
    mc.jetScale = mc_jet_scale
    mc.jetSmear = mc_jet_smear


MC = [DYJets, WJets, TTJets]
# MC.extend( mc_higgs )
selectedComponents =  copy.copy(MC)

if period == 'Period_2011A':
    selectedComponents.extend( data_2011A )
    selectedComponents.extend( embed_2011A )    
elif period == 'Period_2011B':
    selectedComponents.extend( data_2011B )
    selectedComponents.extend( embed_2011B )    
elif period == 'Period_2011AB':
    selectedComponents.extend( data_2011 )
    selectedComponents.extend( embed_2011 )    



sequence = cfg.Sequence( [
    triggerAna,
    TauMuAna,
    vbfAna,
    vertexAna,
    tauWeighter, 
    muonWeighter, 
    treeProducer
   ] )


DYJets.fakes = True
DYJets.splitFactor = 40
WJets.splitFactor = 10
TTJets.splitFactor = 100 
data_Run2011B_PromptReco_v1.splitFactor = 50
data_Run2011A_PromptReco_v4.splitFactor = 40
data_Run2011A_May10ReReco_v1.splitFactor = 40
data_Run2011A_05Aug2011_v1.splitFactor = 20
data_Run2011A_03Oct2011_v1.splitFactor = 20

embed_Run2011B_PromptReco_v1.splitFactor = 10
embed_Run2011A_PromptReco_v4.splitFactor = 10
embed_Run2011A_May10ReReco_v1.splitFactor = 5
embed_Run2011A_05Aug2011_v1.splitFactor = 5
embed_Run2011A_03Oct2011_v1.splitFactor = 5

test = 0
# selectedComponents = embed_2011
if test==1:
    # comp = HiggsVBF120
    # comp.files = ['tauMu_fullsel_tree_CMG.root']
    # comp.files = ['/data/c/cbern/Tests/CMGTools/44X/May05/CMGTools/CMSSW_4_4_4/src/CMGTools/H2TauTau/prod/tauMu_fullsel_tree_CMG.root']
    comp = data_Run2011A_May10ReReco_v1
    selectedComponents = [comp]
    comp.splitFactor = 1
    comp.files = comp.files[:5]
    # TTJets.files = TTJets.files[:1]
elif test==2:
    for comp in selectedComponents:
        comp.splitFactor = 1
        # comp.files = comp.files[:2]

# selectedComponents.extend(MC)

config = cfg.Config( components = selectedComponents,
                     sequence = sequence )
