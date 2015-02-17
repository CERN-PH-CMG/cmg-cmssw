import PhysicsTools.HeppyCore.framework.config as cfg
from PhysicsTools.HeppyCore.framework.config import printComps

# Tau-tau analyzers
from CMGTools.H2TauTau.proto.analyzers.TauEleAnalyzer import TauEleAnalyzer
from CMGTools.H2TauTau.proto.analyzers.H2TauTauTreeProducerTauEle import H2TauTauTreeProducerTauEle

# common configuration and sequence
from CMGTools.H2TauTau.htt_ntuple_base_cff import *

# 'Nom', 'Up', 'Down', or None
shift = None
# 1.0, 1.03, 0.97
tauScaleShift = 1.0

syncntuple = False

puFileMC = None
puFileData = None

# hlt_tauEffWeight_mc = 'effTau_eTau_MC_2012ABCDSummer13'
# hlt_tauEffWeight = 'effTau_eTau_Data_2012ABCDSummer13'
# hlt_eleEffWeight_mc = 'effEle_eTau_MC_2012ABCD'
# hlt_eleEffWeight = 'effEle_eTau_Data_2012ABCDSummer13'

hlt_tauEffWeight_mc = None
hlt_tauEffWeight = None
hlt_eleEffWeight_mc = None
hlt_eleEffWeight = None

eventSelector.toSelect = []
    
diLeptonAna.class_object = TauEleAnalyzer
diLeptonAna.name = 'TauEleAnalyzer'
diLeptonAna.scaleShift1 = tauScaleShift
diLeptonAna.pt1 = 20
diLeptonAna.eta1 = 2.3
diLeptonAna.iso1 = None
diLeptonAna.pt2 = 24
diLeptonAna.eta2 = 2.1
diLeptonAna.iso2 = 0.1
diLeptonAna.m_min = 10
diLeptonAna.m_max = 99999
diLeptonAna.dR_min = 0.5
diLeptonAna.triggerMap = pathsAndFilters
diLeptonAna.verbose = False

dyJetsFakeAna.channel = 'et'

dyLLReweighterTauEle = cfg.Analyzer(
    DYLLReweighterTauEle,
    'DYLLReweighterTauEle',
    # 2012 
    W1p0PB = 1., #1.37, # weight for 1 prong 0 Pi Barrel
    W1p0PE = 1., #1.11,
    W1p1PB = 1., #2.18,
    W1p1PE = 1., #0.47,
    verbose = False
    )

tauDecayModeWeighter = cfg.Analyzer(
    TauDecayModeWeighter,
    'TauDecayModeWeighter',
    )

tauFakeRateWeighter = cfg.Analyzer(
    TauFakeRateWeighter,
    'TauFakeRateWeighter'
    )

treeProducer.class_object = H2TauTauTreeProducerTauEle
treeProducer.name         = 'H2TauTauTreeProducerTauEle'

###################################################
### CONNECT SAMPLES TO THEIR ALIASES AND FILES  ###
###################################################
from CMGTools.H2TauTau.proto.samples.phys14.diTau_Ric_Jan27 import *

###################################################
###     ASSIGN JET SMEAR, SCALE and PU to MC    ###
###################################################
mc_jet_scale = 1.
mc_jet_smear = 0.
for mc in MC_list:
  mc.jetScale   = mc_jet_scale
  mc.jetSmear   = mc_jet_smear
  mc.puFileData = puFileData
  mc.puFileMC   = puFileMC

###################################################
###             SET COMPONENTS BY HAND          ###
###################################################
selectedComponents = allsamples
# selectedComponents  = [ ZZJetsTo4L ]
# for c in selectedComponents : c.splitFactor *= 5

###################################################
###                  SEQUENCE                   ###
###################################################
sequence = commonSequence
sequence.append( tauDecayModeWeighter ) # insert at the end
sequence.append( tauFakeRateWeighter  ) # insert at the end
sequence.insert( sequence.index(dyJetsFakeAna) + 1, dyLLReweighterTauEle ) # insert DY->LL weigher 
# RIC: off until fixed
# if not syncntuple:
#   sequence.remove( treeProducerXCheck )

###################################################
###             CHERRY PICK EVENTS              ###
###################################################
# sequence.insert(0, eventSelector)

###################################################
###    SET THE TRIGGERS TO BE USED WITH DATA    ###
###################################################
# for data in data_parked_2012:
#   data.triggers  = data_parked_triggers_2012  ## ORDER MATTERS!
#   data.triggers += data_triggers_2012         ## ORDER MATTERS!
  
###################################################
###     SET THE TRIGGERS TO BE USED WITH MC     ###
###################################################
# for mc in MC_list:
#   mc.triggers = ['HLT_DoubleMediumIsoPFTau35_Trk5_eta2p1_v6']

###################################################
###   SET THE TRIGGERS TO BE USED WITH RH EMB   ###
###################################################
# for emb in embed_list:
#   emb.triggers = emb_rechit_triggers

###################################################
###            SET BATCH OR LOCAL               ###
###################################################
test = 1 # test = 0 run on batch, test = 1 run locally
if test == 1 :
  cache              = True
  comp               = HiggsGGH125
  comp.triggers      = [] # empty for now
  selectedComponents = [comp]
  comp.splitFactor   = 1
  comp.files         = comp.files[:1]

###################################################
###                SOME PRINTOUT                ###
###################################################
print '_'*70
print 'Processing...' 
print [s.name for s in selectedComponents]

# the following is declared in case this cfg is used in input to the heppy.py script
from PhysicsTools.HeppyCore.framework.eventsfwlite import Events
config = cfg.Config( components   = selectedComponents,
                     sequence     = sequence          ,
                     services     = []                ,  
                     events_class = Events
                     )

printComps(config.components, True)

def modCfgForPlot(config):
    config.components = []

