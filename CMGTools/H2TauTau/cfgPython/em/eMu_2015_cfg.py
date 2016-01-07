import PhysicsTools.HeppyCore.framework.config as cfg
from PhysicsTools.HeppyCore.framework.config import printComps

# Tau-tau analyzers
from CMGTools.H2TauTau.proto.analyzers.MuEleAnalyzer             import MuEleAnalyzer
from CMGTools.H2TauTau.proto.analyzers.H2TauTauTreeProducerMuEle import H2TauTauTreeProducerMuEle
from CMGTools.H2TauTau.proto.analyzers.LeptonWeighter            import LeptonWeighter
from CMGTools.H2TauTau.proto.analyzers.SVfitProducer              import SVfitProducer

from CMGTools.H2TauTau.proto.samples.spring15.connector import httConnector

# common configuration and sequence
from CMGTools.H2TauTau.htt_ntuple_base_cff import commonSequence, genAna, dyJetsFakeAna, puFileData, puFileMC, eventSelector


# local switches
syncntuple   = True
computeSVfit = False
#production   = True  # production = True run on batch, production = False run locally
production   = False  # production = True run on batch, production = False run locally

dyJetsFakeAna.channel = 'em'

### Define mu-ele specific modules

muEleAna = cfg.Analyzer(
  MuEleAnalyzer                 ,
  'MuEleAnalyzer'               ,
  pt1          = 13.            ,
  eta1         = 2.5            ,
  iso1         = 0.15           ,
  looseiso1    = 9999.            ,
  pt2          = 10.            ,
  eta2         = 2.4            ,
  iso2         = 0.15           ,
  looseiso2    = 9999.            ,
  m_min        = 0.             ,
  m_max        = 99999          ,
  dR_min       = 0.3            ,
  from_single_objects=True,
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
#  skimFunction = 'event.isSignal'
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
from CMGTools.RootTools.samples.ComponentCreator import ComponentCreator 
from CMGTools.RootTools.utils.splitFactor import splitFactor
from CMGTools.H2TauTau.proto.samples.spring15.triggers_muEle  import mc_triggers as mc_triggers_em


#my_connect = httConnector('htt_6mar15_manzoni_nom', 'htautau_group',
#                          '.*root', 'tt', production=production)
#my_connect.connect()
#MC_list = my_connect.MC_list

creator = ComponentCreator()
ggh160 = creator.makeMCComponent("GGH160", "/SUSYGluGluToHToTauTau_M-160_TuneCUETP8M1_13TeV-pythia8/RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v1/MINIAODSIM", "CMS", ".*root", 1.0)

MC_list = [ggh160]

split_factor = 2e4


for sample in MC_list:
    sample.triggers = mc_triggers_em
    sample.splitFactor = splitFactor(sample, split_factor)

###################################################
###              ASSIGN PU to MC                ###
###################################################
for mc in MC_list:
    mc.puFileData = puFileData
    mc.puFileMC = puFileMC

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
eventSelector.toSelect = [133381]
sequence.insert(0, eventSelector)

###################################################
###            SET BATCH OR LOCAL               ###
###################################################
if not production:
  cache                = True
#  comp                 = my_connect.mc_dict['HiggsGGH125']
  comp = ggh160
  selectedComponents   = [comp]
  comp.splitFactor     = 1
  comp.fineSplitFactor = 1
#  comp.files           = comp.files[:1]

# the following is declared in case this cfg is used in input to the
# heppy.py script
from PhysicsTools.HeppyCore.framework.eventsfwlite import Events
config = cfg.Config( components   = selectedComponents,
                     sequence     = sequence          ,
                     services     = []                ,
                     events_class = Events
                     )

printComps(config.components, True)

def modCfgForPlot(config):
  config.components = []
