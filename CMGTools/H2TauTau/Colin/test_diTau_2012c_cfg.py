import copy
import os 
import CMGTools.RootTools.fwlite.Config as cfg
from CMGTools.H2TauTau.triggerMap import pathsAndFilters

runOnEmbedded = True
runOnData = False
runOnMC = False
use1prong = False

#puFileDir = os.environ['CMSSW_BASE'] + '/src/CMGTools/RootTools/data/vertexWeight'
#puFileMC = '/'.join([puFileDir, 'Pileup_Summer12MC53X.true.root'])
#puFileData = '/'.join([puFileDir, 'Pileup_2012HCP_190456_203002.true.root'])
puFileMC = '/afs/cern.ch/user/a/agilbert/public/HTT_Pileup/28-09-12/MC_Summer12_PU_S10-600bins.root'
puFileData = '/afs/cern.ch/user/a/agilbert/public/HTT_Pileup/28-09-12/Data_Pileup_2012_HCP-600bins.root'

mc_vertexWeight = None
mc_tauEffWeight_mc = 'effLooseTau15MC'
#mc_tauEffWeight='eff2012IsoTau5_1fb'
#mc_tauEffWeight='eff2012IsoTau5fb'
mc_tauEffWeight='eff2012IsoTau12fb'
#mc_tauEffWeight='eff2012IsoTau5fbUp'
#mc_tauEffWeight='eff2012IsoTau5fbDown'
#mc_tauEffWeight='eff2012IsoTau5fbUpSlope'
#mc_tauEffWeight='eff2012IsoTau5fbDownSlope'
#mc_tauEffWeight='eff2012IsoTau5fbUpPlateau'
#mc_tauEffWeight='eff2012IsoTau5fbDownPlateau'
#mc_tauEffWeight='eff2012IsoTau5fbCrystalBall'
#mc_tauEffWeight='eff2012IsoTau5fbFitFrom30'
#mc_jetEffWeight='eff2012Jet30'
#mc_jetEffWeight='eff2012Jet5fb'
mc_jetEffWeight='eff2012Jet12fb'

if use1prong:
    mc_tauEffWeight='eff2012IsoTau1prong12fb'

triggerAna = cfg.Analyzer(
    'TriggerAnalyzer',
    verbose = False
    )

TauTauAna = cfg.Analyzer(
    'TauTauAnalyzer',
    pt1 = 0,
    pt2 = 0,
    iso1 = 999,
    iso2 = 999,
    eta1 = 999,
    eta2 = 999,
    m_min = 0,
    m_max = 99999,
    diLeptonCutString = '',
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
    vbfMvaWeights = os.environ['CMSSW_BASE'] + '/src/CMGTools/H2TauTau/data/VBFMVA_BDTG_HCP_52X.weights.xml',
    jetCol = 'cmgPFJetSel',
    jetPt = 20,
    jetEta = 4.7,
    cjvPtCut = 30.,
    btagSFseed = 123456,
    relaxJetId = False,
    **vbfKwargs
    )

treeProducer = cfg.Analyzer(
    'H2TauTauTreeProducerTauTau',
    )

#########################################################################################

from CMGTools.H2TauTau.proto.samples.run2012.diTau_Oct24 import * 

#########################################################################################

mc_jet_scale = 1.
mc_jet_smear = 0.
for mc in MC:
    # could handle the weights in the same way
    mc.jetScale = mc_jet_scale
    mc.jetSmear = mc_jet_smear
    mc.puFileData = puFileData
    mc.puFileMC = puFileMC

for emb in embedded_2012:
    emb.puFileData = puFileData
    emb.puFileMC = puFileData

if use1prong:
  for data in data_2012A:
    data.triggers = data_triggers_1prong_2012A
  for data in data_2012B:
    data.triggers = data_triggers_1prong_2012B
  for data in data_2012C:
    data.triggers = data_triggers_1prong_2012C
  for data in data_2012D:
    data.triggers = data_triggers_1prong_2012D


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
    #selectedComponents = [data_Run2012A_PromptReco_v1]
    #selectedComponents = [data_Run2012B_PromptReco_v1]
    #selectedComponents = [data_Run2012C_PromptReco_v1]
    #selectedComponents = [data_Run2012C_PromptReco_v2]
    #selectedComponents = [data_Run2012D_PromptReco_v1]
