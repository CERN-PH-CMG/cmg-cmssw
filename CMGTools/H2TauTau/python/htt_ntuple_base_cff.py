import PhysicsTools.HeppyCore.framework.config as cfg
from PhysicsTools.HeppyCore.framework.config import printComps

# import all analysers:
# Heppy analyzers
from PhysicsTools.Heppy.analyzers.core.JSONAnalyzer         import JSONAnalyzer
from PhysicsTools.Heppy.analyzers.core.EventSelector        import EventSelector
# from PhysicsTools.Heppy.analyzers.examples.TriggerAnalyzer  import TriggerAnalyzer
from PhysicsTools.Heppy.analyzers.objects.VertexAnalyzer    import VertexAnalyzer
from PhysicsTools.Heppy.analyzers.core.PileUpAnalyzer       import PileUpAnalyzer
from PhysicsTools.Heppy.analyzers.gen.GeneratorAnalyzer     import GeneratorAnalyzer

# Tau-tau analyzers
from CMGTools.H2TauTau.proto.analyzers.JetAnalyzer          import JetAnalyzer
from CMGTools.H2TauTau.proto.analyzers.EmbedWeighter        import EmbedWeighter
from CMGTools.H2TauTau.proto.analyzers.GenErsatzAnalyzer    import GenErsatzAnalyzer
from CMGTools.H2TauTau.proto.analyzers.DiLeptonAnalyzer     import DiLeptonAnalyzer
from CMGTools.H2TauTau.proto.analyzers.H2TauTauTreeProducer import H2TauTauTreeProducer
from CMGTools.H2TauTau.proto.analyzers.DYJetsFakeAnalyzer   import DYJetsFakeAnalyzer
from CMGTools.H2TauTau.proto.analyzers.WNJetsAnalyzer       import WNJetsAnalyzer
from CMGTools.H2TauTau.proto.analyzers.NJetsAnalyzer        import NJetsAnalyzer
from CMGTools.H2TauTau.proto.analyzers.HiggsPtWeighter      import HiggsPtWeighter
from CMGTools.H2TauTau.proto.analyzers.WNJetsTreeAnalyzer   import WNJetsTreeAnalyzer
from CMGTools.H2TauTau.proto.analyzers.LeptonWeighter       import LeptonWeighter
# RIC: off until fixed
# from CMGTools.H2TauTau.proto.analyzers.H2TauTauSyncTree     import H2TauTauSyncTree

from CMGTools.RootTools.analyzers.VBFSimpleAnalyzer         import VBFSimpleAnalyzer
from CMGTools.H2TauTau.triggerMap                           import pathsAndFilters

puFileMC   = '/afs/cern.ch/user/a/agilbert/public/HTT_Pileup/07-01-13/MC_Summer12_PU_S10-600bins.root'
puFileData = '/afs/cern.ch/user/a/agilbert/public/HTT_Pileup/13-09-13/Data_Pileup_2012_ReRecoPixel-600bins.root' ## new for rereco Pixel Lumi 17/9/2013

eventSelector = cfg.Analyzer(
  EventSelector  ,
  'EventSelector',
  toSelect = [
  ]
  )

jsonAna = cfg.Analyzer(
  JSONAnalyzer         ,
  name = 'JSONAnalyzer',
  )

# RIC: off until fixed
# triggerAna = cfg.Analyzer(
#   TriggerAnalyzer  ,
#   'TriggerAnalyzer',
#   verbose = False
#   )

vertexAna = cfg.Analyzer(
  VertexAnalyzer                         ,
  'VertexAnalyzer'                       ,
  goodVertices = 'offlinePrimaryVertices', 
  fixedWeight  = 1                       ,
  verbose      = False
  )

pileUpAna = cfg.Analyzer(
  PileUpAnalyzer  ,
  'PileUpAnalyzer',
  true = True
  )

genAna = GeneratorAnalyzer.defaultConfig

