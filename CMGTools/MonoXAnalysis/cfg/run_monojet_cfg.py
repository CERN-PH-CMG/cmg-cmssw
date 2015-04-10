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
##  TOLOLOGIAL VARIABLES: RAZOR
##------------------------------------------
from CMGTools.MonoXAnalysis.analyzers.monoXRazorAnalyzer import monoXRazorAnalyzer
monoXRazorAna = cfg.Analyzer(
    monoXRazorAnalyzer, name = 'monoXRazorAnalyzer',
    doOnlyDefault = False
    )

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
from CMGTools.MonoXAnalysis.samples.samples_monojet import *
from CMGTools.MonoXAnalysis.samples.samples_8TeV import *

selectedComponents = [ DoubleElectronAB, Monojet_M_10 ]

from CMGTools.MonoXAnalysis.samples.samples_monojet import triggers_MonoJet, triggers_WZ
from CMGTools.MonoXAnalysis.samples.samples_8TeV import triggers_ee
triggerFlagsAna.triggerBits = {
    'MonoJet' : triggers_MonoJet,
    'WZ' : triggers_WZ,
    'DoubleEl' : triggers_ee,
}



#-------- SEQUENCE
sequence = cfg.Sequence(dmCoreSequence+[
    monoXRazorAna,
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
    
test = 4
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
    # test data
    comp = DoubleElectronAB; comp.name = "dataSamplesAll"
    comp.triggers = []
    jetAna.recalibrateJets = False 
    jetAna.smearJets       = False 
    comp.files = [ '/afs/cern.ch/work/e/emanuele/monox/heppy/CMSSW_7_2_3_patch1/src/step5.root' ]
    comp.isMC = False
    comp.splitFactor = 1
    comp.fineSplitFactor = 1
    monoJetSkim.metCut = 0
    selectedComponents = [ comp ]
elif test == 10: # sync
    #eventSelector.toSelect = [ 11809 ]
    #sequence = cfg.Sequence([eventSelector] + susyCoreSequence+[ ttHEventAna, treeProducer, ])
    jetAna.recalibrateJets = False 
    jetAna.smearJets       = False 
    comp = Monojet_M_10; comp.name = "Monojet_M_10"
    comp.files = [ 'root://eoscms//eos/cms/store/mc/Phys14DR/DYJetsToLL_M-50_13TeV-madgraph-pythia8/MINIAODSIM/PU20bx25_PHYS14_25_V1-v1/00000/0432E62A-7A6C-E411-87BB-002590DB92A8.root' ]
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


