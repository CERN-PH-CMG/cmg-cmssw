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


# selected by Andrew
#   *   113591 * 203.66099 *    179452 *       387 * 587118487 *


eventSelector = cfg.Analyzer(
    'EventSelector',
    toSelect = [
    587118487
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
    iso1 = None,
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
    'H2TauTauTreeProducerTauMu'
    )

treeProducerXCheck = cfg.Analyzer(
    'H2TauTauSyncTree',
    pt20 = False
    )

#########################################################################################

# from CMGTools.H2TauTau.proto.samples.run2011.tauMu_ColinJun25 import * 
# from CMGTools.H2TauTau.proto.samples.run2011.tauMu_ColinJul4 import * 
# from CMGTools.H2TauTau.proto.samples.run2011.tauMu_Sync_ColinAug30 import *

# from CMGTools.H2TauTau.proto.samples.run2011.tauMu_ColinSep20 import *
from CMGTools.H2TauTau.proto.samples.run2011.tauMu_Sync_Colin import *

#########################################################################################


# MC_list = [WJets, W3Jets, DYJets, TTJets, HiggsVBF125, WW, WZ, ZZ]
# MC_list = copy.copy(MC)
# data_list = copy.copy(data_list_2011)
# embed_list = copy.copy(embed_list_2011)

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
selectedComponents = [WJets, W1Jets, W2Jets, W3Jets, W4Jets, TTJets, DYJets]
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


test = 1
if test==1:
    comp = HiggsVBF125
    # comp.files = comp.files[:2]
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
