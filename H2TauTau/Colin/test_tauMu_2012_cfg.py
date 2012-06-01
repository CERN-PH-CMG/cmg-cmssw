import copy
import os 
import CMGTools.RootTools.fwlite.Config as cfg
from CMGTools.H2TauTau.triggerMap import pathsAndFilters


period = 'Period_2012AB'


puFileDir = os.environ['CMSSW_BASE'] + '/src/CMGTools/RootTools/data/Reweight/2012'
puFileData = None
puFileMC = '/'.join([puFileDir, 'MyMCPileupHistogram_true.root'])

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
elif period == 'Period_2012A':
    puFileData = '/'.join([puFileDir, 'MyDataPileupHistogram_true_A.root'])
elif period == 'Period_2012B':
    puFileData = '/'.join([puFileDir, 'MyDataPileupHistogram_true_B.root'])
elif period == 'Period_2012AB':
    puFileData = '/'.join([puFileDir, 'MyDataPileupHistogram_true_AB.root'])
    

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

pileUpAna = cfg.Analyzer(
    'PileUpAnalyzer',
    true = True
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
    # triggerMap = pathsAndFilters
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
    jetCol = 'cmgPFJetSel',
    jetPt = 30,
    jetEta = 4.5,
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

if period == 'Period_2011A':
    selectedComponents.extend( data_2011A )
    selectedComponents.extend( embed_2011A )    
elif period == 'Period_2011B':
    selectedComponents.extend( data_2011B )
    selectedComponents.extend( embed_2011B )    
elif period == 'Period_2011AB':
    selectedComponents.extend( data_2011 )
    selectedComponents.extend( embed_2011 )    
elif period == 'Period_2012A':
    selectedComponents.append( data_Run2012A )
elif period == 'Period_2012B':
    selectedComponents.append( data_Run2012B )
elif period == 'Period_2012AB':
    selectedComponents.extend( data_2012 )
    


sequence = cfg.Sequence( [
    jsonAna,
    triggerAna,
    vertexAna,
    TauMuAna,
    vbfAna,
    pileUpAna,
    tauWeighter, 
    muonWeighter, 
    treeProducer
   ] )


DYJets.fakes = True
DYJets.splitFactor = 10
WJets.splitFactor = 10
TTJets.splitFactor = 80

if period.find('Period_2012')==-1:
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
else:
    data_Run2012A.splitFactor = 80
    data_Run2012B.splitFactor = 40

test = 0
if test==1:
    # comp = data_Run2012B
    comp = TTJets
    # comp.files = comp.files[:20]
    # comp = data_2012[0]
    comp.files = comp.files[:1]
    selectedComponents = [comp]
    comp.splitFactor = 1
elif test==2:
    for comp in selectedComponents:
        comp.splitFactor = 1
        comp.files = comp.files[:2]

# selectedComponents = [TTJets]
# TTJets.splitFactor = 20

config = cfg.Config( components = selectedComponents,
                     sequence = sequence )
