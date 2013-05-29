import copy
import os 
import CMGTools.RootTools.fwlite.Config as cfg
from CMGTools.RootTools.fwlite.Config import printComps

from CMGTools.H2TauTau.triggerMap import pathsAndFilters
from CMGTools.H2TauTau.proto.weights.weighttable import mu_id_taumu_2012, mu_iso_taumu_2012
from CMGTools.H2TauTau.proto.samples.sampleShift import selectShift
from CMGTools.RootTools.RootTools import * 

# 'Nom', 'Up', 'Down', or None
shift = None
# 1.0, 1.03, 0.97
tauScaleShift = 1.0

puFileDir = os.environ['CMSSW_BASE'] + '/src/CMGTools/RootTools/data/Reweight/2012'

# mine: 
# puFileMC = '/'.join([puFileDir, 'MyMCPileupHistogram_true.root'])
# puFileData = '/'.join([puFileDir, 'MyDataPileupHistogram_true_AB_start_196509.root'])

# andrew ICHEP
# puFileMC = '/afs/cern.ch/user/a/agilbert/public/HTT_Pileup/12-09-12/MC_Summer12_PU_S7.root'
# puFileData = '/afs/cern.ch/user/a/agilbert/public/HTT_Pileup/12-09-12/Data_Pileup_2012.root'
# andrew HCP
puFileMC = '/afs/cern.ch/user/a/agilbert/public/HTT_Pileup/28-09-12/MC_Summer12_PU_S10-600bins.root'
puFileData = '/afs/cern.ch/user/a/agilbert/public/HTT_Pileup/28-09-12/Data_Pileup_2012_HCP-600bins.root'


vertexFileDir = os.environ['CMSSW_BASE'] + '/src/CMGTools/RootTools/data/Reweight/2012/Vertices'
vertexFileData = '/'.join([vertexFileDir, 'vertices_data_2012A_2012B_start_195947.root'])

mc_vertexWeight = None

# mc_tauEffWeight_mc = 'effTau2012MC53X'
# mc_muEffWeight_mc = 'eff_2012_Rebecca_TauMu_IsoMu1753XMC'
# mc_tauEffWeight = 'effTau2012ABC'
# mc_muEffWeight = 'effMu2012_Rebecca_TauMu_ABC'

mc_tauEffWeight_mc = 'effTau_muTau_MC_2012ABCD'
mc_muEffWeight_mc = 'effMu_muTau_MC_2012ABCD'
mc_tauEffWeight = 'effTau_muTau_Data_2012ABCD'
mc_muEffWeight = 'effMu_muTau_Data_2012ABCD'
    
    
eventSelector = cfg.Analyzer(
    'EventSelector',
    toSelect = [
    105104
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
    fixedWeight = 1,
    verbose = False,
    )

embedWeighter = cfg.Analyzer(
    'EmbedWeighter',
    verbose = False
    )

pileUpAna = cfg.Analyzer(
    'PileUpAnalyzer',
    true = True
    )

genErsatzAna = cfg.Analyzer(
    'GenErsatzAnalyzer',
    verbose = False
    )

TauMuAna = cfg.Analyzer(
    'TauMuAnalyzer',
    scaleShift1 = tauScaleShift,
    pt1 = 20,
    eta1 = 2.3,
    iso1 = None,
    pt2 = 20,
    eta2 = 2.1,
    iso2 = 0.1,
    m_min = 10,
    m_max = 99999,
    dR_min = 0.5,
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
    fractions = [ 0.744344,
                  0.175482,
                  0.0562994,
                  0.0169402,
                  0.0069341
                  ],
    )

WNJetsTreeAna = cfg.Analyzer(
    'WNJetsTreeAnalyzer'
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
    idWeight = mu_id_taumu_2012,
    isoWeight = mu_iso_taumu_2012    
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
    relaxJetId = False, # For jet ID studies
    jerCorr = False, # NEW!
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


# from CMGTools.H2TauTau.proto.samples.run2012.tauMu_MuRm_ColinOct9 import * 
# from CMGTools.H2TauTau.proto.samples.run2012.tauMu_ColinOct10 import * 
#from CMGTools.H2TauTau.proto.samples.run2012.tauMu_Sync_Colin import * 
from CMGTools.H2TauTau.proto.samples.run2012.WJets_JanMay29 import * 

#########################################################################################


for mc in MC_list:
    mc.puFileMC = puFileMC
    mc.puFileData = puFileData

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
VVgroup = None # This is currently needed for the plotting script

higgs = mc_higgs

w_list = [ WJets, W1Jets, W2Jets, W3Jets, W4Jets ]

selectedComponents = [
    TTJets, DYJets, 
    data_Run2012A,
    data_Run2012B,
    data_Run2012C_v1,
    data_Run2012C_v2,
    data_Run2012D_v1,
    ]
selectedComponents.extend( w_list )
selectedComponents.extend( higgs )
selectedComponents.extend( embed_list )

selectedComponents = copy.copy( w_list ) 

sequence = cfg.Sequence( [
    # eventSelector,
    jsonAna, 
    triggerAna, #No! 
    vertexAna, 
    TauMuAna, #Yes!
    dyJetsFakeAna,
    WNJetsAna,
    higgsWeighter, 
    vbfAna, #Yes!
    pileUpAna,
    embedWeighter, 
    tauWeighter, 
    muonWeighter, 
    treeProducer, #Yes! 
    # treeProducerXCheck
   ] )


sequence = [WNJetsAna, WNJetsTreeAna]

selectedComponents = [comp for comp in selectedComponents if comp.dataset_entries > 0]

test = 2
if test==1:
    #comp = HiggsGGH125
    comp = WJets
    selectedComponents = [comp]
    comp.splitFactor = 1
    comp.files = comp.files[:10]
    # comp.files = ['tauMu_fullsel_tree_CMG.root']
elif test==2:
    for comp in selectedComponents:
        comp.splitFactor = 2
        # comp.files = comp.files[:10]


config = cfg.Config( components = selectedComponents,
                     sequence = sequence )

printComps(config.components, True)
