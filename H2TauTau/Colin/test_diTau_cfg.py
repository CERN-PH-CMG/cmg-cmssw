import copy
import os 
import CMGTools.RootTools.fwlite.Config as cfg
from CMGTools.H2TauTau.triggerMap import pathsAndFilters


period = 'Period_2011AB'

baseDir = '2011'

mc_vertexWeight = None
mc_tauEffWeight = None
mc_tauEffWeight_mc = 'effLooseTau15MC'
if period == 'Period_2011A':
    mc_vertexWeight = 'vertexWeightFall112invfb'
    mc_tauEffWeight = 'effTau2011A'
elif period == 'Period_2011B':
    mc_vertexWeight = 'vertexWeightFall112011B'
    mc_tauEffWeight = 'effTau2011B'
elif period == 'Period_2011AB':
    mc_vertexWeight = 'vertexWeightFall112011AB'
    mc_tauEffWeight = 'effTau2011AB'

mc_tauEffWeight='effTau1fb'
#mc_tauEffWeight='effIsoTau25'
#mc_tauEffWeight='eff2012IsoTauL1'
#mc_tauEffWeight=None

triggerAna = cfg.Analyzer(
    'TriggerAnalyzer'
    )

TauTauAna = cfg.Analyzer(
    'TauTauAnalyzer',
    pt1 = 20,
    pt2 = 20,
    iso1 = 999,
    iso2 = 999,
    eta1 = 999,
    eta2 = 999,
    m_min = 10,
    m_max = 99999,
    diLeptonCutString = 'cuts_baseline',
    # No trigger matching for the moment since MC only contains too high threshold trigger
    #triggerMap = pathsAndFilters
    )

tau1Weighter = cfg.Analyzer(
    'LeptonWeighter_tau1',
    effWeight = mc_tauEffWeight,
    #effWeightMC = mc_tauEffWeight_mc,
    lepton = 'leg1',
    verbose = False
    )

tau2Weighter = cfg.Analyzer(
    'LeptonWeighter_tau2',
    effWeight = mc_tauEffWeight,
    #effWeightMC = mc_tauEffWeight_mc,
    lepton = 'leg2',
    verbose = False
    )

vertexAna = cfg.Analyzer(
    'VertexAnalyzer',
    vertexWeight = mc_vertexWeight,
    verbose = False
    )

# defined for vbfAna and eventSorter
vbfKwargs = dict( Mjj = 400,
                  deltaEta = 4.0    
                  )

vbfAna = cfg.Analyzer(
    'VBFAnalyzer',
    jetPt = 30,
    jetEta = 4.5,
    **vbfKwargs
    )

treeProducer = cfg.Analyzer(
    'H2TauTauTreeProducer',
    )

#########################################################################################

from CMGTools.H2TauTau.proto.samples.diTau_Apr10 import * 

#########################################################################################

mc_jet_scale = 1.
mc_jet_smear = 0.
for mc in MC:
    # could handle the weights in the same way
    mc.jetScale = mc_jet_scale
    mc.jetSmear = mc_jet_smear


selectedComponents =  copy.copy(MC)
if period == 'Period_2011A':
    selectedComponents.extend( data_2011A )
    # selectedComponents.extend( embed_2011A )    
elif period == 'Period_2011B':
    selectedComponents.extend( data_2011B )
    # selectedComponents.extend( embed_2011B )    
elif period == 'Period_2011AB':
    selectedComponents.extend( data_2011 )
    # selectedComponents.extend( embed_2011 )    



sequence = cfg.Sequence( [
    triggerAna,
    TauTauAna,
    vbfAna,
    vertexAna,
    tau1Weighter, 
    tau2Weighter, 
    treeProducer
   ] )


DYJets.fakes = True
DYJets.splitFactor = 16
WJets.fakes = True
WJets.splitFactor = 2
TTJets.splitFactor = 2 
QCD15.splitFactor = 2 
QCD30.splitFactor = 2 
QCD50.splitFactor = 2 
QCD80.splitFactor = 2 
ggHTT125.splitFactor = 1
VBFHTT125.splitFactor = 1
data_Run2011B_PromptReco_v1.splitFactor = 8
data_Run2011A_05Aug2011_v1.splitFactor = 2
data_Run2011A_PromptReco_v4.splitFactor = 2 
data_Run2011A_PromptReco_v6.splitFactor = 2
data_Run2011A_May10ReReco_v1.splitFactor = 2

test = 0
if test==1:
    comp = DYJets
    selectedComponents = [comp]
    comp.splitFactor = 1
    comp.files = comp.files[:10]
elif test==2:
    for comp in selectedComponents:
     comp.splitFactor = 1
     comp.files = comp.files[:1]

    
config = cfg.Config( components = selectedComponents,
                     sequence = sequence )
