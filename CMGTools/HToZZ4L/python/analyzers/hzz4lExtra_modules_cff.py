import PhysicsTools.HeppyCore.framework.config as cfg

from PhysicsTools.Heppy.analyzers.core.AutoFillTreeProducer import AutoFillTreeProducer
from PhysicsTools.Heppy.analyzers.core.TriggerMatchAnalyzer import TriggerMatchAnalyzer
from CMGTools.HToZZ4L.analyzers.TwoLeptonAnalyzer import TwoLeptonAnalyzer
from CMGTools.HToZZ4L.analyzers.ThreeLeptonAnalyzer import ThreeLeptonAnalyzer
from CMGTools.HToZZ4L.analyzers.FourLeptonEventSkimmer import FourLeptonEventSkimmer
from CMGTools.HToZZ4L.analyzers.ZTagAndProbeAnalyzer import ZTagAndProbeAnalyzer
from CMGTools.HToZZ4L.analyzers.fourLeptonTree import *

twoLeptonAnalyzer = cfg.Analyzer(
    TwoLeptonAnalyzer, name="twoLeptonAnalyzer",
    mode = "Z"
)

twoLeptonEventSkimmer = cfg.Analyzer(
    FourLeptonEventSkimmer, name="twoLeptonEventSkimmer",
    required = ['bestIsoZ']
)

twoLeptonTreeProducer = cfg.Analyzer(
     AutoFillTreeProducer, name='twoLeptonTreeProducer',
     vectorTree = True,
     saveTLorentzVectors = False,  # can set to True to get also the TLorentzVectors, but trees will be bigger
     globalVariables = hzz_globalVariables, # rho, nvertices, njets
     globalObjects = hzz_globalObjects, # met
     collections = {
         "bestIsoZ"        : NTupleCollection("z",   ZType, 1, help="Dilepton Candidates"),    
         "selectedLeptons" : NTupleCollection("Lep", leptonTypeHZZ, 10, help="Leptons after the preselection"),
         "cleanJets"       : NTupleCollection("Jet", jetTypeExtra, 10, help="Cental jets after full selection and cleaning, sorted by pt"),
     },
     defaultFloatType = 'F',
)

twoLeptonAnalyzerOnia = cfg.Analyzer(
    TwoLeptonAnalyzer, name="twoLeptonAnalyzerOnia",
    mode = "Onia"
)

twoLeptonEventSkimmerOnia = cfg.Analyzer(
    FourLeptonEventSkimmer, name="twoLeptonEventSkimmerOnia",
    required = ['onia']
)


twoLeptonTreeProducerOnia = cfg.Analyzer(
     AutoFillTreeProducer, name='twoLeptonTreeProducerOnia',
     vectorTree = True,
     saveTLorentzVectors = False,  # can set to True to get also the TLorentzVectors, but trees will be bigger
     globalVariables = hzz_globalVariables, # rho, nvertices, njets
     globalObjects = hzz_globalObjects, # met
     collections = {
         "onia"        : NTupleCollection("Onia",   ZType, 20, help="Dilepton Candidates"),    
         "selectedLeptons" : NTupleCollection("Lep", leptonTypeHZZ, 10, help="Leptons after the preselection"),
     },
     defaultFloatType = 'F',
)

trigMatcher1Mu = cfg.Analyzer(
    TriggerMatchAnalyzer, name="trigMatcher1Mu",
    label='1Mu',
    processName = 'PAT',
    fallbackProcessName = 'RECO',
    unpackPathNames = True,
    trgObjSelectors = [ lambda t : t.path("HLT_IsoMu22_v*",1,0) or t.path("HLT_IsoMu20_v*",1,0) ],
    collToMatch = 'selectedLeptons',
    collMatchSelectors = [ lambda l,t : abs(l.pdgId()) == 13 ],
    collMatchDRCut = 0.3,
    univoqueMatching = True,
    verbose = False,
)
trigMatcher1El = trigMatcher1Mu.clone(
    name="trigMatcher1El",
    label='1El',
    trgObjSelectors = [ lambda t : t.path("HLT_Ele27_eta2p1_WP75_Gsf_v*",1,0) or t.path("HLT_Ele27_eta2p1_WPLoose_Gsf_v*",1,0) ],
    collMatchSelectors = [ lambda l,t : abs(l.pdgId()) == 11 ],
)

twoLeptonAnalyzerTnP = cfg.Analyzer(
    ZTagAndProbeAnalyzer, name="twoLeptonAnalyzerTnP",
    mode = "Z",
    tagSelection = lambda lep : lep.pt() > 25 and lep.tightId() and lep.relIso04 < 0.2 and (lep.matchedTrgObj1El if abs(lep.pdgId())==11 else lep.matchedTrgObj1Mu),
    probeCollection = "selectedLeptons", 
    probeSelection = lambda lep : True,
)

twoLeptonEventSkimmerTnP = cfg.Analyzer(
    FourLeptonEventSkimmer, name="twoLeptonEventSkimmerTnP",
    required = ['zTnP']
)


twoLeptonTreeProducerTnP = cfg.Analyzer(
     AutoFillTreeProducer, name='twoLeptonTreeProducerTnP',
     vectorTree = True,
     saveTLorentzVectors = False,  # can set to True to get also the TLorentzVectors, but trees will be bigger
     globalVariables = hzz_globalVariables, # rho, nvertices, njets
     globalObjects = hzz_globalObjects, # met
     collections = {
         "zTnP" : NTupleCollection("TnP",   ZType, 20, help="Dilepton Candidates"),    
     },
     defaultFloatType = 'F',
)



threeLeptonAnalyzer = cfg.Analyzer(
    ThreeLeptonAnalyzer, name="threeLeptonAnalyzer",
    srcZ = "bestIsoZ",
    lep3Sel = lambda lep : True,
)

threeLeptonEventSkimmer = cfg.Analyzer(
    FourLeptonEventSkimmer, name="threeLeptonEventSkimmer",
    required = ['zlepCands']
)

threeLeptonTreeProducer = cfg.Analyzer(
     AutoFillTreeProducer, name='threeLeptonTreeProducer',
     vectorTree = True,
     saveTLorentzVectors = False,  # can set to True to get also the TLorentzVectors, but trees will be bigger
     globalVariables = hzz_globalVariables, # rho, nvertices, njets
     globalObjects = hzz_globalObjects, # met
     collections = {
         "zlepCands"       : NTupleCollection("WZ",  WZType, 1, help="Dilepton Candidates"),    
         "cleanJets"       : NTupleCollection("Jet", jetTypeExtra, 10, help="Cental jets after full selection and cleaning, sorted by pt"),
     },
     defaultFloatType = 'F',
)
