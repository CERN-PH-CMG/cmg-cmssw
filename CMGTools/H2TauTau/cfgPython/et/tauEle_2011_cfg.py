import copy
import os 
import CMGTools.RootTools.fwlite.Config as cfg
from CMGTools.RootTools.fwlite.Config import printComps

from CMGTools.H2TauTau.triggerMap import pathsAndFilters
from CMGTools.H2TauTau.proto.weights.weighttable import ele_id_tauele_2011, ele_iso_tauele_2011
from CMGTools.H2TauTau.proto.samples.sampleShift import selectShift
from CMGTools.RootTools.RootTools import * 

# 'Nom', 'Up', 'Down', or None
shift = None
# 1.0, 1.03, 0.97
tauScaleShift = 1.0
syncntuple = True

mc_vertexWeight = 'vertexWeightFall112011AB'
mc_tauEffWeight = None
mc_muEffWeight = None


mc_tauEffWeight_mc = 'effMediumIsoTau20MC'
mc_eleEffWeight_mc = 'effEle18MC'
mc_tauEffWeight = 'effTau2011AB_TauEle'
mc_eleEffWeight = 'effEle2011AB'


jsonAna = cfg.Analyzer(
    'JSONAnalyzer',
    )

triggerAna = cfg.Analyzer(
    'TriggerAnalyzer',
    verbose = False
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
    eta1 = 2.3,
    iso1 = None,
    pt2 = 20,
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

dyLLReweighterTauEle = cfg.Analyzer(
    'DYLLReweighterTauEle',
    # 2012 
    W1p0PB = 0.87, # weight for 1 prong 0 Pi Barrel
    W1p0PE = 0.40,
    W1p1PB = 1.19,
    W1p1PE = 0.72,
    verbose = False
    )

WNJetsAna = cfg.Analyzer(
    'WNJetsAnalyzer',
    verbose = False,
    fractions = [ 0.752276599407,
                  0.171668857336,
                  0.0536961443722,
                  0.0159474294633,
                  0.00641100015491,
                  ],
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

eleWeighter = cfg.Analyzer(
    'LeptonWeighter_ele',
    effWeight = mc_eleEffWeight,
    effWeightMC = mc_eleEffWeight_mc,
    lepton = 'leg2',
    verbose = False,
    disable = False,
    idWeight = ele_id_tauele_2011,
    isoWeight = ele_iso_tauele_2011    
    )



# defined for vbfAna and eventSorter
vbfKwargs = dict( Mjj = 500,
                  deltaEta = 3.5    
                  )

vbfAna = cfg.Analyzer(
    'VBFAnalyzer',
    vbfMvaWeights = os.environ['CMSSW_BASE'] + '/src/CMGTools/H2TauTau/data/VBFMVA_BDTG_HCP_42X.weights.xml',
    jetCol = 'cmgPFJetSel',
    jetPt = 20.,
    jetEta = 4.7,
    cjvPtCut = 30.,
    btagSFseed = 123456,
    relaxJetId = False,
    **vbfKwargs
    )


treeProducer = cfg.Analyzer(
    'H2TauTauTreeProducerTauEle'
    )

treeProducerXCheck = cfg.Analyzer(
    'H2TauTauSyncTree',
    pt20 = False,
    # isFake = 0
    )

#########################################################################################

# from CMGTools.H2TauTau.proto.samples.run2011.tauMu_ColinJun25 import * 
# from CMGTools.H2TauTau.proto.samples.run2011.tauMu_ColinJul4 import * 
# from CMGTools.H2TauTau.proto.samples.run2011.tauMu_Sync_ColinAug30 import *

# from CMGTools.H2TauTau.proto.samples.run2011.tauMu_ColinSep20 import *

# from CMGTools.H2TauTau.proto.samples.run2011.tauEle_Up_ColinOct31 import *
# from CMGTools.H2TauTau.proto.samples.run2011.tauEle_Down_ColinOct31 import *
from CMGTools.H2TauTau.proto.samples.run2011.tauEle_Sync_Colin import *

#########################################################################################



WNJetsAna.nevents = [ WJets.nGenEvents,
                      W1Jets.nGenEvents,
                      W2Jets.nGenEvents,
                      W3Jets.nGenEvents,
                      W4Jets.nGenEvents
                      ]

# selectedComponents = allsamples
diboson_list = [    WWJetsTo2L2Nu,
                    WZJetsTo2L2Q,
                    WZJetsTo3LNu,
                    ZZJetsTo2L2Nu,
                    ZZJetsTo2L2Q,
                    ZZJetsTo4L,
                    T_tW,
                    Tbar_tW
                    ]
WJetsSoup = copy.copy(WJets)
WJetsSoup.name = 'WJetsSoup'
VVgroup = [comp.name for comp in diboson_list]
# higgs = [HiggsVBF125, HiggsGGH125, HiggsVH125]
selectedComponents =  [WJetsSoup, TTJets, DYJets]
# selectedComponents = [WJets, W1Jets, W2Jets, W3Jets, W4Jets, TTJets, DYJets]
higgs = mc_higgs
selectedComponents.extend( higgs )
selectedComponents.extend( diboson_list )
selectedComponents.extend( data_list_2011 )
selectedComponents.extend( embed_list_2011 )

sequence = cfg.Sequence( [
#     eventSelector,
    jsonAna,
    triggerAna,
    vertexAna,
    tauEleAna,
    dyJetsFakeAna,
    dyLLReweighterTauEle,
    WNJetsAna, 
    higgsWeighter, 
    vbfAna,
    embedWeighter,
    tauWeighter,
    eleWeighter, 
    treeProducer
   ] )

if syncntuple:
    sequence.append( treeProducerXCheck)


test = 1
if test==1:
    comp = HiggsVBF125
    # comp.files = comp.files[:5]
    selectedComponents = [comp]
    comp.splitFactor = 14
elif test==2:
    selectedComponents = copy.copy(data_list_2011)
    selectedComponents.extend(embed_list_2011)
    selectedComponents.extend(MC_list[0:3])
    for comp in selectedComponents:
        comp.splitFactor = 1
        comp.files = comp.files[:3]



config = cfg.Config( components = selectedComponents,
                     sequence = sequence )

printComps(config.components, True)
