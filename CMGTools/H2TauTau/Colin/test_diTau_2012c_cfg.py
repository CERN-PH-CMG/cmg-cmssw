import copy
import os 
import CMGTools.RootTools.fwlite.Config as cfg
from CMGTools.H2TauTau.triggerMap import pathsAndFilters

# out of scope
runOnEmbedded       = False
runOnData           = False
runOnMC             = True
use1prong           = False
useParked           = False
MC_trigger_matching = True

puFileMC   = '/afs/cern.ch/user/a/agilbert/public/HTT_Pileup/07-01-13/MC_Summer12_PU_S10-600bins.root'
puFileData = '/afs/cern.ch/user/a/agilbert/public/HTT_Pileup/07-01-13/Data_Pileup_2012_Moriond-600bins.root'

mc_vertexWeight    = None
mc_tauEffWeight_mc = 'effLooseTau15MC'
mc_tauEffWeight    = 'eff2012IsoTau19fb'
mc_jetEffWeight    = 'eff2012Jet19fb'

if use1prong or useParked:
    mc_tauEffWeight='eff2012IsoTau1prong19fb'

triggerAna = cfg.Analyzer(
    'TriggerAnalyzer',
    verbose = False
    )
    
TauTauAna = cfg.Analyzer(
    'TauTauAnalyzer',
    pt1   = 0,
    pt2   = 0,
    iso1  = 999,
    iso2  = 999,
    eta1  = 999,
    eta2  = 999,
    m_min = 0,
    m_max = 99999,
    diLeptonCutString = '',
    # No trigger matching for the moment since MC only contains too high threshold trigger
    triggerMap = pathsAndFilters,
    jetPt  = 50.,
    jetEta = 3.0,
    relaxJetId = False,
    )

tau1Weighter = cfg.Analyzer(
    'LeptonWeighter_tau1',
    effWeight = mc_tauEffWeight,
    effWeightMC = mc_tauEffWeight_mc,
    lepton = 'leg1',
    verbose = False
    )

tau2Weighter = cfg.Analyzer(
    'LeptonWeighter_tau2',
    effWeight   = mc_tauEffWeight,
    effWeightMC = mc_tauEffWeight_mc,
    lepton      = 'leg2',
    verbose     = False
    )

jetWeighter = cfg.Analyzer(
    'JetWeighter_jet1',
    effWeight   = mc_jetEffWeight,
    effWeightMC = mc_tauEffWeight_mc,
    verbose     = False
    )

vertexAna = cfg.Analyzer(
    'VertexAnalyzer',
    goodVertices = 'offlinePrimaryVertices', # hum... collection not available in old tuples
    #goodVertices = 'goodPVFilter',
    fixedWeight = 1,
    #vertexWeight = mc_vertexWeight,
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

# defined for vbfAna and eventSorter
vbfKwargs = dict( Mjj = 400,
                  deltaEta = 4.0    
                  )

vbfAna = cfg.Analyzer(
    'VBFAnalyzer',
    vbfMvaWeights = os.environ['CMSSW_BASE'] + '/src/CMGTools/H2TauTau/data/VBFMVA_BDTG_HCP_52X.weights.xml',
    jetCol     = 'cmgPFJetSel',
    jetPt      = 30,
    jetEta     = 4.7,
    cjvPtCut   = 30.,
    btagSFseed = 123456,
    relaxJetId = False,
    **vbfKwargs
    )

treeProducer = cfg.Analyzer(
    'H2TauTauTreeProducerTauTau',
    )


treeProducerXcheck = cfg.Analyzer(
    'H2TauTauSyncTreeTauTau',
    pt20 = False
    )


#########################################################################################

from CMGTools.H2TauTau.proto.samples.run2012.diTau_Colin_Feb8 import *

#########################################################################################

mc_jet_scale = 1.
mc_jet_smear = 0.
for mc in MC_list:
    # could handle the weights in the same way
    mc.jetScale = mc_jet_scale
    mc.jetSmear = mc_jet_smear
    mc.puFileData = puFileData
    mc.puFileMC = puFileMC

# selectedComponents = MC_list
# selectedComponents = data_list
# selectedComponents = embed_list
selectedComponents = allsamples
#selectedComponents = [HiggsGGH125]
#HiggsGGH125.splitFactor=10
#HiggsGGH125.files  = getFiles('/GluGluToHToTauTau_M-125_8TeV-powheg-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/PAT_CMG_V5_8_0/HTT_Nom_Mar6_hinzmann', 'hinzmann', 'diTau.*fullsel.*root', True)
#HiggsGGH125.files  = getFiles('/GluGluToHToTauTau_M-125_8TeV-powheg-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/PAT_CMG_V5_8_0/HTT_Mar7_manzoni_Nom', 'manzoni', 'diTau.*fullsel.*root', True)


print [c.name for c in selectedComponents]

sequence = cfg.Sequence( [
    triggerAna,
    vertexAna,
    TauTauAna,
    vbfAna,
    pileUpAna,
    embedWeighter, 
    tau1Weighter, 
    tau2Weighter,
    jetWeighter,
    treeProducer,
    treeProducerXcheck,
    ] )

if runOnEmbedded:
    sequence.remove(triggerAna)
    
if use1prong or useParked:
    sequence.remove(jetWeighter)

if use1prong or useParked:
  for data in data_2012A:
    data.triggers = data_triggers_1prong_2012A
  for data in data_2012B:
    data.triggers = data_triggers_1prong_2012B
  for data in data_2012C:
    data.triggers = data_triggers_1prong_2012C
  for data in data_2012D:
    data.triggers = data_triggers_1prong_2012D

for mc in MC_list:
  if MC_trigger_matching:
    if use1prong:
      mc.triggers = ['HLT_DoubleMediumIsoPFTau35_Trk5_eta2p1_Prong1_v6']
    elif useParked:
      mc.triggers = ['HLT_DoubleMediumIsoPFTau35_Trk5_eta2p1_v6']
    else:
      mc.triggers = ['HLT_DoubleMediumIsoPFTau30_Trk5_eta2p1_Jet30_v2']
  else:
      mc.triggers = []

DYJets.fakes = True
WJets.fakes = True

test = 0    # test = 0 run on batch, test = 1 run locally
if test==1:
    #comp = HiggsSUSYBB180
    #comp = HiggsGGH120
    comp = DY3Jets
    #comp = embed_Run2012C_PromptReco_v2
    selectedComponents = [comp]
    comp.splitFactor = 1
    comp.files = comp.files[:5]
elif test==2:
    selectedComponents = []
    for comp in MC_list:
        selectedComponents.append(comp)
        comp.splitFactor = 1
        comp.files = comp.files[:1]

print [s.name for s in selectedComponents]
    
config = cfg.Config( components = selectedComponents,
                     sequence = sequence )
