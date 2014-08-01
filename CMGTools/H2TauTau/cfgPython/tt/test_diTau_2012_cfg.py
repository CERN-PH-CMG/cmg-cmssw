import copy
import os 
import CMGTools.RootTools.fwlite.Config as cfg
from CMGTools.H2TauTau.triggerMap import pathsAndFilters

runOnEmbedded = False
runOnData = False
runOnMC = False

puFileDir = os.environ['CMSSW_BASE'] + '/src/CMGTools/RootTools/data/Reweight/2012'
puFileMC = '/'.join([puFileDir, 'MyMCPileupHistogram_true.root'])
puFileData = '/'.join([puFileDir, 'MyDataPileupHistogram_true_AB_190456_196531.root'])

mc_vertexWeight = None
mc_tauEffWeight_mc = 'effLooseTau15MC'
#mc_tauEffWeight='eff2012IsoTau5_1fb'
mc_tauEffWeight='eff2012IsoTau5fb'
#mc_tauEffWeight='eff2012IsoTau5fbUp'
#mc_tauEffWeight='eff2012IsoTau5fbDown'
#mc_tauEffWeight='eff2012IsoTau5fbUpSlope'
#mc_tauEffWeight='eff2012IsoTau5fbDownSlope'
#mc_tauEffWeight='eff2012IsoTau5fbUpPlateau'
#mc_tauEffWeight='eff2012IsoTau5fbDownPlateau'
#mc_tauEffWeight='eff2012IsoTau5fbCrystalBall'
#mc_tauEffWeight='eff2012IsoTau5fbFitFrom30'
#mc_jetEffWeight='eff2012Jet30'
mc_jetEffWeight='eff2012Jet5fb'

triggerAna = cfg.Analyzer(
    'TriggerAnalyzer',
    verbose = False
    )

TauTauAna = cfg.Analyzer(
    'TauTauAnalyzer',
    pt1 = 20,
    pt2 = 20,
    iso1 = 999,
    iso2 = 999,
    eta1 = 999,
    eta2 = 999,
    m_min = 10,
    m_max = 99999,
    diLeptonCutString = 'cuts_baseline',
    # No trigger matching for the moment since MC only contains too high threshold trigger
    triggerMap = pathsAndFilters
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
    effWeight = mc_tauEffWeight,
    effWeightMC = mc_tauEffWeight_mc,
    lepton = 'leg2',
    verbose = False
    )

jetWeighter = cfg.Analyzer(
    'JetWeighter_jet1',
    effWeight = mc_jetEffWeight,
    effWeightMC = mc_tauEffWeight_mc,
    verbose = False
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
    jetCol = 'cmgPFJetSel',
    jetPt = 30,
    jetEta = 4.5,
    vbfMvaWeights = os.environ['CMSSW_BASE'] + '/src/CMGTools/H2TauTau/data/VBFMVA_BDTG.weights.5XX.xml',
    **vbfKwargs
    )

treeProducer = cfg.Analyzer(
    'H2TauTauTreeProducerTauTau',
    )

#########################################################################################

from CMGTools.H2TauTau.proto.samples.run2012.diTau_June5 import * 

#########################################################################################

mc_jet_scale = 1.
mc_jet_smear = 0.
for mc in MC:
    # could handle the weights in the same way
    mc.jetScale = mc_jet_scale
    mc.jetSmear = mc_jet_smear
    mc.puFileData = puFileData
    mc.puFileMC = puFileMC

selectedComponents = data_2012 + embedded_2012 + [DYJets, WJets, W3Jets, TTJets, WW, WZ, ZZ]
selectedComponents += [ Higgsgg110 , Higgsgg115 , Higgsgg120 , Higgsgg125 , Higgsgg130 , Higgsgg135 , Higgsgg140 , Higgsgg145 ,
                        HiggsVBF110, HiggsVBF115, HiggsVBF120, HiggsVBF125, HiggsVBF130, HiggsVBF135, HiggsVBF140, HiggsVBF145, 
                        HiggsVH110 , HiggsVH115 , HiggsVH120 , HiggsVH125 , HiggsVH130 , HiggsVH135 , HiggsVH140 , HiggsVH145 ]
#selectedComponents += [QCD50,QCD80]
#selectedComponents += [ GluGluToHToWWTo2LAndTau2Nu_M_125, VBF_HToWWTo2LAndTau2Nu_M_125 ]
#selectedComponents += [ GluGluToHToWWTo2Tau2Nu_M_120, GluGluToHToWWToTauNuQQ_M_120, VBF_HToWWToTauNuQQ_M_120, VBF_HToWWTo2Tau2Nu_M_120,
#                        GluGluToHToZZTo2L2Nu_M_125, GluGluToHToZZTo2L2Q_M_125, VBF_ToHToZZTo2l2q_M_130, VBF_ToHToZZTo2L2NU_M_125]

if runOnData:
    selectedComponents = data_2012
if runOnMC:
    selectedComponents = [DYJets, TTJets, WW, WZ, ZZ] #WJets, W3Jets, 
    selectedComponents += [ Higgsgg110 , Higgsgg115 , Higgsgg120 , Higgsgg125 , Higgsgg130 , Higgsgg135 , Higgsgg140 , Higgsgg145 ,
                        HiggsVBF110, HiggsVBF115, HiggsVBF120, HiggsVBF125, HiggsVBF130, HiggsVBF135, HiggsVBF140, HiggsVBF145, 
                        HiggsVH110 , HiggsVH115 , HiggsVH120 , HiggsVH125 , HiggsVH130 , HiggsVH135 , HiggsVH140 , HiggsVH145 ]
#    selectedComponents = [QCD50,QCD80]
if runOnEmbedded:
    selectedComponents = embedded_2012

print [c.name for c in selectedComponents]

if runOnMC or runOnEmbedded:
  sequence = cfg.Sequence( [
    TauTauAna,
    vbfAna,
    pileUpAna,
    embedWeighter, 
    vertexAna,
    tau1Weighter, 
    tau2Weighter,
    jetWeighter,
    treeProducer
   ] )
else:
  sequence = cfg.Sequence( [
    triggerAna,
    TauTauAna,
    vbfAna,
    pileUpAna,
    embedWeighter, 
    vertexAna,
    tau1Weighter, 
    tau2Weighter, 
    jetWeighter,
    treeProducer
   ] )

DYJets.fakes = True
DYJets.splitFactor = 50
WJets.fakes = True
WJets.splitFactor = 50
W3Jets.splitFactor = 50
TTJets.splitFactor = 50 
WW.splitFactor = 10
WZ.splitFactor = 10
ZZ.splitFactor = 10
#QCD.splitFactor = 50
QCD50.splitFactor = 50
QCD80.splitFactor = 50
data_Run2012A_PromptReco_v1.splitFactor = 50
data_Run2012B_PromptReco_v1.splitFactor = 50
embed_Run2012A_PromptReco_v1.splitFactor = 50
embed_Run2012B_PromptReco_v1.splitFactor = 50

test = 0
if test==1:
    comp = DYJets
    #comp = WJets
    #comp = data_Run2012A_PromptReco_v1
    #comp = GluGluToHToWWTo2LAndTau2Nu_M_125
    #comp = embed_Run2012A_PromptReco_v1
    selectedComponents = [comp]
    comp.splitFactor = 10
    comp.files = comp.files[:1]
elif test==2:
    for comp in selectedComponents:
     comp.splitFactor = 1
     comp.files = comp.files[:1]

    
config = cfg.Config( components = selectedComponents,
                     sequence = sequence )