if runOnMC:
    #selectedComponents = [DYJets, DY1Jets, DY2Jets, DY3Jets, DY4Jets, TTJets, WW, WZ, ZZ]
    #selectedComponents = [WJets, W1Jets, W2Jets, W3Jets, W4Jets, T_tW, Tbar_tW]
    #selectedComponents = [ Higgsgg110 , Higgsgg115 , Higgsgg120 , Higgsgg125 , Higgsgg130 , Higgsgg135 , Higgsgg140 , Higgsgg145 ,
    #                    HiggsVBF110, HiggsVBF115, HiggsVBF120, HiggsVBF125, HiggsVBF130, HiggsVBF135, HiggsVBF140, HiggsVBF145, 
    #                    HiggsVH110 , HiggsVH115 , HiggsVH120 , HiggsVH125 , HiggsVH130 , HiggsVH135 , HiggsVH140 , HiggsVH145 ]
    #selectedComponents = [ HiggsSUSYBB80, HiggsSUSYBB90, HiggsSUSYBB100, HiggsSUSYBB110, HiggsSUSYBB120, HiggsSUSYBB130, HiggsSUSYBB140, HiggsSUSYBB160, HiggsSUSYBB180, HiggsSUSYBB200, HiggsSUSYBB250, HiggsSUSYBB300, HiggsSUSYBB350, HiggsSUSYBB400, HiggsSUSYBB450, HiggsSUSYBB500, HiggsSUSYBB600, HiggsSUSYBB700, HiggsSUSYBB800, HiggsSUSYBB1000,
    #                       HiggsSUSYGluGlu80, HiggsSUSYGluGlu90, HiggsSUSYGluGlu100, HiggsSUSYGluGlu110, HiggsSUSYGluGlu120, HiggsSUSYGluGlu130, HiggsSUSYGluGlu140, HiggsSUSYGluGlu160, HiggsSUSYGluGlu180, HiggsSUSYGluGlu200, HiggsSUSYGluGlu250, HiggsSUSYGluGlu300, HiggsSUSYGluGlu350, HiggsSUSYGluGlu400, HiggsSUSYGluGlu450, HiggsSUSYGluGlu500, HiggsSUSYGluGlu600, HiggsSUSYGluGlu700, HiggsSUSYGluGlu800, HiggsSUSYGluGlu1000 ]
    #selectedComponents = [ DY3Jets, W4Jets, HiggsVH125, HiggsSUSYBB500, HiggsSUSYBB130, HiggsSUSYBB350, HiggsSUSYBB250, HiggsSUSYBB400, HiggsSUSYBB700, HiggsSUSYGluGlu140, HiggsSUSYGluGlu120 ]
    selectedComponents = [ WJets, W1Jets ]
    #selectedComponents = [ HiggsVH130, ZZ , HiggsSUSYBB110 , HiggsSUSYBB130 , HiggsSUSYBB140 , HiggsSUSYBB600 , HiggsSUSYBB1000 , HiggsSUSYGluGlu100, HiggsSUSYGluGlu600 , Higgsgg110 , Higgsgg130 , Higgsgg135 , Higgsgg140 , Higgsgg145 , HiggsVBF110 , HiggsVBF115 , HiggsVBF120 , HiggsVBF125 , HiggsVBF130 , HiggsVBF135 , HiggsVBF140 , HiggsVBF145 ]
    #selectedComponents = [ Higgsgg120 ]
    #selectedComponents = [QCD80,QCD120]
    #selectedComponents = [T_s, T_t, Tbar_s, Tbar_t]
if runOnEmbedded:
    selectedComponents = [embed_Run2012D_PromptReco_v1]
    #selectedComponents = embedded_2012

print [c.name for c in selectedComponents]

if runOnMC:
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
    treeProducer
   ] )
elif runOnEmbedded:
  sequence = cfg.Sequence( [
    #triggerAna,
    vertexAna,
    TauTauAna,
    vbfAna,
    pileUpAna,
    embedWeighter, 
    tau1Weighter, 
    tau2Weighter,
    jetWeighter,
    treeProducer
   ] )
else:
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
    treeProducer
   ] )

if use1prong:
    sequence.remove(jetWeighter)

