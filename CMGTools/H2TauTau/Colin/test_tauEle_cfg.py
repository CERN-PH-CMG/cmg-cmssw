import copy
import os 
import CMGTools.RootTools.fwlite.Config as cfg
from CMGTools.RootTools.fwlite.Config import printComps
from CMGTools.H2TauTau.triggerMap import pathsAndFilters

# 'Nom', 'Up', 'Down', or None
shift = None
# 1.0, 1.03, 0.97
tauScaleShift = 1

period = 'Period_2011AB'


mc_tauEffWeight_mc = 'effMediumIsoTau20MC'
mc_eleEffWeight_mc = 'effEle18MC'
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
    scaleShift1 = tauScaleShift,
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
    recEffVersion = None
    )

eleWeighter = cfg.Analyzer(
    'LeptonWeighter_ele',
    effWeight = mc_eleEffWeight,
    effWeightMC = mc_eleEffWeight_mc,
    lepton = 'leg2',
    verbose = False,
    disable = False,
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

#from CMGTools.H2TauTau.proto.samples.tauEle_PietroJun26 import * 
from CMGTools.H2TauTau.proto.samples.tauEle_PietroJul4 import * 

#########################################################################################



# MC_list = [WJets, DYJets, TTJets, W2Jets, W3Jets]
MC_list = copy.copy(MC)
data_list = copy.copy(data_2011)
embed_list = copy.copy(embed_2011)


for mc in MC_list:
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
        mc.splitFactor = 200
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
    emb.splitFactor = 10


data_Run2011A_May10ReReco_v1.splitFactor = 50
data_Run2011A_PromptReco_v4.splitFactor = 200
data_Run2011A_05Aug2011_v1.splitFactor = 50
data_Run2011A_03Oct2011_v1.splitFactor = 50
data_Run2011B_PromptReco_v1.splitFactor = 100

embed_Run2011A_May10ReReco_v1.splitFactor = 5
embed_Run2011A_PromptReco_v4.splitFactor = 5
embed_Run2011A_05Aug2011_v1.splitFactor = 5
embed_Run2011A_03Oct2011_v1.splitFactor = 5
embed_Run2011B_PromptReco_v1.splitFactor = 20

selectedComponents =  copy.copy(MC_list)
selectedComponents.extend( data_list )
selectedComponents.extend( embed_list )


sequence = cfg.Sequence( [
    jsonAna,
    triggerAna,
    vertexAna,
    tauEleAna,
    dyJetsFakeAna,
    higgsWeighter,
    vbfAna,
    embedWeighter,
    tauWeighter,
    eleWeighter, 
    treeProducer
   ] )


#selectedComponents = mc_higgs_ggh

test = 0
if test==1:
    comp = HiggsGGH125
    comp.files = getFiles('/VBF_HToTauTau_M-125_7TeV-powheg-pythia6-tauola/Fall11-PU_S6_START42_V14B-v1/AODSIM/V5/PAT_CMG_V5_4_1/TAUMU_TestMetFix', 'cmgtools', 'tauMu.*root')
    selectedComponents = [comp]
    comp.splitFactor = 1
elif test==2:
    for comp in selectedComponents:
        comp.splitFactor = 1
        comp.files = comp.files[:5]



config = cfg.Config( components = selectedComponents,
                     sequence = sequence )

printComps(config.components, True)
