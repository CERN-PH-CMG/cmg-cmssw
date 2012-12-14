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


puFileMC = '/afs/cern.ch/user/a/agilbert/public/HTT_Pileup/28-09-12/MC_Summer12_PU_S10-600bins.root'
puFileData = '/afs/cern.ch/user/a/agilbert/public/HTT_Pileup/28-09-12/Data_Pileup_2012_HCP-600bins.root'


# vertexFileDir = os.environ['CMSSW_BASE'] + '/src/CMGTools/RootTools/data/Reweight/2012/Vertices'
# vertexFileData = '/'.join([vertexFileDir, 'vertices_data_2012A_2012B_start_195947.root'])

mc_vertexWeight = None

hlt_tauEffWeight_mc = 'eff2012Tau20MC53X_TauEle'
hlt_tauEffWeight = 'effTau2012ABC_TauEle'
hlt_eleEffWeight_mc = 'eff_2012_Rebecca_TauEle_Ele2253XMC'
hlt_eleEffWeight = 'effEle2012_Rebecca_TauEle_ABC'

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
    leptonType = 11
    )

dyLLReweighterTauEle = cfg.Analyzer(
    'DYLLReweighterTauEle',
    # 2012 
    W1p0PB = 0.82, # weight for 1 prong 0 Pi Barrel
    W1p0PE = 0.76,
    W1p1PB = 1.65,
    W1p1PE = 0.24,
    verbose = False
    )

WNJetsAna = cfg.Analyzer(
    'WNJetsAnalyzer',
    verbose = False,
    fractions = [ 0.743912391955,
                  0.175996204386,
                  0.0562852166761,
                  0.0168876628296,
                  0.00691852415316,
                ],
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

from CMGTools.H2TauTau.proto.samples.run2012.tauEle_Sync_Colin import *

#########################################################################################

for mc in MC_list:
    mc.puFileMC = puFileMC
    mc.puFileData = puFileData
    if mc.name.find('DYJets')!=-1:
        mc.fakes = True
for emb in embed_list:
    emb.puFileData = None
    emb.puFileMC = None

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
    WNJetsAna, 
    higgsWeighter, 
    vbfAna,
    pileUpAna,
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
    selectedComponents = copy.copy(data_list)
    selectedComponents.extend(embed_list)
    selectedComponents.extend(MC_list[0:3])
    for comp in selectedComponents:
        comp.splitFactor = 1
        comp.files = comp.files[:3]



config = cfg.Config( components = selectedComponents,
                     sequence = sequence )

printComps(config.components, True)