DYJets.fakes = True
DYJets.splitFactor = 50
DY1Jets.splitFactor = 50
DY2Jets.splitFactor = 50
DY3Jets.splitFactor = 50
DY4Jets.splitFactor = 50
WJets.fakes = True
WJets.splitFactor = 50
W1Jets.splitFactor = 50
W2Jets.splitFactor = 50
W3Jets.splitFactor = 50
W4Jets.splitFactor = 50
T_s.splitFactor = 50 
T_t.splitFactor = 50 
T_tW.splitFactor = 50 
Tbar_s.splitFactor = 50 
Tbar_t.splitFactor = 50 
Tbar_tW.splitFactor = 50 
TTJets.splitFactor = 50 
WW.splitFactor = 10
WZ.splitFactor = 10
ZZ.splitFactor = 10
#QCD.splitFactor = 50
QCD80.splitFactor = 50
QCD120.splitFactor = 50
data_Run2012A_PromptReco_v1.splitFactor = 50
data_Run2012B_PromptReco_v1.splitFactor = 200
data_Run2012C_PromptReco_v1.splitFactor = 50
data_Run2012C_PromptReco_v2.splitFactor = 200
data_Run2012D_PromptReco_v1.splitFactor = 200
embed_Run2012A_PromptReco_v1.splitFactor = 50
embed_Run2012B_PromptReco_v1.splitFactor = 50
embed_Run2012C_PromptReco_v1.splitFactor = 50
embed_Run2012C_PromptReco_v2.splitFactor = 50
embed_Run2012D_PromptReco_v1.splitFactor = 50
Higgsgg110.splitFactor = 20
HiggsVBF110.splitFactor = 20
HiggsVH110.splitFactor = 10
Higgsgg115.splitFactor = 20
HiggsVBF115.splitFactor = 20
HiggsVH115.splitFactor = 10
Higgsgg120.splitFactor = 50
HiggsVBF120.splitFactor = 50
HiggsVH120.splitFactor = 10
Higgsgg125.splitFactor = 20
HiggsVBF125.splitFactor = 20
HiggsVH125.splitFactor = 10
Higgsgg130.splitFactor = 20
HiggsVBF130.splitFactor = 20
HiggsVH130.splitFactor = 10
Higgsgg135.splitFactor = 20
HiggsVBF135.splitFactor = 20
HiggsVH135.splitFactor = 10
Higgsgg140.splitFactor = 20
HiggsVBF140.splitFactor = 20
HiggsVH140.splitFactor = 10
Higgsgg145.splitFactor = 20
HiggsVBF145.splitFactor = 20
HiggsVH145.splitFactor = 10
HiggsSUSYBB80.splitFactor = 10
HiggsSUSYBB90.splitFactor = 10
HiggsSUSYBB100.splitFactor = 10
HiggsSUSYBB110.splitFactor = 10
HiggsSUSYBB120.splitFactor = 10
HiggsSUSYBB130.splitFactor = 10
HiggsSUSYBB140.splitFactor = 10
HiggsSUSYBB160.splitFactor = 10
HiggsSUSYBB180.splitFactor = 10
HiggsSUSYBB200.splitFactor = 10
HiggsSUSYBB250.splitFactor = 10
HiggsSUSYBB300.splitFactor = 10
HiggsSUSYBB350.splitFactor = 10
HiggsSUSYBB400.splitFactor = 10
HiggsSUSYBB450.splitFactor = 10
HiggsSUSYBB500.splitFactor = 10
HiggsSUSYBB600.splitFactor = 10
HiggsSUSYBB700.splitFactor = 10
HiggsSUSYBB800.splitFactor = 10
HiggsSUSYBB1000.splitFactor = 10
HiggsSUSYGluGlu80.splitFactor = 10
HiggsSUSYGluGlu90.splitFactor = 10
HiggsSUSYGluGlu100.splitFactor = 10
HiggsSUSYGluGlu110.splitFactor = 10
HiggsSUSYGluGlu120.splitFactor = 10
HiggsSUSYGluGlu130.splitFactor = 10
HiggsSUSYGluGlu140.splitFactor = 10
HiggsSUSYGluGlu160.splitFactor = 10
HiggsSUSYGluGlu180.splitFactor = 10
HiggsSUSYGluGlu200.splitFactor = 10
HiggsSUSYGluGlu250.splitFactor = 10
HiggsSUSYGluGlu300.splitFactor = 10
HiggsSUSYGluGlu350.splitFactor = 10
HiggsSUSYGluGlu400.splitFactor = 10
HiggsSUSYGluGlu450.splitFactor = 10
HiggsSUSYGluGlu500.splitFactor = 10
HiggsSUSYGluGlu600.splitFactor = 10
HiggsSUSYGluGlu700.splitFactor = 10
HiggsSUSYGluGlu800.splitFactor = 10
HiggsSUSYGluGlu1000.splitFactor = 10

test = 0
if test==1:
    #DYJets.files = ["~/workspace/ditau/diTau_fullsel_tree_CMG.root"]
    #comp = HiggsSUSYBB1000
    #comp = DYJets
    #comp = data_Run2012C_PromptReco_v1
    #comp = GluGluToHToWWTo2LAndTau2Nu_M_125
    comp = embed_Run2012D_PromptReco_v1
    #comp = HiggsSUSYBB300
    #comp = Higgsgg125
    selectedComponents = [comp]
    comp.splitFactor = 1
    comp.files = comp.files[:1]
elif test==2:
    for comp in selectedComponents:
     comp.splitFactor = 1
     comp.files = comp.files[:1]

    
config = cfg.Config( components = selectedComponents,
                     sequence = sequence )
