import copy
import os 
import CMGTools.RootTools.fwlite.Config as cfg
from CMGTools.H2TauTau.triggerMap import pathsAndFilters

runOnData = False
runOnMC = False

period = 'Period_2011A'

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

if runOnMC:
    mc_tauEffWeight='effTau1fb'

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
    effWeightMC = mc_tauEffWeight_mc,
    lepton = 'leg1',
    verbose = False
    )

tau2Weighter = cfg.Analyzer(
    'LeptonWeighter_tau2',
    effWeight = mc_tauEffWeight,
    effWeightMC = mc_tauEffWeight_mc,
    lepton = 'leg2',
    verbose = False
    )

if runOnData:
  vertexAna = cfg.Analyzer(
    'VertexAnalyzer',
    fixedWeight = 1,
    verbose = False
    )
else:
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
    jetCol = 'cmgPFJetSelCHS2',
    jetPt = 30,
    jetEta = 4.5,
    **vbfKwargs
    )

treeProducer = cfg.Analyzer(
    'H2TauTauTreeProducerTauTau',
    )

#########################################################################################

from CMGTools.H2TauTau.proto.samples.diTau_May10 import * 

#########################################################################################

mc_jet_scale = 1.
mc_jet_smear = 0.
for mc in MC:
    # could handle the weights in the same way
    mc.jetScale = mc_jet_scale
    mc.jetSmear = mc_jet_smear

selectedComponents = data_2011 + [DYJets, WJets, TTJets, WW, WZ, ZZ]
selectedComponents += [Higgsgg110, Higgsgg115, Higgsgg120, Higgsgg125, Higgsgg130, Higgsgg135, Higgsgg140, Higgsgg145,
                       HiggsVBF110, HiggsVBF115, HiggsVBF120, HiggsVBF125, HiggsVBF130, HiggsVBF135, HiggsVBF140, HiggsVBF145]

if runOnData:
    selectedComponents = data_2011
if runOnMC:
    selectedComponents = [DYJets, WJets, TTJets, WW, WZ, ZZ]
    selectedComponents += [Higgsgg110, Higgsgg115, Higgsgg120, Higgsgg125, Higgsgg130, Higgsgg135, Higgsgg140, Higgsgg145,
                           HiggsVBF110, HiggsVBF115, HiggsVBF120, HiggsVBF125, HiggsVBF130, HiggsVBF135, HiggsVBF140, HiggsVBF145]

print [c.name for c in selectedComponents]

if runOnMC:
  sequence = cfg.Sequence( [
    TauTauAna,
    vbfAna,
    vertexAna,
    tau1Weighter, 
    tau2Weighter, 
    treeProducer
   ] )
else:
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
DYJets.splitFactor = 50
WJets.fakes = True
WJets.splitFactor = 50
TTJets.splitFactor = 50 
WW.splitFactor = 10
WZ.splitFactor = 10
ZZ.splitFactor = 10
data_Run2011A_May10ReReco_v1.splitFactor = 50
data_Run2011A_PromptReco_v4.splitFactor = 50 
data_Run2011A_05Aug2011_v1.splitFactor = 50
#data_Run2011A_PromptReco_v6.splitFactor = 50
#data_Run2011B_PromptReco_v1.splitFactor = 50

test = 0
if test==1:
    comp = DYJets
    #comp = WJets
    #comp = data_Run2011A_05Aug2011_v1
    #comp = Higgsgg125
    selectedComponents = [comp]
    comp.splitFactor = 10
    comp.files = comp.files[:1]
elif test==2:
    for comp in selectedComponents:
     comp.splitFactor = 1
     comp.files = comp.files[:1]

    
config = cfg.Config( components = selectedComponents,
                     sequence = sequence )
