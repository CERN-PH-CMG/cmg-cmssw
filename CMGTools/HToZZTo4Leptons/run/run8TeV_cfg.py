import copy
import os 
import CMGTools.RootTools.fwlite.Config as cfg
from CMGTools.HToZZTo4Leptons.setup.EffectiveAreas import effectiveAreas2012 as effectiveAreas
from CMGTools.HToZZTo4Leptons.setup.FSR import FSRConfig as fsr

channel = 'all'

skimAnalyzer = cfg.Analyzer('SkimCountAnalyzer')

A4Skim = cfg.Analyzer('A4Skim',
    muons='cmgMuonSel',
    electrons='cmgElectronSel'
    )


#Trigger Information
triggerAna = cfg.Analyzer(
    'TriggerAnalyzer'
    )

#Vertex information
vertexAna = cfg.Analyzer(
    'VertexAnalyzer',
    fixedWeight = 1,
    verbose = False,
    makeHists=False
    )


#Pu reweighting
puAna = cfg.Analyzer(
    "PileUpAnalyzer",
    # build unweighted pu distribution using number of pile up interactions if False
    # otherwise, use fill the distribution using number of true interactions
    true = True,
    makeHists=False
    )


#Analyzers
ana = cfg.Analyzer(
    'AllFourLeptonAnalyzer',
    minPt1=5,
    maxEta1=2.4,
    minPt2=7,
    maxEta2=2.5,
    z1_m = (40.,120.),
    z2_m = (12.,120.),
    z1_pt1 = 20,
    z1_pt2 = 10,
    minHMass=100.,
    minHMassZ2=12.,
    minMass=4.,
    keep=False,
    effectiveAreas=effectiveAreas,
    rhoMuon     = 'kt6PFJetsCentralNeutral',
    rhoElectron = 'kt6PFJets',
    FSR=fsr
    )

zzTree = cfg.Analyzer( 'FourLeptonTreeProducer',
                   effectiveAreas=effectiveAreas
)


####################################################################################

from CMGTools.HToZZTo4Leptons.samples.samples_8TeV import * 

####################################################################################


jsonFilter = cfg.Analyzer(
    'JSONAnalyzer'
    )

    




#Define Sequences for data and MC
EventSelector = cfg.Analyzer(
    'EventSelector',
    toSelect = [10391]
    )


dataSequence=[
#    EventSelector,
    skimAnalyzer,
    jsonFilter,
    puAna,
    triggerAna,
    vertexAna,
    ana,
    zzTree
    ]



#for full production use data sequence
sequence = cfg.Sequence(dataSequence)

selectedComponents=mcSamples+dataSamples



test =0
if test==1:
    dataset=mcSamples[0]
    selectedComponents=[dataset]
    dataset.splitFactor = 1
    dataset.files=['vbf.root']
if test==2:
    dataset=dataSamples[0]
    selectedComponents=[dataset]
    dataset.splitFactor = 1

    
config = cfg.Config( components = selectedComponents,
                     sequence = sequence )