diLeptonAna = cfg.Analyzer(
  DiLeptonAnalyzer             ,
  'DiLeptonAnalyzer'           ,
  scaleShift1 = 1.             ,
  scaleShift2 = 1.             ,
  pt1         = 0.             ,
  eta1        = 99.            ,
  iso1        = None           ,
  pt2         = 0.             ,
  eta2        = 0.             ,
  iso2        = 0.1            ,
  m_min       = 10             ,
  m_max       = 99999          ,
  dR_min      = 0.5            ,
  triggerMap  = pathsAndFilters,
  verbose     = False           
  )
    
dyJetsFakeAna = cfg.Analyzer(
  DYJetsFakeAnalyzer  ,
  'DYJetsFakeAnalyzer',
  channel = ''        ,
  )

jetAna = cfg.Analyzer(
  JetAnalyzer                 ,
  'JetAnalyzer'               ,
  jetCol      = 'slimmedJets' , # <- These are CHS jets
  #jetCol      = 'patJetsAK4PF', # <- These are plain PF jets
  jetPt       = 20.           ,
  jetEta      = 4.7           ,
  btagSFseed  = 123456        ,
  relaxJetId  = False         , 
  jerCorr     = False         ,
  #jesCorr     = 1.            ,
  puJetIDDisc = 'pileupJetIdFull:full53xDiscriminant',
  )

# defined for vbfAna and eventSorter
vbfKwargs = dict( Mjj = 500, deltaEta = 3.5 )

vbfAna = cfg.Analyzer(
  VBFSimpleAnalyzer            ,
  'VBFSimpleAnalyzer'          ,
#   vbfMvaWeights = os.environ['CMSSW_BASE'] + '/src/CMGTools/H2TauTau/data/VBFMVA_BDTG_HCP_52X.weights.xml',
#   jetCol        = 'cmgPFJetSel',
  jetPt         = 30.          ,
  looseJetPt    = 20           ,
  jetEta        = 4.7          ,
  cjvPtCut      = 30.          ,
  btagSFseed    = 123456       ,
  relaxJetId    = False        ,
  **vbfKwargs                  
  )

embedWeighter = cfg.Analyzer(
  EmbedWeighter            ,
  name     ='EmbedWeighter',
  isRecHit = False         ,
  verbose  = False
  )

NJetsAna = cfg.Analyzer(
  NJetsAnalyzer   ,
  'NJetsAnalyzer' ,
  fillTree = True ,
  verbose  = False
  )

leg1Weighter = cfg.Analyzer(
  LeptonWeighter      ,
  'leg1Weighter'      ,
  effWeight   = None  ,
  effWeightMC = None  ,
  lepton      = 'leg1',
  verbose     = False ,
  disable     = True  ,
  )

leg2Weighter = cfg.Analyzer(
  LeptonWeighter      ,
  'leg2Weighter'      ,
  effWeight   = None  ,
  effWeightMC = None  ,
  lepton      = 'leg2',
  verbose     = False ,
  disable     = True  ,
  )

higgsWeighter = cfg.Analyzer(
  HiggsPtWeighter  ,
  'HiggsPtWeighter',
  )

treeProducer = cfg.Analyzer(
  H2TauTauTreeProducer  ,
  'H2TauTauTreeProducer',
  )

# RIC: off until fixed
# treeProducerXCheck = cfg.Analyzer(
#   H2TauTauSyncTree  ,
#   'H2TauTauSyncTree',
#   pt20 = False
#   )

###################################################
### CONNECT SAMPLES TO THEIR ALIASES AND FILES  ###
###################################################
from CMGTools.H2TauTau.proto.samples.phys14.diTau_Ric_Jan27 import *

###################################################
###             SET COMPONENTS BY HAND          ###
###################################################
selectedComponents = allsamples

###################################################
###                  SEQUENCE                   ###
###################################################
commonSequence = cfg.Sequence( [
  jsonAna            ,
  #triggerAna         ,
  vertexAna          ,
  diLeptonAna        ,
  genAna             ,
  dyJetsFakeAna      ,
  jetAna             ,
  vbfAna             ,
  pileUpAna          ,
  embedWeighter      ,
  NJetsAna           ,
  leg1Weighter       , 
  leg2Weighter       ,
  higgsWeighter      ,
  treeProducer       ,
# RIC: off until fixed
#   treeProducerXCheck ,
  ] )