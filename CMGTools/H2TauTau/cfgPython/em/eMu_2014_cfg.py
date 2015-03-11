import PhysicsTools.HeppyCore.framework.config as cfg
from PhysicsTools.HeppyCore.framework.config import printComps

# Tau-tau analyzers
from CMGTools.H2TauTau.proto.analyzers.MuEleAnalyzer             import MuEleAnalyzer
from CMGTools.H2TauTau.proto.analyzers.H2TauTauTreeProducerMuEle import H2TauTauTreeProducerMuEle
from CMGTools.H2TauTau.proto.analyzers.LeptonWeighter            import LeptonWeighter
from CMGTools.H2TauTau.proto.analyzers.SVfitProducer              import SVfitProducer

# common configuration and sequence
from CMGTools.H2TauTau.htt_ntuple_base_cff import commonSequence, genAna, dyJetsFakeAna, puFileData, puFileMC, eventSelector

# local switches
syncntuple = True
computeSVfit = True

dyJetsFakeAna.channel = 'em'

### Define mu-ele specific modules

muEleAna = cfg.Analyzer(
  MuEleAnalyzer                 ,
  'MuEleAnalyzer'               ,
  pt1          = 10.            ,
  eta1         = 2.5            ,
  iso1         = 0.15           ,
  looseiso1    = 10.            ,
  pt2          = 10.            ,
  eta2         = 2.5            ,
  iso2         = 0.15           ,
  looseiso2    = 10.            ,
  m_min        = 10             ,
  m_max        = 99999          ,
  dR_min       = 0.5            ,
#   triggerMap   = pathsAndFilters,
  jetPt        = 30.            ,
  jetEta       = 4.7            ,
  relaxJetId   = False          ,
  verbose      = False          ,
  )

muonWeighter = cfg.Analyzer(
  LeptonWeighter                  ,
  name        ='LeptonWeighter_mu',
  effWeight   = None              ,
  effWeightMC = None              ,
  lepton      = 'leg1'            ,
  verbose     = False             ,
  disable     = True              ,
  idWeight    = None              ,
  isoWeight   = None
  )

eleWeighter = cfg.Analyzer(
  LeptonWeighter                   ,
  name        ='LeptonWeighter_ele',
  effWeight   = None               ,
  effWeightMC = None               ,
  lepton      = 'leg2'             ,
  verbose     = False              ,
  disable     = True               ,
  idWeight    = None               ,
  isoWeight   = None
  )

treeProducer = cfg.Analyzer(
  H2TauTauTreeProducerMuEle         ,
  name = 'H2TauTauTreeProducerMuEle'
  )

syncTreeProducer = cfg.Analyzer(
  H2TauTauTreeProducerMuEle                     ,
  name         = 'H2TauTauSyncTreeProducerMuEle',
  varStyle     = 'sync'                         ,
  skimFunction = 'event.isSignal'
  )

svfitProducer = cfg.Analyzer(
  SVfitProducer                ,
  name        = 'SVfitProducer',
  # integration = 'VEGAS'        ,
  integration = 'MarkovChain'  ,
  # verbose     = True           ,
  # order       = '21'           , # muon first, tau second
  l1type      = 'muon'         ,
  l2type      = 'ele'
  )

###################################################
### CONNECT SAMPLES TO THEIR ALIASES AND FILES  ###
###################################################
# from CMGTools.H2TauTau.proto.samples.phys14.muEle_Ric_Jan27 import *
from CMGTools.H2TauTau.proto.samples.phys14.htt_Ric_Mar9 import MC_list, mc_dict, mc_triggers_em

###################################################
###              ASSIGN PU to MC                ###
###################################################
for mc in MC_list:
    mc.puFileData = puFileData
    mc.puFileMC = puFileMC

###################################################
###              ASSIGN TRIGGER                 ###
###################################################
for mc in MC_list:
    mc.triggers = mc_triggers_em

###################################################
###             SET COMPONENTS BY HAND          ###
###################################################
selectedComponents = MC_list
# selectedComponents = mc_dict['HiggsGGH125']
# for c in selectedComponents : c.splitFactor *= 5

###################################################
###                  SEQUENCE                   ###
###################################################
sequence = commonSequence
sequence.insert(sequence.index(genAna), muEleAna)
sequence.append(muonWeighter)
sequence.append(eleWeighter)
if computeSVfit:
    sequence.append(svfitProducer)
sequence.append(treeProducer)
if syncntuple:
    sequence.append(syncTreeProducer)

###################################################
###             CHERRY PICK EVENTS              ###
###################################################
# eventSelector.toSelect = []
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
test = 0 # test = 0 run on batch, test = 1 run locally
if test == 1 :
  cache              = True
  comp               = mc_dict['HiggsGGH125']
  #comp.triggers      = [] # empty for now
  selectedComponents = [comp]
  comp.splitFactor     = 1
#   comp.fineSplitFactor = 5
  comp.files         = comp.files[:1]

# the following is declared in case this cfg is used in input to the
# heppy.py script
from PhysicsTools.HeppyCore.framework.eventsfwlite import Events
config = cfg.Config( components   = selectedComponents,
                     sequence     = sequence          ,
                     services     = []                ,
                     events_class = Events
                     )

# printComps(config.components, True)
#
# def modCfgForPlot(config):
#     config.components = []
