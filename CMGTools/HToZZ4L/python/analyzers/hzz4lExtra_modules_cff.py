import PhysicsTools.HeppyCore.framework.config as cfg

from PhysicsTools.Heppy.analyzers.core.AutoFillTreeProducer import AutoFillTreeProducer
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
