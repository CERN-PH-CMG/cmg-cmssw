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
doThePlot = True

puFileMC = '/afs/cern.ch/user/a/agilbert/public/HTT_Pileup/13-09-13/MC_Summer12_PU_S10-600bins.root'
puFileData = '/afs/cern.ch/user/a/agilbert/public/HTT_Pileup/13-09-13/Data_Pileup_2012_ReRecoPixel-600bins.root'

# vertexFileDir = os.environ['CMSSW_BASE'] + '/src/CMGTools/RootTools/data/Reweight/2012/Vertices'
# vertexFileData = '/'.join([vertexFileDir, 'vertices_data_2012A_2012B_start_195947.root'])

mc_vertexWeight = None

hlt_tauEffWeight_mc = 'effTau_eTau_MC_2012ABCDSummer13'
hlt_tauEffWeight = 'effTau_eTau_Data_2012ABCDSummer13'
hlt_eleEffWeight_mc = 'effEle_eTau_MC_2012ABCD'
hlt_eleEffWeight = 'effEle_eTau_Data_2012ABCDSummer13'
    
eventSelector = cfg.Analyzer(
    'EventSelector',
    toSelect = [
    # 569585, 57550, 912336, 133349, 423876, 468948, 676704, 126551, 586718, 375970, 922574, 206784
    168219, 936722, 147035, 57589, 207111
    ]
    )

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
    fixedWeight = 1,
    verbose = False
    )

embedWeighter = cfg.Analyzer(
    'EmbedWeighter',
    isRecHit = False,
    verbose = False
    )

pileUpAna = cfg.Analyzer(
    'PileUpAnalyzer',
    true = True
    )


tauEleAna = cfg.Analyzer(
    'TauEleAnalyzer',
    scaleShift1 = tauScaleShift,
    pt1 = 20,
    eta1 = 2.3,
    iso1 = None,
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
    src = 'genParticlesPruned',
    )

dyLLReweighterTauEle = cfg.Analyzer(
    'DYLLReweighterTauEle',
    # 2012 
    W1p0PB = 1.37, # weight for 1 prong 0 Pi Barrel
    W1p0PE = 1.11,
    W1p1PB = 2.18,
    W1p1PE = 0.47,
    verbose = False
    )

NJetsAna = cfg.Analyzer(
    'NJetsAnalyzer',
    verbose = False,
    fillTree = True,
    )

higgsWeighter = cfg.Analyzer(
    'HiggsPtWeighter',
    src = 'genParticlesPruned',
    )

tauDecayModeWeighter = cfg.Analyzer(
    'TauDecayModeWeighter',
    verbose = False,
    )

tauFakeRateWeighter = cfg.Analyzer(
    'TauFakeRateWeighter'
    )

tauWeighter = cfg.Analyzer(
    'LeptonWeighter_tau',
    effWeight = hlt_tauEffWeight,
    effWeightMC = hlt_tauEffWeight_mc,
    lepton = 'leg1',
    verbose = False,
    disable = False,
    )

eleWeighter = cfg.Analyzer(
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
vbfKwargs = dict( Mjj = 500,
                  deltaEta = 3.5    
                  )

vbfAna = cfg.Analyzer(
    'VBFAnalyzer',
    vbfMvaWeights = os.environ['CMSSW_BASE'] + '/src/CMGTools/H2TauTau/data/VBFMVA_BDTG_HCP_52X.weights.xml',
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

# from CMGTools.H2TauTau.proto.samples.run2012.tauEle_JanJul15 import *
from CMGTools.H2TauTau.proto.samples.run2012.tauEle_JanAug08 import *

#########################################################################################

for mc in MC_list:
    mc.puFileMC = puFileMC
    mc.puFileData = puFileData
    if mc.name.find('DYJets')!=-1:
        mc.fakes = True
for emb in embed_list:
    emb.puFileData = None
    emb.puFileMC = None


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

DYJetsSoup = copy.copy(DYJets)
DYJetsSoup.name = 'DYJetsSoup'

VVgroup = [comp.name for comp in diboson_list]
# higgs = [HiggsVBF125, HiggsGGH125, HiggsVH125]
TTgroup = None

selectedComponents = [TTJetsFullLept,
    TTJetsSemiLept,
    TTJetsHadronic, 
    WJets, W1Jets, W2Jets, W3Jets, W4Jets, 
    W1Jets_ext, W2Jets_ext, W3Jets_ext,
    # TTJets, 
    DYJets, DY1Jets, DY2Jets, DY3Jets, DY4Jets,
    ]

TTgroup = None
if doThePlot:
    selectedComponents = [
    WJetsSoup,
    TTJetsFullLept,
    TTJetsSemiLept,
    TTJetsHadronic, 
    DYJetsSoup
    ]
    TTgroup = [TTJetsFullLept.name,
        TTJetsSemiLept.name,
        TTJetsHadronic.name]
    # VVgroup = None

higgs = [c for c in mc_higgs if c.getWeight().genNEvents]
# if not doThePlot:
selectedComponents.extend( higgs )
selectedComponents.extend( mc_higgs_susy )
selectedComponents.extend( diboson_list )
selectedComponents.extend( data_list )
selectedComponents.extend( embed_list )

sequence = cfg.Sequence( [
    # eventSelector,
    jsonAna,
    triggerAna,
    vertexAna,
    tauEleAna,
    dyJetsFakeAna,
    dyLLReweighterTauEle,
    NJetsAna, 
    higgsWeighter, 
    vbfAna,
    pileUpAna,
    embedWeighter, 
    tauDecayModeWeighter,
    tauFakeRateWeighter,
    tauWeighter, 
    eleWeighter, 
    treeProducer
   ] )

if syncntuple:
    sequence.append( treeProducerXCheck)


test = 0
if test==1:
    comp = HiggsVBF125
    comp = DYJets
    comp = W1Jets_ext
    # comp.files = comp.files[:5]
    # comp = embed_Run2012D_22Jan
    selectedComponents = [comp]
    comp.splitFactor = 1
elif test==2:
    selectedComponents = copy.copy(data_list)
    selectedComponents.extend(embed_list)
    selectedComponents.extend(MC_list[0:3])
    for comp in selectedComponents:
        comp.splitFactor = 1
        comp.files = comp.files[:3]
elif test==3:
    # selectedComponents = [s for s in selectedComponents if 'Higgs' in s.name and not '125' in s.name]
    # selectedComponents = [h for h in higgs if 'GGH' in h.name]
    # selectedComponents = [DYJets, DY1Jets, DY2Jets, DY3Jets, DY4Jets]
    selectedComponents = [W1Jets_ext, W2Jets_ext, W3Jets_ext,WJets, W1Jets, W2Jets, W3Jets, W4Jets]
    # selectedComponents = mc_higgs_susy
    # selectedComponents += higgs
    # selectedComponents += [DYJets, DY1Jets, DY2Jets, DY3Jets, DY4Jets]
    # selectedComponents = embed_list
    # selectedComponents += [DYJets, DY1Jets, DY2Jets, DY3Jets, DY4Jets]
    # selectedComponents = data_list


config = cfg.Config( components = selectedComponents,
                     sequence = sequence )

printComps(config.components, True)
