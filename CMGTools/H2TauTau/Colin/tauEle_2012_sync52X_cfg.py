
import copy
import os 
import CMGTools.RootTools.fwlite.Config as cfg
from CMGTools.RootTools.fwlite.Config import printComps

from CMGTools.H2TauTau.triggerMap import pathsAndFilters
from CMGTools.H2TauTau.proto.weights.weighttable import ele_id_tauele_2012, ele_iso_tauele_2012
from CMGTools.H2TauTau.proto.samples.sampleShift import selectShift
from CMGTools.RootTools.RootTools import * 

# 'Nom', 'Up', 'Down', or None
shift = None
# 1.0, 1.03, 0.97
tauScaleShift = 1.0
syncntuple = True

puFileDir = os.environ['CMSSW_BASE'] + '/src/CMGTools/RootTools/data/Reweight/2012'

# andrew ICHEP
puFileMC = '/afs/cern.ch/user/a/agilbert/public/HTT_Pileup/12-09-12/MC_Summer12_PU_S7.root'
puFileData = '/afs/cern.ch/user/a/agilbert/public/HTT_Pileup/12-09-12/Data_Pileup_2012.root'

vertexFileDir = os.environ['CMSSW_BASE'] + '/src/CMGTools/RootTools/data/Reweight/2012/Vertices'
vertexFileData = '/'.join([vertexFileDir, 'vertices_data_2012A_2012B_start_195947.root'])

mc_vertexWeight = None

mc_tauEffWeight_mc = 'eff2012Tau20MC_TauEle'
mc_eleEffWeight_mc = 'eff2012Ele20MC'
mc_tauEffWeight = 'effTau2012AB_TauEle'
mc_eleEffWeight = 'effEle2012AB'
    

jsonAna = cfg.Analyzer(
    'JSONAnalyzer',
    )

triggerAna = cfg.Analyzer(
    'TriggerAnalyzer',
#    verbose = True
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
    mvametsigs = 'mvaMETTauEle',
    verbose = False
    )

dyJetsFakeAna = cfg.Analyzer(
    'DYJetsFakeAnalyzer',
    leptonType = 11
    )

WNJetsAna = cfg.Analyzer(
    'WNJetsAnalyzer',
    verbose = False
    )

higgsWeighter = cfg.Analyzer(
    'HiggsPtWeighter',
    )

tauWeighter = cfg.Analyzer(
    'LeptonWeighter_tau',
    effWeight = mc_tauEffWeight,
    effWeightMC = mc_tauEffWeight_mc,
    lepton = 'leg1',
    verbose = False,
    disable = False,
    )

electronWeighter = cfg.Analyzer(
    'LeptonWeighter_ele',
    effWeight = mc_eleEffWeight,
    effWeightMC = mc_eleEffWeight_mc,
    lepton = 'leg2',
    verbose = False,
    disable = False,
    idWeight = ele_id_tauele_2012,
    isoWeight = ele_iso_tauele_2012    
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
    'H2TauTauSyncTree'    
    )

#########################################################################################

# from CMGTools.H2TauTau.proto.samples.run2012.tauEle_ColinJul5 import *
# from CMGTools.H2TauTau.proto.samples.run2012.tauEle_ColinAug8 import *
# from CMGTools.H2TauTau.proto.samples.run2012.tauEle_Sync_ColinOct1 import *
from CMGTools.H2TauTau.proto.samples.run2012.tauEle_PietroSep25 import *

#########################################################################################


# MC_list = [WJets, DYJets, TTJets]
# MC_list = [HiggsVBF125]
MC_list = copy.copy(MC)
data_list = copy.copy(data_list_Run2012A)
data_list.extend(data_list_Run2012B)
embed_list = copy.copy(embed_list_Run2012A)
embed_list.extend(embed_list_Run2012B)

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
    elif mc.name.find('T_tW')!=-1:
        mc.splitFactor = 1
    elif mc.name.find('Tbar_tW')!=-1:
        mc.splitFactor = 1
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
data_Run2012B.splitFactor = 200

selectedComponents =  copy.copy(MC_list)
selectedComponents.extend( data_list )
selectedComponents.extend( embed_list )


sequence = cfg.Sequence( [
    # eventSelector,
    jsonAna,
    triggerAna,
    vertexAna,
    TauEleAna,
    dyJetsFakeAna,
    WNJetsAna,
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

# selectedComponents = embed_list

test = 0
if test==1:
    comp = HiggsVBF125
    selectedComponents = [comp]
    comp.splitFactor = 1
elif test==2:
    for comp in selectedComponents:
        comp.splitFactor = 1
        comp.files = comp.files[:5]



config = cfg.Config( components = selectedComponents,
                     sequence = sequence )

printComps(config.components, True)
