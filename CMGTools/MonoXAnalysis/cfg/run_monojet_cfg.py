##########################################################
##       CONFIGURATION FOR EXO MONOJET TREES            ##
## skim condition:   MET > 200 GeV                      ##
##########################################################
import PhysicsTools.HeppyCore.framework.config as cfg

#Load all analyzers
from CMGTools.MonoXAnalysis.analyzers.dmCore_modules_cff import * 
from PhysicsTools.Heppy.analyzers.objects.METAnalyzer import *

# Redefine what I need

# --- MONOJET SKIMMING ---
monoJetSkim.metCut = 200
monoJetSkim.jetPtCuts = []

# run miniIso
lepAna.doMiniIsolation = True
lepAna.packedCandidates = 'packedPFCandidates'
lepAna.miniIsolationPUCorr = 'rhoArea'
lepAna.miniIsolationVetoLeptons = None # use 'inclusive' to veto inclusive leptons and their footprint in all isolation cones
## will become miniIso perhaps?
#lepAna.loose_muon_isoCut     = lambda muon : muon.relIso03 < 10.5
#lepAna.loose_electron_isoCut = lambda electron : electron.relIso03 < 10.5
    

# switch off slow photon MC matching
photonAna.do_mc_match = False

##------------------------------------------
##  TOLOLOGIAL VARIABLES: MT, MT2
##------------------------------------------
from CMGTools.TTHAnalysis.analyzers.ttHTopoVarAnalyzer import ttHTopoVarAnalyzer
ttHTopoJetAna = cfg.Analyzer(
    ttHTopoVarAnalyzer, name = 'ttHTopoVarAnalyzer',
    doOnlyDefault = True
    )

from CMGTools.TTHAnalysis.analyzers.ttHMT2Control import ttHMT2Control
ttHMT2ControlAna = cfg.Analyzer(
    ttHMT2Control, name = 'ttHMT2Control',

    )
##------------------------------------------
##  TOLOLOGIAL VARIABLES: ALPHAT
##------------------------------------------
from CMGTools.TTHAnalysis.analyzers.ttHAlphaTVarAnalyzer import ttHAlphaTVarAnalyzer
ttHAlphaTAna = cfg.Analyzer(
    ttHAlphaTVarAnalyzer, name = 'ttHAlphaTVarAnalyzer',
    )

from CMGTools.TTHAnalysis.analyzers.ttHAlphaTControlAnalyzer import ttHAlphaTControlAnalyzer
ttHAlphaTControlAna = cfg.Analyzer(
    ttHAlphaTVarAnalyzer, name = 'ttHAlphaTControlAnalyzer',
    )
##-----------------------------------------------
##  TOLOLOGIAL VARIABLES: MONOJET SPECIFIC ONES
##-----------------------------------------------
from CMGTools.MonoXAnalysis.analyzers.monoJetVarAnalyzer import monoJetVarAnalyzer
monoJetVarAna = cfg.Analyzer(
    monoJetVarAnalyzer, name = 'monoJetVarAnalyzer',
    )

##------------------------------------------
# Event Analyzer for monojet 
##------------------------------------------
from CMGTools.MonoXAnalysis.analyzers.monoJetEventAnalyzer import monoJetEventAnalyzer
MonoJetEventAna = cfg.Analyzer(
    monoJetEventAnalyzer, name="monoJetEventAnalyzer",
    minJets25 = 0,
    )

from CMGTools.MonoXAnalysis.samples.samples_monojet import triggers_MonoJet,triggers_WZ
triggerFlagsAna.triggerBits = {
    'MonoJet' : triggers_MonoJet,
    'WZ' : triggers_WZ,
}

from CMGTools.MonoXAnalysis.analyzers.treeProducerDarkMatterMonoJet import * 
## Tree Producer
treeProducer = cfg.Analyzer(
     AutoFillTreeProducer, name='treeProducerDarkMatterMonoJet',
     vectorTree = True,
     saveTLorentzVectors = False,  # can set to True to get also the TLorentzVectors, but trees will be bigger
     defaultFloatType = 'F', # use Float_t for floating point
     PDFWeights = PDFWeights,
     globalVariables = dmMonoJet_globalVariables,
     globalObjects = dmMonoJet_globalObjects,
     collections = dmMonoJet_collections,
)

#-------- SAMPLES AND TRIGGERS -----------

#-------- SEQUENCE
from CMGTools.MonoXAnalysis.samples.samples_monojet import *

selectedComponents = []

sequence = cfg.Sequence(dmCoreSequence+[
    ttHMT2ControlAna,
    ttHTopoJetAna,
    ttHFatJetAna,
    ttHAlphaTAna,
    ttHAlphaTControlAna,
    monoJetVarAna,
    MonoJetEventAna,
    treeProducer,
    ])

# -- fine splitting, for some private MC samples with a single file
#for comp in selectedComponents:
#    comp.splitFactor = 1
#    comp.fineSplitFactor = 40
    
test = 10
if test == 1:
    comp = Monojet_M_10; comp.name = "Monojet_M_10"
    #comp = SMS_T1tttt_2J_mGl1500_mLSP100
    comp.files = comp.files[:1]
    comp.files = [ '/afs/cern.ch/work/a/avartak/public/dmVM10.root' ]
    comp.splitFactor = 1
    comp.fineSplitFactor = 1
    monoJetSkim.metCut = 0
    selectedComponents = [ comp ]
elif test == 2:
    for comp in selectedComponents:
        comp.files = comp.files[:1]
        comp.splitFactor = 1
        comp.fineSplitFactor = 1
elif test == 3:
    comp = TTJets
    comp.files = comp.files[:1]
    comp.splitFactor = 1
    selectedComponents = [ comp ]
elif test == 4:
    comp = SingleMu
    comp.files = comp.files[:1]
    comp.splitFactor = 1
    selectedComponents = [ comp ]
elif test == 10: # sync
    #eventSelector.toSelect = [ 11809 ]
    #sequence = cfg.Sequence([eventSelector] + susyCoreSequence+[ ttHEventAna, treeProducer, ])
    jetAna.recalibrateJets = False 
    jetAna.smearJets       = False 
    comp = Monojet_M_10; comp.name = "Monojet_M_10"
    comp.files = [ '/afs/cern.ch/work/a/avartak/public/dmVM10.root' ]
    comp.splitFactor = 1
    comp.fineSplitFactor = 1
    monoJetSkim.metCut = 0
    selectedComponents = [ comp ]




# the following is declared in case this cfg is used in input to the heppy.py script
from PhysicsTools.HeppyCore.framework.eventsfwlite import Events
config = cfg.Config( components = selectedComponents,
                     sequence = sequence,
                     services = [],  
                     events_class = Events)


