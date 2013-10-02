import copy
import os 
import CMGTools.RootTools.fwlite.Config as cfg
from CMGTools.H2TauTau.triggerMap import pathsAndFilters


period = 'Period_2011AB'


mc_vertexWeight = None
mc_tauEffWeight = None
mc_muEffWeight = None
mc_tauEffWeight_mc = 'effLooseTau15MC'
# mc_muEffWeight_mc = 'effIsoMu15MC'
mc_muEffWeight_mc = None

mc_tauEffWeight = 'effTau2012AB'

puFileDir11 = os.environ['CMSSW_BASE'] + '/src/CMGTools/RootTools/data/Reweight/2011'
puFileDir12 = os.environ['CMSSW_BASE'] + '/src/CMGTools/RootTools/data/Reweight/2012'
puFileMC = '/'.join([puFileDir11, 'MyMCPileupHistogram_true.root'])
# puFileData = '/'.join([puFileDir12, 'MyDataPileupHistogram_true_AB.root'])
puFileData = '/'.join([puFileDir12, 'MyDataPileupHistogram_true_AB_190456_196531.root'])


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
    verbose = False
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
    vbfMvaWeights = os.environ['CMSSW_BASE'] + '/src/CMGTools/H2TauTau/data/VBFMVA_BDTG.weights.44X.xml',
    jetCol = 'cmgPFJetSel',
    jetPt = 30,
    jetEta = 5.0,
    **vbfKwargs
    )


treeProducer = cfg.Analyzer(
    'H2TauTauTreeProducerTauMu'
    )

#########################################################################################

from CMGTools.H2TauTau.proto.samples.tauMu_ColinJune1 import * 
# from CMGTools.H2TauTau.proto.samples.tauMu_ColinMay18_CHS import * 
# from CMGTools.H2TauTau.proto.samples.tauMu_ColinMay18 import * 
# from CMGTools.H2TauTau.proto.samples.tauMu_ColinMay15 import * 

#########################################################################################



MC = [WJets]
for mc in MC:
    mc.puFileData = puFileData
    mc.puFileMC = puFileMC


selectedComponents =  copy.copy(MC)


sequence = cfg.Sequence( [
    triggerAna,
    vertexAna,
    pileUpAna, 
    TauMuAna,
    vbfAna,
    tauWeighter, 
    muonWeighter, 
    treeProducer
   ] )


W3Jets.splitFactor = 50 
TTJets.splitFactor = 100
WJets.splitFactor = 100

test = 0
if test==1:
    comp = WJets
    comp.files = comp.files[:19]
    # comp = data_2012[0]
    selectedComponents = [comp]
    comp.splitFactor = 1


config = cfg.Config( components = selectedComponents,
                     sequence = sequence )
