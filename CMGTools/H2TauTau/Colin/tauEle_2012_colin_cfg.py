
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
# puFileMC = '/afs/cern.ch/user/a/agilbert/public/HTT_Pileup/12-09-12/MC_Summer12_PU_S7.root'
# puFileData = '/afs/cern.ch/user/a/agilbert/public/HTT_Pileup/12-09-12/Data_Pileup_2012.root'
# andrew HCP
puFileMC = '/afs/cern.ch/user/a/agilbert/public/HTT_Pileup/28-09-12/MC_Summer12_PU_S10-600bins.root'
puFileData = '/afs/cern.ch/user/a/agilbert/public/HTT_Pileup/28-09-12/Data_Pileup_2012_HCP-600bins.root'


vertexFileDir = os.environ['CMSSW_BASE'] + '/src/CMGTools/RootTools/data/Reweight/2012/Vertices'
vertexFileData = '/'.join([vertexFileDir, 'vertices_data_2012A_2012B_start_195947.root'])

mc_vertexWeight = None

hlt_tauEffWeight_mc = 'eff2012Tau20MC53X_TauEle'
hlt_tauEffWeight = 'effTau2012ABC_TauEle'
hlt_eleEffWeight_mc = 'eff_2012_Rebecca_TauEle_Ele2253XMC'
hlt_eleEffWeight = 'effEle2012_Rebecca_TauEle_ABC'


eventSelector = cfg.Analyzer(
    'EventSelector',
    toSelect = [
    294445 ,
    375970 ,
    857134 ,
     27006 ,
     53847 ,
     55227 ,
     99808 ,
    712871 ,
    719098 ,
    107416 ,
    240580 ,
    588573 ,
    178124 ,
    359703 ,
    369616 ,
    571459 ,
    ]
    )

jsonAna = cfg.Analyzer(
    'JSONAnalyzer',
    )

triggerAna = cfg.Analyzer(
    'TriggerAnalyzer',
    verbose = True
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
    verbose = True
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
    effWeight = hlt_tauEffWeight,
    effWeightMC = hlt_tauEffWeight_mc,
    lepton = 'leg1',
    verbose = False,
    disable = False,
    )

electronWeighter = cfg.Analyzer(
    'LeptonWeighter_ele',
    effWeight = hlt_eleEffWeight,
    effWeightMC = hlt_eleEffWeight_mc,
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
    vbfMvaWeights = os.environ['CMSSW_BASE'] + '/src/CMGTools/H2TauTau/data/VBFMVA_BDTG_HCP_52X.weights.xml',
    jetCol = 'cmgPFJetSel',
    jetPt = 20,
    jetEta = 4.7,
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
# from CMGTools.H2TauTau.proto.samples.run2012.tauEle_PietroSep25 import *

# from CMGTools.H2TauTau.proto.samples.run2012.tauEle_PietroOct05 import *
from CMGTools.H2TauTau.proto.samples.run2012.tauEle_Sync_Colin import *

#########################################################################################


for mc in MC_list:
    mc.puFileMC = puFileMC
    mc.puFileData = puFileData

for emb in embed_list:
    emb.puFileData = None
    emb.puFileMC = None

selectedComponents = allsamples


sequence = cfg.Sequence( [
    eventSelector,
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
    sequence.append( treeProducerXCheck )

test = 1
if test==1:
    #    comp = DYJets
    #    comp = data_Run2012A
    # comp = data_Run2012C_v2
    comp = HiggsVBF125
    #    comp = HiggsVH125
    #    comp = Tbar_tW
    #    HiggsVBF125.triggers = []
    comp.files = comp.files[:10]
    selectedComponents = [comp]
    comp.splitFactor = 1
    # comp.files = comp.files[:1]
    # for 53 MC: 
    #    comp.triggers = ['HLT_Ele22_eta2p1_WP90Rho_LooseIsoPFTau20_v*']
elif test==2:
    selectedComponents = copy.copy (embed_list)
    for comp in selectedComponents:
        comp.splitFactor = 1
        comp.files = comp.files[:10]
elif test==5:
    # run this with python to get numbers for the soup
    print 'WJets  : ', WJets.nGenEvents,  WJets.effCorrFactor
    print 'W1Jets : ', W1Jets.nGenEvents, W1Jets.effCorrFactor
    print 'W2Jets : ', W2Jets.nGenEvents, W2Jets.effCorrFactor
    print 'W3Jets : ', W3Jets.nGenEvents, W3Jets.effCorrFactor
    print 'W4Jets : ', W4Jets.nGenEvents, W4Jets.effCorrFactor


config = cfg.Config( components = selectedComponents,
                     sequence = sequence )

printComps(config.components, True)
