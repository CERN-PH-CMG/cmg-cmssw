#TODO NO TRIGGER MAPPING YET! 
#NO MC TRIGGER SELECTION YET! should now be ok
#NEED TO TRY MIT MVA
#GET EMBEDDED SAMPLES

import copy
import os 
import CMGTools.RootTools.fwlite.Config as cfg
from CMGTools.H2TauTau.triggerMap import pathsAndFilters


period = 'Period_2011AB'

baseDir = '2011'

mc_vertexWeight = None
mc_tauEffWeight = None
mc_eleEffWeight = None
mc_tauEffWeight_mc = 'effMediumIsoTau20MC'
mc_eleEffWeight_mc = 'effEle18MC'
if period == 'Period_2011A':
    mc_vertexWeight = 'vertexWeightFall112invfb'
    mc_tauEffWeight = 'effTau2011A_TauEle'
    mc_eleEffWeight = 'effEle2011A'
elif period == 'Period_2011B':
    mc_vertexWeight = 'vertexWeightFall112011B'
    mc_tauEffWeight = 'effTau2011B_TauEle'
    mc_eleEffWeight = 'effEle2011B'
elif period == 'Period_2011AB':
    mc_vertexWeight = 'vertexWeightFall112011AB'
    mc_tauEffWeight = 'effTau2011AB_TauEle'
    mc_eleEffWeight = 'effEle2011AB'


triggerAna = cfg.Analyzer(
    'TriggerAnalyzer'
    )

tauEleAna = cfg.Analyzer(
    'TauEleAnalyzer',
    pt1 = 20,
    pt2 = 20,
    iso1 = 999,
    iso2 = 0.1,
    eta1 = 999,
    eta2 = 2.1,
    m_min = 10,
    m_max = 99999,
    diLeptonCutString = 'cuts_baseline',
    # triggerMap = pathsAndFilters
    )

tauWeighter = cfg.Analyzer(
    'LeptonWeighter_tau',
    effWeight = mc_tauEffWeight,
    effWeightMC = mc_tauEffWeight_mc,
    lepton = 'leg1',
    verbose = False
    )

# DO I USE THE FALL11 SAMPLES? WITH TRIGGER SELECTION?
eleWeighter = cfg.Analyzer(
    'LeptonWeighter_ele',
    effWeight = mc_eleEffWeight,
    effWeightMC = mc_eleEffWeight_mc,
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

eventSorter = cfg.Analyzer(
    'H2TauTauEventSorter',
    # vertexWeight = mc_vertexWeight,
    leg1 = 'tau',
    leg2 = 'ele',
    MT_low = 40,
    MT_high = 60,
    Boosted_JetPt = 150,
    **vbfKwargs
    )

#########################################################################################

from CMGTools.H2TauTau.proto.samples.tauEle_march12 import * 

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
    tauEleAna,
    vbfAna,
    vertexAna,
    tauWeighter, 
    eleWeighter, 
    eventSorter
   ] )

DYJets.fakes = True
TTJets.splitFactor = 2 
DYJets.splitFactor = 8
WJets.splitFactor = 3
data_Run2011B_PromptReco_v1.splitFactor = 8
data_Run2011A_PromptReco_v4.splitFactor = 2 

## embed_Run2011A_May10ReReco_v1.splitFactor = 2
## embed_Run2011A_PromptReco_v4.splitFactor = 4
## embed_Run2011A_05Aug2011_v1.splitFactor = 2
## embed_Run2011A_03Oct2011_v1.splitFactor = 2
## embed_Run2011B_PromptReco_v1.splitFactor = 8

test = 0
if test==1:
    comp = DYJets
    selectedComponents = [comp]
    comp.splitFactor = 1
    comp.files = comp.files[:2]
    # TTJets.files = TTJets.files[:1]
elif test==2:
    for comp in selectedComponents:
     comp.splitFactor = 1
     comp.files = comp.files[:2]


config = cfg.Config( components = selectedComponents,
                     sequence = sequence )
