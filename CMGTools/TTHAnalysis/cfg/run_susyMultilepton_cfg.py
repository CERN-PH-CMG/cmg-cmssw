##########################################################
##       CONFIGURATION FOR SUSY MULTILEPTON TREES       ##
## skim condition: >= 2 loose leptons, no pt cuts or id ##
##########################################################
import PhysicsTools.HeppyCore.framework.config as cfg

#Load all analyzers
from CMGTools.TTHAnalysis.analyzers.susyCore_modules_cff import * 

# Redefine what I need

# --- LEPTON SKIMMING ---
ttHLepSkim.minLeptons = 2
ttHLepSkim.maxLeptons = 999
#ttHLepSkim.idCut  = ""
#ttHLepSkim.ptCuts = []


## Event Analyzer for susy multi-lepton (at the moment, it's the TTH one)
#ttHEventAna = cfg.Analyzer(
#    'ttHLepEventAnalyzer',
#    minJets25 = 0,
#    )
#
## Insert the SV analyzer in the sequence
#susyCoreSequence.insert(susyCoreSequence.index(ttHCoreEventAna), 
#                        ttHFatJetAna)
#susyCoreSequence.insert(susyCoreSequence.index(ttHCoreEventAna), 
#                        ttHSVAnalyzer)
#susyCoreSequence.insert(susyCoreSequence.index(ttHCoreEventAna), 
#                        ttHHeavyFlavourHadronAnalyzer)
#
#
#from CMGTools.TTHAnalysis.samples.samples_8TeV_v517 import triggers_mumu, triggers_ee, triggers_mue, triggers_1mu

from CMGTools.TTHAnalysis.analyzers.treeProducerSusyMultilepton import * 
## Tree Producer
treeProducer = cfg.Analyzer(
     AutoFillTreeProducer, name='treeProducerSusyMultilepton',
     vectorTree = True,
     saveTLorentzVectors = False,  # can set to True to get also the TLorentzVectors, but trees will be bigger
     PDFWeights = PDFWeights,
     globalVariables = susyMultilepton_globalVariables,
     globalObjects = susyMultilepton_globalObjects,
     collections = susyMultilepton_collections,
)

#-------- SAMPLES AND TRIGGERS -----------

#-------- SEQUENCE
from CMGTools.TTHAnalysis.samples.samples_13TeV_PHYS14 import TTH_PU40bx25
TTHTest = cfg.MCComponent("TTHTest", files = TTH_PU40bx25.files[:1])

selectedComponents = [ TTHTest ]

sequence = cfg.Sequence(susyCoreSequence+[
    #ttHEventAna,
    treeProducer,
    ])

# the following is declared in case this cfg is used in input to the heppy.py script
from PhysicsTools.HeppyCore.framework.eventsfwlite import Events
config = cfg.Config( components = selectedComponents,
                     sequence = sequence,
                     services = [],  
                     events_class = Events)


