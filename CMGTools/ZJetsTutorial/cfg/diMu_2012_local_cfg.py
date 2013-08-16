import copy
import os 
import CMGTools.RootTools.fwlite.Config as cfg
from CMGTools.RootTools.fwlite.Config import printComps

# from CMGTools.H2TauTau.triggerMap import pathsAndFilters
from CMGTools.RootTools.RootTools import * 

pathsAndFilters = {
    "HLT_Mu17_Mu8_v16": ("hltDiMuonMu17Mu8DzFiltered0p2","hltDiMuonMu17Mu8DzFiltered0p2"), 
    "HLT_Mu17_Mu8_v17": ("hltL3fL1DoubleMu10MuOpenL1f0L2f10L3Filtered17","hltL3fL1DoubleMu10MuOpenL1f0L2f10L3Filtered17"),
    "HLT_Mu17_Mu8_v18": ("hltL3fL1DoubleMu10MuOpenOR3p5L1f0L2f10L3Filtered17","hltL3fL1DoubleMu10MuOpenOR3p5L1f0L2f10L3Filtered17"), 
    "HLT_Mu17_Mu8_v19": ("hltL3fL1DoubleMu10MuOpenOR3p5L1f0L2f10L3Filtered17","hltL3fL1DoubleMu10MuOpenOR3p5L1f0L2f10L3Filtered17"), 
    "HLT_Mu17_Mu8_v20": ("hltL3fL1DoubleMu10MuOpenOR3p5L1f0L2f10L3Filtered17","hltL3fL1DoubleMu10MuOpenOR3p5L1f0L2f10L3Filtered17"),
    "HLT_Mu17_Mu8_v21": ("hltDiMuonGlb17Glb8DzFiltered0p2","hltDiMuonGlb17Glb8DzFiltered0p2"),
    "HLT_Mu17_Mu8_v22": ("hltDiMuonGlb17Glb8DzFiltered0p2","hltDiMuonGlb17Glb8DzFiltered0p2"),
    }

eventSelector = cfg.Analyzer(
    'EventSelector',
    toSelect = [
    # here put the event numbers (actual event numbers from CMSSW)
    ]
    )

jsonAna = cfg.Analyzer(
    'JSONAnalyzer',
    )


triggerAna = cfg.Analyzer(
    'TriggerAnalyzer',
    )

# this analyzer is just there to select a list of good primary vertices.
# in 2012, we don't use it to reweight events for pile-up
vertexAna = cfg.Analyzer(
    'VertexAnalyzer',
    goodVertices = 'offlinePrimaryVertices',
    vertexWeight = None,
    fixedWeight = 1,
    verbose = False
    )

# this analyzer actually does the pile-up reweighting.
# see below for the definition of the pile-up histograms. 
pileUpAna = cfg.Analyzer(
    'PileUpAnalyzer',
    true = True
    )


ZMuMuAna = cfg.Analyzer(
    'ZMuMuAnalyzer',
    pt1 = 10,
    eta1 = 2.4,
    iso1 = 0.3,
    pt2 = 10,
    eta2 = 2.4,
    iso2 = 0.3,
    m_min = 50,
    m_max = 120,
    verbose = False,
    triggerMap = pathsAndFilters
    )

ZMuMuRochCor = cfg.Analyzer(
    'ZMuMuRochCorAnalyzer'
    )


jetAna = cfg.Analyzer(
    'JetAnalyzer',
    jetCol = 'cmgPFJetSel',
    jetPt = 20.,
    jetEta = 4.7,
    btagSFseed = 123456,
    relaxJetId = True, 
    )

vbfAna = cfg.Analyzer(
    'VBFSimpleAnalyzer',
    Mjj = 500,
    deltaEta = 3.5,
    cjvPtCut = 30.,
    )

treeProducer = cfg.Analyzer(
    'ZJetsTreeProducer'
    )


###############################################################################


# from CMGTools.ZJetsTutorial.samples.run2012.diMu_ColinNov13 import *
from CMGTools.ZJetsTutorial.samples.run2012.ewk import * 
###############################################################################


# VVgroup = [c.name for c in mc_diboson_inc]

# allsamples = copy.copy(data_list)
# allsamples.extend([DYJets])
# allsamples.extend([TTJets])
# allsamples.extend(mc_diboson_inc)

DYJets.files = ['/afs/cern.ch/user/c/cbern/public/ZJetsTutorial/cmgTuple_0.root']
MC_list = [DYJets]
allsamples = MC_list

for c in allsamples:
    c.triggers = [
        'HLT_Mu17_Mu8_v16',
        'HLT_Mu17_Mu8_v17',
        'HLT_Mu17_Mu8_v18',
        'HLT_Mu17_Mu8_v19',
        'HLT_Mu17_Mu8_v20',
        'HLT_Mu17_Mu8_v21',
        'HLT_Mu17_Mu8_v22',
        ]

# allsamples = data_list

selectedComponents = allsamples

puFileMC = '/afs/cern.ch/user/a/agilbert/public/HTT_Pileup/28-09-12/MC_Summer12_PU_S10-600bins.root'
puFileData = '/afs/cern.ch/user/a/agilbert/public/HTT_Pileup/28-09-12/Data_Pileup_2012_HCP-600bins.root'
for mc in MC_list:
    mc.puFileMC = puFileMC
    mc.puFileData = puFileData

sequence = cfg.Sequence( [
    #     eventSelector,
    jsonAna,
    triggerAna,
    vertexAna,
    pileUpAna,
    ZMuMuAna,
    ZMuMuRochCor, 
    jetAna,
    vbfAna,
    treeProducer
   ] )


# set test = 0 to run all jobs, in case you are using pybatch.py
test = 1
if test==1:
    # test a single component, using a single thread.
    # necessary to debug the code, until it doesn't crash anymore
    comp = DYJets
    comp.files = comp.files[:10]
    selectedComponents = [comp]
    comp.splitFactor = 1
elif test==2:    
    # test all components (1 thread per component.
    # important to make sure that your code runs on any kind of component
    for comp in selectedComponents:
        comp.splitFactor = 1
        comp.files = comp.files[:3]

# creation of the processing configuration.
# we define here on which components to run, and
# what is the sequence of analyzers to run on each event. 
config = cfg.Config( components = selectedComponents,
                     sequence = sequence )

printComps(config.components, True)
