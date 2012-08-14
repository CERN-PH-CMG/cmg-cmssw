import copy
import os 
import CMGTools.RootTools.fwlite.Config as cfg
from CMGTools.RootTools.fwlite.Config import printComps

from CMGTools.H2TauTau.triggerMap import pathsAndFilters
from CMGTools.RootTools.RootTools import * 

# 'Nom', 'Up', 'Down', or None
shift = None
# 1.0, 1.03, 0.97
tauScaleShift = 1.0
syncntuple = True

puFileDir = os.environ['CMSSW_BASE'] + '/src/CMGTools/RootTools/data/Reweight/2012'
puFileData = None
puFileMC = '/'.join([puFileDir, 'MyMCPileupHistogram_true.root'])
puFileData = '/'.join([puFileDir, 'MyDataPileupHistogram_true_AB_start_196509.root'])
# puFileData = '/'.join([puFileDir, 'Test/MyDataPileupHistogram_RLT.root'])

vertexFileDir = os.environ['CMSSW_BASE'] + '/src/CMGTools/RootTools/data/Reweight/2012/Vertices'
vertexFileData = '/'.join([vertexFileDir, 'vertices_data_2012A_2012B_start_195947.root'])

mc_vertexWeight = None

#COLIN need to set the right triggers and to pick the right efficiencies
mc_tauEffWeight_mc = 'eff2012Tau20MC_TauEle'
mc_eleEffWeight_mc = 'eff2012Ele20MC'
mc_tauEffWeight = 'effTau2012AB_TauEle'
mc_eleEffWeight = 'effEle2012AB'
    

jsonAna = cfg.Analyzer(
    'JSONAnalyzer',
    # fixme pick it up automatically
    )

triggerAna = cfg.Analyzer(
    'TriggerAnalyzer'
    )

vertexAna = cfg.Analyzer(
    'VertexAnalyzer',
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


TauEleAna = cfg.Analyzer(
    'TauEleAnalyzer',
    scaleShift1 = tauScaleShift,
    pt1 = 20,
    eta1 = 2.3,
    iso1 = 999,
    pt2 = 24,
    eta2 = 2.1,
    iso2 = 0.1,
    m_min = 10,
    m_max = 99999,
    triggerMap = pathsAndFilters,
    verbose = False
    )

dyJetsFakeAna = cfg.Analyzer(
    'DYJetsFakeAnalyzer',
    leptonType = 11
    )

higgsWeighter = cfg.Analyzer(
    'HiggsPtWeighter',
    )

#COLIN enable efficiency weighting (make sure the efficiencies correspond to tau-ele triggers)
tauWeighter = cfg.Analyzer(
    'LeptonWeighter_tau',
    effWeight = mc_tauEffWeight,
    effWeightMC = mc_tauEffWeight_mc,
    lepton = 'leg1',
    verbose = False,
    disable = False,
    recEffVersion = None
    )

#COLIN enable efficiency weighting
# trigger & reco
electronWeighter = cfg.Analyzer(
    'LeptonWeighter_ele',
    effWeight = mc_eleEffWeight,
    effWeightMC = mc_eleEffWeight_mc,
    lepton = 'leg2',
    verbose = False,
    disable = False,
    recEffVersion = None
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
    'H2TauTauTreeProducerTauEle'
    )

treeProducerXCheck = cfg.Analyzer(
    'H2TauTauTreeProducerTauMuXCheck'    
    )

#########################################################################################

# from CMGTools.H2TauTau.proto.samples.run2012.tauEle_ColinJul5 import *
from CMGTools.H2TauTau.proto.samples.run2012.tauEle_ColinAug8 import *

#########################################################################################


# MC_list = [WJets, DYJets, TTJets, W2Jets, W3Jets]
MC_list = copy.copy(MC)
data_list = copy.copy(data_list_2012)
embed_list = copy.copy(embed_list_2012)

for mc in MC_list:
    mc.puFileMC = puFileMC
    mc.puFileData = puFileData
    mc.splitFactor = 10
    if mc.name.find('DYJets')!=-1:
        mc.splitFactor = 100
        mc.fakes = True
    elif mc.name.find('WJets')!=-1:
        mc.splitFactor = 50
    elif mc.name.find('W2Jets')!=-1:
        mc.splitFactor = 50
    elif mc.name.find('W3Jets')!=-1:
        mc.splitFactor = 25
    elif mc.name.find('TTJets')!=-1:
        mc.splitFactor = 80
    elif mc.name.find('WW')!=-1 or \
         mc.name.find('WZ')!=-1 or \
         mc.name.find('ZZ')!=-1:
        mc.splitFactor = 50
    elif mc.name.find('HiggsVBF120')!=-1 or \
         mc.name.find('HiggsGGH120')!=-1 or \
         mc.name.find('HiggsVH120')!=-1:
        mc.splitFactor = 50
    elif mc.name.find('HiggsVBF150')!=-1 or \
         mc.name.find('HiggsGGH150')!=-1 or \
         mc.name.find('HiggsVH150')!=-1:
        mc.splitFactor = 30
for emb in embed_list:
    emb.puFileData = None
    emb.puFileMC = None
    emb.splitFactor = 10

data_Run2012A.splitFactor = 40
data_Run2012B_start_194479.splitFactor = 50
data_Run2012B_194480_195016.splitFactor = 40
data_Run2012B_195017_195947.splitFactor = 40 
data_Run2012B_195948_196509.splitFactor = 50
data_Run2012B_start_196509.splitFactor = 200

selectedComponents =  copy.copy(MC_list)
selectedComponents.extend( data_list )
selectedComponents.extend( embed_list )


sequence = cfg.Sequence( [
    jsonAna,
    triggerAna,
    vertexAna,
    TauEleAna,
    dyJetsFakeAna,
    higgsWeighter, 
    vbfAna,
    pileUpAna,
    embedWeighter, 
    tauWeighter, 
    electronWeighter, 
    treeProducer
   ] )

if syncntuple:
    sequence.append( treeProducerXCheck)


test = 1
if test==1:
    comp = HiggsVBF125
    # comp.files = 'Aug14/joshMinusColin.root'
    selectedComponents = [comp]
    comp.splitFactor = 50
elif test==2:
    for comp in selectedComponents:
        comp.splitFactor = 1
        comp.files = comp.files[:5]



config = cfg.Config( components = selectedComponents,
                     sequence = sequence )

printComps(config.components, True)
