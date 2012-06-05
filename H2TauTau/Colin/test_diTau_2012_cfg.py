import copy
import os 
import CMGTools.RootTools.fwlite.Config as cfg
from CMGTools.H2TauTau.triggerMap import pathsAndFilters

runOnData = False
runOnMC = False

mc_vertexWeight = 'vertexWeightFall112011B'
mc_tauEffWeight = None
mc_jetEffWeight = None
mc_tauEffWeight_mc = 'effLooseTau15MC'
if runOnMC:
    mc_tauEffWeight='eff2012IsoTau25'
    mc_jetEffWeight='eff2012Jet30'

triggerAna = cfg.Analyzer(
    'TriggerAnalyzer',
    verbose = False
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
    triggerMap = pathsAndFilters
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

jetWeighter = cfg.Analyzer(
    'JetWeighter_jet1',
    effWeight = mc_jetEffWeight,
    effWeightMC = mc_tauEffWeight_mc,
    verbose = False
    )

if runOnData:
  vertexAna = cfg.Analyzer(
    'VertexAnalyzer',
    goodVertices = 'offlinePrimaryVertices', # hum... collection not available in old tuples
    #goodVertices = 'goodPVFilter',
    fixedWeight = 1,
    verbose = False
    )
else:
  vertexAna = cfg.Analyzer(
    'VertexAnalyzer',
    goodVertices = 'offlinePrimaryVertices', # hum... collection not available in old tuples
    #goodVertices = 'goodPVFilter',
    fixedWeight = 1,
    #vertexWeight = mc_vertexWeight,
    verbose = False
    )

# defined for vbfAna and eventSorter
vbfKwargs = dict( Mjj = 400,
                  deltaEta = 4.0    
                  )

vbfAna = cfg.Analyzer(
    'VBFAnalyzer',
    jetCol = 'cmgPFJetSel',
    jetPt = 30,
    jetEta = 4.5,
    vbfMvaWeights='/afs/cern.ch/user/h/hinzmann/workspace/stable2012/CMGTools/CMSSW_5_2_5/src/CMGTools/H2TauTau/data/VBFMVA_BDTG.weights.5XX.xml',
    **vbfKwargs
    )

treeProducer = cfg.Analyzer(
    'H2TauTauTreeProducerTauTau',
    )

#########################################################################################

from CMGTools.H2TauTau.proto.samples.run2012.diTau_June1 import * 

#########################################################################################

mc_jet_scale = 1.
mc_jet_smear = 0.
for mc in MC:
    # could handle the weights in the same way
    mc.jetScale = mc_jet_scale
    mc.jetSmear = mc_jet_smear

selectedComponents = data_2012 + [DYJets, WJets, TTJets, WW, WZ, ZZ]
#selectedComponents += [Higgsgg110, Higgsgg115, Higgsgg120, Higgsgg125, Higgsgg130, Higgsgg135, Higgsgg140, Higgsgg145,
#                       HiggsVBF110, HiggsVBF115, HiggsVBF120, HiggsVBF125, HiggsVBF130, HiggsVBF135, HiggsVBF140, HiggsVBF145]
selectedComponents += [Higgsgg110, Higgsgg120, Higgsgg125, Higgsgg130, Higgsgg135, Higgsgg140, Higgsgg145,
                       HiggsVBF115, HiggsVBF120, HiggsVBF125 ]

if runOnData:
    selectedComponents = data_2012
if runOnMC:
    selectedComponents = [DYJets, WJets, TTJets, WW, WZ, ZZ]
    #selectedComponents += [Higgsgg110, Higgsgg115, Higgsgg120, Higgsgg125, Higgsgg130, Higgsgg135, Higgsgg140, Higgsgg145,
    #                       HiggsVBF110, HiggsVBF115, HiggsVBF120, HiggsVBF125, HiggsVBF130, HiggsVBF135, HiggsVBF140, HiggsVBF145]
    selectedComponents += [Higgsgg110, Higgsgg120, Higgsgg125, Higgsgg130, Higgsgg135, Higgsgg140, Higgsgg145,
                           HiggsVBF115, HiggsVBF120, HiggsVBF125 ]

print [c.name for c in selectedComponents]

if runOnMC:
  sequence = cfg.Sequence( [
    TauTauAna,
    vbfAna,
    vertexAna,
    tau1Weighter, 
    tau2Weighter,
    jetWeighter,
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
    jetWeighter,
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
data_Run2012A_PromptReco_v1.splitFactor = 50
data_Run2012B_PromptReco_v1.splitFactor = 50

test = 0
if test==1:
    comp = DYJets
    #comp = WJets
    #comp = data_Run2012A_PromptReco_v1
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
