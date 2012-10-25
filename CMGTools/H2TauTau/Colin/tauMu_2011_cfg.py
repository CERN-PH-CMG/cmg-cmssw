import copy
import os 
import CMGTools.RootTools.fwlite.Config as cfg
from CMGTools.RootTools.fwlite.Config import printComps

from CMGTools.H2TauTau.triggerMap import pathsAndFilters
from CMGTools.H2TauTau.proto.weights.weighttable import mu_id_taumu_2011, mu_iso_taumu_2011
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

mc_tauEffWeight_mc = 'effLooseTau15MC'
mc_muEffWeight_mc = 'effIsoMu15MC'
mc_tauEffWeight = 'effTau2011AB'
mc_muEffWeight = 'effMu2011AB'
    
eventSelector = cfg.Analyzer(
    'EventSelector',
    toSelect = [
    70370
    ]
    )


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


TauMuAna = cfg.Analyzer(
    'TauMuAnalyzer',
    scaleShift1 = tauScaleShift,
    pt1 = 20,
    eta1 = 2.3,
    iso1 = 999,
    pt2 = 17,
    eta2 = 2.1,
    iso2 = 0.1,
    m_min = 10,
    m_max = 99999,
    triggerMap = pathsAndFilters,
    mvametsigs = 'mvaMETTauMu',
    verbose = False
    )

dyJetsFakeAna = cfg.Analyzer(
    'DYJetsFakeAnalyzer',
    leptonType = 13
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

muonWeighter = cfg.Analyzer(
    'LeptonWeighter_mu',
    effWeight = mc_muEffWeight,
    effWeightMC = mc_muEffWeight_mc,
    lepton = 'leg2',
    verbose = False,
    disable = False,
    idWeight = mu_id_taumu_2011,
    isoWeight = mu_iso_taumu_2011    
    )



# defined for vbfAna and eventSorter
vbfKwargs = dict( Mjj = 400,
                  deltaEta = 4.0    
                  )

vbfAna = cfg.Analyzer(
    'VBFAnalyzer',
    vbfMvaWeights = os.environ['CMSSW_BASE'] + '/src/CMGTools/H2TauTau/data/VBFMVA_BDTG_HCP_42X.weights.xml',
    jetCol = 'cmgPFJetSel',
    jetPt = 30,
    jetEta = 5.0,
    btagSFseed = 123456,
    **vbfKwargs
    )


treeProducer = cfg.Analyzer(
    'H2TauTauTreeProducerTauMu'
    )

treeProducerXCheck = cfg.Analyzer(
    'H2TauTauSyncTree'    
    )

#########################################################################################

# from CMGTools.H2TauTau.proto.samples.run2011.tauMu_ColinJun25 import * 
# from CMGTools.H2TauTau.proto.samples.tauMu_ColinJul4 import * 
# from CMGTools.H2TauTau.proto.samples.tauMu_Sync_ColinAug30 import *

# from CMGTools.H2TauTau.proto.samples.tauMu_ColinSep20 import *
from CMGTools.H2TauTau.proto.samples.tauMu_ColinOct23 import *

#########################################################################################


# MC_list = [WJets, W3Jets, DYJets, TTJets, HiggsVBF125, WW, WZ, ZZ]
# MC_list = copy.copy(MC)
# data_list = copy.copy(data_list_2011)
# embed_list = copy.copy(embed_list_2011)

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
higgs = [HiggsVBF125, HiggsGGH125, HiggsVH125]
selectedComponents =  [WJetsSoup, TTJets, DYJets]
selectedComponents.extend( higgs )
selectedComponents.extend( diboson_list )
selectedComponents.extend( data_list_2011 )
selectedComponents.extend( embed_list_2011 )


sequence = cfg.Sequence( [
#     eventSelector,
    jsonAna,
    triggerAna,
    vertexAna,
    TauMuAna,
    dyJetsFakeAna,
    WNJetsAna, 
    higgsWeighter, 
    vbfAna,
    embedWeighter, 
    tauWeighter, 
    muonWeighter, 
    treeProducer
   ] )

if syncntuple:
    sequence.append( treeProducerXCheck)


test = 0
if test==1:
    comp = HiggsVBF125
    comp.files = comp.files[:2]
    selectedComponents = [comp]
    comp.splitFactor = 1
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
