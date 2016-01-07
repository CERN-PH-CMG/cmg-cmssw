import PhysicsTools.HeppyCore.framework.config as cfg
from PhysicsTools.HeppyCore.framework.config import printComps
from PhysicsTools.HeppyCore.framework.heppy_loop import getHeppyOption

# Tau-tau analyzers
from CMGTools.H2TauTau.proto.analyzers.TauTauAnalyzer             import TauTauAnalyzer
from CMGTools.H2TauTau.proto.analyzers.H2TauTauTreeProducerTauTau import H2TauTauTreeProducerTauTau
from CMGTools.H2TauTau.proto.analyzers.TauDecayModeWeighter       import TauDecayModeWeighter
from CMGTools.H2TauTau.proto.analyzers.LeptonWeighter             import LeptonWeighter
from CMGTools.H2TauTau.proto.analyzers.SVfitProducer              import SVfitProducer

# common configuration and sequence
from CMGTools.H2TauTau.htt_ntuple_base_cff import commonSequence, genAna, dyJetsFakeAna, puFileData, puFileMC, eventSelector

# Get all heppy options; set via '-o production' or '-o production=True'

# production = True run on batch, production = False (or unset) run locally
production = getHeppyOption('production')

# local switches
syncntuple   = True
computeSVfit = False
pick_events  = False

dyJetsFakeAna.channel = 'tt'

### Define tau-tau specific modules

tauTauAna = cfg.Analyzer(
  class_object        = TauTauAnalyzer                            ,
  name                = 'TauTauAnalyzer'                          ,
  pt1                 = 45                                        ,
  eta1                = 2.1                                       ,
  iso1                = 1.                                        ,
  looseiso1           = 999999999.                                ,
  pt2                 = 45                                        ,
  eta2                = 2.1                                       ,
  iso2                = 1.                                        ,
  looseiso2           = 999999999.                                ,
  isolation           = 'byCombinedIsolationDeltaBetaCorrRaw3Hits',
  m_min               = 10                                        ,
  m_max               = 99999                                     ,
  dR_min              = 0.5                                       ,
  jetPt               = 30.                                       ,
  jetEta              = 4.7                                       ,
  relaxJetId          = False                                     ,
  verbose             = False                                     ,
  from_single_objects = True                                      ,
  )

tauDecayModeWeighter = cfg.Analyzer(
  TauDecayModeWeighter   ,
  'TauDecayModeWeighter' ,
  legs = ['leg1', 'leg2'],
  )

tau1Weighter = cfg.Analyzer(
  LeptonWeighter                    ,
  name        ='LeptonWeighter_tau1',
  effWeight   = None                ,
  effWeightMC = None                ,
  lepton      = 'leg1'              ,
  verbose     = False               ,
  disable     = True                ,
  )

tau2Weighter = cfg.Analyzer(
  LeptonWeighter                    ,
  name        ='LeptonWeighter_tau2',
  effWeight   = None                ,
  effWeightMC = None                ,
  lepton      = 'leg2'              ,
  verbose     = False               ,
  disable     = True                ,
  )

treeProducer = cfg.Analyzer(
  H2TauTauTreeProducerTauTau         ,
  name = 'H2TauTauTreeProducerTauTau'
  )

syncTreeProducer = cfg.Analyzer(
  H2TauTauTreeProducerTauTau                     ,
  name         = 'H2TauTauSyncTreeProducerTauTau',
  varStyle     = 'sync'                          ,
  #skimFunction = 'event.isSignal' #don't cut out any events from the sync tuple
  )

svfitProducer = cfg.Analyzer(
  SVfitProducer,
  name        = 'SVfitProducer',
  # integration = 'VEGAS'        ,
  integration = 'MarkovChain'  ,
  # verbose     = True           ,
  # order       = '21'           , # muon first, tau second
  l1type      = 'tau'          ,
  l2type      = 'tau'
  )

