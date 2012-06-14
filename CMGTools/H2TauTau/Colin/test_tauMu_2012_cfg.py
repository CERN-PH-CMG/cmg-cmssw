import copy
import os 
import CMGTools.RootTools.fwlite.Config as cfg
from CMGTools.H2TauTau.triggerMap import pathsAndFilters
from CMGTools.RootTools.RootTools import * 

period = 'Period_2012AB'


puFileDir = os.environ['CMSSW_BASE'] + '/src/CMGTools/RootTools/data/Reweight/2012'
puFileData = None
puFileMC = '/'.join([puFileDir, 'MyMCPileupHistogram_true.root'])

mc_vertexWeight = None
mc_tauEffWeight = None
mc_muEffWeight = None
mc_tauEffWeight_mc = 'effTau2012MC'

mc_muEffWeight_mc = None

if period == 'Period_2012A':
    puFileData = '/'.join([puFileDir, 'MyDataPileupHistogram_true_A.root'])
    mc_tauEffWeight = 'effTau2012A'
elif period == 'Period_2012B':
    puFileData = '/'.join([puFileDir, 'MyDataPileupHistogram_true_B.root'])
    mc_tauEffWeight = 'effTau2012B'
elif period == 'Period_2012AB':
    puFileData = '/'.join([puFileDir, 'MyDataPileupHistogram_true_AB.root'])
    mc_tauEffWeight = 'effTau2012AB'
    

jsonAna = cfg.Analyzer(
    'JSONAnalyzer',
    # fixme pick it up automatically
    )

triggerAna = cfg.Analyzer(
    'TriggerAnalyzer'
    )

vertexAna = cfg.Analyzer(
    'VertexAnalyzer',
    # goodVertices = 'offlinePrimaryVertices', # hum... collection not available in old tuples
    goodVertices = 'goodPVFilter',
    vertexWeight = mc_vertexWeight,
    fixedWeight = 1,
    verbose = False
    )

embedWeighter = cfg.Analyzer(
    'EmbedWeighter',
    verbose = False
    )

pileUpAna = cfg.Analyzer(
    'PileUpAnalyzer',
    true = True
    )


TauMuAna = cfg.Analyzer(
    'TauMuAnalyzer',
    pt1 = 20,
    eta1 = 2.3,
    iso1 = 999,
    pt2 = 18,
    eta2 = 2.1,
    iso2 = 0.1,
    m_min = 10,
    m_max = 99999,
    triggerMap = pathsAndFilters
    )

tauWeighter = cfg.Analyzer(
    'LeptonWeighter_tau',
    effWeight = mc_tauEffWeight,
    effWeightMC = mc_tauEffWeight_mc,
    lepton = 'leg1',
    verbose = False,
    disable = True
    )

muonWeighter = cfg.Analyzer(
    'LeptonWeighter_mu',
    effWeight = mc_muEffWeight,
    effWeightMC = mc_muEffWeight_mc,
    lepton = 'leg2',
    verbose = False,
    disable = True
    )



# defined for vbfAna and eventSorter
vbfKwargs = dict( Mjj = 400,
                  deltaEta = 4.0    
                  )

vbfAna = cfg.Analyzer(
    'VBFAnalyzer',
    vbfMvaWeights = os.environ['CMSSW_BASE'] + '/src/CMGTools/H2TauTau/data/VBFMVA_BDTG.weights.5XX.xml',
    jetCol = 'cmgPFJetSel',
    jetPt = 30,
    jetEta = 5.0,
    **vbfKwargs
    )


treeProducer = cfg.Analyzer(
    'H2TauTauTreeProducerTauMu'
    )

#########################################################################################

from CMGTools.H2TauTau.proto.samples.run2012.tauMu_ColinMay30 import * 
# from CMGTools.H2TauTau.proto.samples.tauMu_ColinMay18_CHS import * 
# from CMGTools.H2TauTau.proto.samples.tauMu_ColinMay18 import * 
# from CMGTools.H2TauTau.proto.samples.tauMu_ColinMay15 import * 

#########################################################################################

mc_jet_scale = 1.
mc_jet_smear = 0.
for mc in MC:
    mc.jetScale = mc_jet_scale
    mc.jetSmear = mc_jet_smear
    mc.puFileData = puFileData
    mc.puFileMC = puFileMC


MC = [DYJets, WJets, TTJets]
# MC.extend( mc_higgs )
selectedComponents =  copy.copy(MC)


if period == 'Period_2012A':
    selectedComponents.extend( data_list_Run2012A )
    selectedComponents.extend( embed_list_Run2012A )
elif period == 'Period_2012B':
    selectedComponents.extend( data_list_Run2012B )
    selectedComponents.extend( embed_list_Run2012B )
elif period == 'Period_2012AB':
    selectedComponents.extend( data_list_2012 )
    selectedComponents.extend( embed_list_2012 )
    
# selectedComponents.append( zdata_Run2012A )

sequence = cfg.Sequence( [
    jsonAna,
    triggerAna,
    vertexAna,
    TauMuAna,
    vbfAna,
    pileUpAna,
    embedWeighter, 
    tauWeighter, 
    muonWeighter, 
    treeProducer
   ] )


DYJets.fakes = True
DYJets.splitFactor = 100
WJets.splitFactor = 10
TTJets.splitFactor = 50
HiggsVBF115.splitFactor = 10
HiggsVBF120.splitFactor = 50
HiggsVBF125.splitFactor = 10

data_Run2012A.splitFactor = 40
data_Run2012B_start_194479.splitFactor = 40
data_Run2012B_194480_195016.splitFactor = 40

embed_Run2012A.splitFactor = 5
embed_Run2012B_193752_195135.splitFactor = 10


test = 0
if test==1:
    comp = embed_Run2012A
    # comp = HiggsVBF125
    comp.files = comp.files[:10]
    selectedComponents = [comp]
    comp.splitFactor = 1
elif test==2:
    for comp in selectedComponents:
        comp.splitFactor = 1
        comp.files = comp.files[:5]

selectedComponents = embed_list_2012
selectedComponents.extend(MC)

# TTJets.splitFactor = 20

config = cfg.Config( components = selectedComponents,
                     sequence = sequence )
