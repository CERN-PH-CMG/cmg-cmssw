#TODO NO TRIGGER MAPPING YET! <-- is it true? FIXME 
#NO MC TRIGGER SELECTION YET! should now be ok FIXME
#GET EMBEDDED SAMPLES

import copy
import os 
import CMGTools.RootTools.fwlite.Config as cfg
from CMGTools.RootTools.fwlite.Config import printComps
from CMGTools.H2TauTau.triggerMap import pathsAndFilters


period = 'Period_2011AB'

baseDir = '2011'

mc_vertexWeight = None
mc_tauEffWeight = None
mc_eleEffWeight = None
mc_tauEffWeight_mc = 'effLooseTau15MC'
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


jsonAna = cfg.Analyzer(
    'JSONAnalyzer',
    )

triggerAna = cfg.Analyzer(
    'TriggerAnalyzer'
    )

vertexAna = cfg.Analyzer(
    'VertexAnalyzer',
    goodVertices = 'goodPVFilter', 
    vertexWeight = mc_vertexWeight,
    # fixedWeight = 1,
    verbose = False
    )

embedWeighter = cfg.Analyzer(
    'EmbedWeighter',
    verbose = False
    )

tauEleAna = cfg.Analyzer(
    'TauEleAnalyzer',
    pt1 = 20,
    pt2 = 20,
    iso1 = 999,
    iso2 = 0.1,
    eta1 = 2.3,
    eta2 = 2.1,
    m_min = 10,
    m_max = 99999,
    triggerMap = pathsAndFilters      
    )

dyJetsFakeAna = cfg.Analyzer(
    'DYJetsFakeAnalyzer',
    leptonType = 11
    )

tauWeighter = cfg.Analyzer(
    'LeptonWeighter_tau',
    effWeight = mc_tauEffWeight,
    effWeightMC = mc_tauEffWeight_mc,
    lepton = 'leg1',
    verbose = False,
    recEffVersion = None
    )

eleWeighter = cfg.Analyzer(
    'LeptonWeighter_ele',
    effWeight = mc_eleEffWeight,
    effWeightMC = mc_eleEffWeight_mc,
    lepton = 'leg2',
    verbose = False,
    recEffVersion = '2011'
    )


# defined for vbfAna and eventSorter
vbfKwargs = dict( Mjj = 400,
                  deltaEta = 4.0    
                  )


vbfAna = cfg.Analyzer(
    'VBFAnalyzer',
    jetCol = 'cmgPFJetSel',
    jetPt = 30,
    jetEta = 5.0,
    vbfMvaWeights = os.environ['CMSSW_BASE'] + '/src/CMGTools/H2TauTau/data/VBFMVA_BDTG.weights.44X.xml',
    **vbfKwargs
    )


treeProducer = cfg.Analyzer(
    'H2TauTauTreeProducerTauEle'
    )


#########################################################################################

from CMGTools.H2TauTau.proto.samples.tauEle_PietroJun26 import * 

#########################################################################################



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
    jsonAna,
    triggerAna,
    vertexAna,
    tauEleAna,
    dyJetsFakeAna,
    vbfAna,
    embedWeighter,
    tauWeighter,
    eleWeighter, 
    treeProducer
   ] )


DYJets.fakes = True
DYJets.splitFactor = 100
WJets.splitFactor = 10
TTJets.splitFactor = 100

data_Run2011A_May10ReReco_v1.splitFactor = 50
data_Run2011A_PromptReco_v4.splitFactor = 50
data_Run2011A_05Aug2011_v1.splitFactor = 50
data_Run2011A_03Oct2011_v1.splitFactor = 50
data_Run2011B_PromptReco_v1.splitFactor = 100

embed_Run2011A_May10ReReco_v1.splitFactor = 2
embed_Run2011A_PromptReco_v4.splitFactor = 4
embed_Run2011A_05Aug2011_v1.splitFactor = 2
embed_Run2011A_03Oct2011_v1.splitFactor = 2
embed_Run2011B_PromptReco_v1.splitFactor = 8

test = 0
if test==1:
    comp = embed_Run2011A_May10ReReco_v1
    selectedComponents = [comp]
    comp.splitFactor = 1
    comp.files = comp.files[:1]
elif test==2:
    for comp in selectedComponents:
     comp.splitFactor = 1
     comp.files = comp.files[:2]
elif test==3: # data only, test
    selectedComponents = data_2011 
    for comp in selectedComponents:
     comp.splitFactor = 1
     comp.files = comp.files[:2]
elif test==4: # data only, full run
    selectedComponents = data_2011 

config = cfg.Config( components = selectedComponents,
                     sequence = sequence )