###################################################
### CONNECT SAMPLES TO THEIR ALIASES AND FILES  ###
###################################################
from CMGTools.RootTools.utils.splitFactor import splitFactor
from CMGTools.RootTools.samples.ComponentCreator import ComponentCreator
from CMGTools.RootTools.samples.samples_13TeV_74X import TT_pow, DYJetsToLL_M50, WJetsToLNu, WJetsToLNu_HT100to200, WJetsToLNu_HT200to400, WJetsToLNu_HT400to600, WJetsToLNu_HT600toInf, QCD_Mu15, WWTo2L2Nu, ZZp8, WZp8, SingleTop
from CMGTools.RootTools.samples.samples_13TeV_DATA2015 import SingleMuon_Run2015B_17Jul, SingleMuon_Run2015B
from CMGTools.H2TauTau.proto.samples.spring15.triggers_tauTau import mc_triggers as mc_triggers_tt

creator = ComponentCreator()

ggh160   = creator.makeMCComponent  ("GGH160", 
                                     "/SUSYGluGluToHToTauTau_M-160_TuneCUETP8M1_13TeV-pythia8/RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v1/MINIAODSIM", 
                                     "CMS", 
                                     ".*root", 
                                     1.0)
run2015B = creator.makeDataComponent("DataRun2015B", 
                                     "/Tau/Run2015B-PromptReco-v1/MINIAOD", 
                                     "CMS", 
                                     ".*root", 
                                     "/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions15/13TeV/DCSOnly/json_DCSONLY_Run2015B.txt"
                                     )

MC_list = [ggh160]

run2015B.intLumi  = '2.0' # in pb
run2015B.triggers = mc_triggers_tt

split_factor = 1e5

for sample in MC_list:
    sample.triggers = mc_triggers_tt
    sample.splitFactor = splitFactor(sample, split_factor)
    
data_list = [run2015B]

###################################################
###              ASSIGN PU to MC                ###
###################################################
for mc in MC_list:
    mc.puFileData = puFileData
    mc.puFileMC = puFileMC

###################################################
###             SET COMPONENTS BY HAND          ###
###################################################
selectedComponents = MC_list #+ data_list
# selectedComponents = mc_dict['HiggsGGH125']
for c in selectedComponents : 
    c.splitFactor *= 10
#     c.fineSplitFactor = 4

###################################################
###                  SEQUENCE                   ###
###################################################
sequence = commonSequence
sequence.insert(sequence.index(genAna), tauTauAna)
sequence.append(tauDecayModeWeighter)
sequence.append(tau1Weighter)
sequence.append(tau2Weighter)
if computeSVfit:
    sequence.append(svfitProducer)
sequence.append(treeProducer)
if syncntuple:
    sequence.append(syncTreeProducer)

###################################################
###             CHERRY PICK EVENTS              ###
###################################################
if pick_events:

    import csv
    fileName = '/afs/cern.ch/work/m/manzoni/diTau2015/CMSSW_7_4_3/src/CMGTools/H2TauTau/cfgPython/2015-sync/Imperial.csv'
#     fileName = '/afs/cern.ch/work/m/manzoni/diTau2015/CMSSW_7_4_3/src/CMGTools/H2TauTau/cfgPython/2015-sync/CERN.csv'
    f = open(fileName, 'rb')
    reader = csv.reader(f)
    evtsToPick = []

    for i, row in enumerate(reader):
        evtsToPick += [int(j) for j in row]

    eventSelector.toSelect = evtsToPick
    sequence.insert(0, eventSelector)

###################################################
###            SET BATCH OR LOCAL               ###
###################################################
if not production:
  cache                = True
#   comp                 = my_connect.mc_dict['HiggsGGH125']
#   comp                 = DYJetsToLL_M50
#   comp                 = run2015B
  comp                 = ggh160
  selectedComponents   = [comp]
  comp.splitFactor     = 1
  comp.fineSplitFactor = 1
#   comp.files           = comp.files[:1]
#   for comp in selectedComponents:
#     comp.splitFactor     = 1
#     comp.fineSplitFactor = 4
    


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
