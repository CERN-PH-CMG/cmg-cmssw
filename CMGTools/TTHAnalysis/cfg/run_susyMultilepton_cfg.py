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

# Event Analyzer for susy multi-lepton (at the moment, it's the TTH one)
from CMGTools.TTHAnalysis.analyzers.ttHLepEventAnalyzer import ttHLepEventAnalyzer
ttHEventAna = cfg.Analyzer(
    ttHLepEventAnalyzer, name="ttHLepEventAnalyzer",
    minJets25 = 0,
    )

## Insert the SV analyzer in the sequence
susyCoreSequence.insert(susyCoreSequence.index(ttHCoreEventAna), 
                        ttHFatJetAna)
susyCoreSequence.insert(susyCoreSequence.index(ttHCoreEventAna), 
                        ttHSVAna)
susyCoreSequence.insert(susyCoreSequence.index(ttHCoreEventAna), 
                        ttHHeavyFlavourHadronAna)

from CMGTools.TTHAnalysis.samples.samples_13TeV_PHYS14 import triggers_mumu_iso, triggers_mumu_noniso, triggers_ee, triggers_3e, triggers_mue, triggers_1mu_iso, triggers_1e
triggerFlagsAna.triggerBits = {
    'DoubleMu' : triggers_mumu_iso,
    'DoubleMuNoIso' : triggers_mumu_noniso,
    'DoubleEl' : triggers_ee,
    'TripleEl' : triggers_3e,
    'MuEG'     : triggers_mue,
    'SingleMu' : triggers_1mu_iso,
    'SingleEl' : triggers_1e,
}

from CMGTools.TTHAnalysis.analyzers.treeProducerSusyMultilepton import * 
## Tree Producer
treeProducer = cfg.Analyzer(
     AutoFillTreeProducer, name='treeProducerSusyMultilepton',
     vectorTree = True,
     saveTLorentzVectors = False,  # can set to True to get also the TLorentzVectors, but trees will be bigger
     defaultFloatType = 'F', # use Float_t for floating point
     PDFWeights = PDFWeights,
     globalVariables = susyMultilepton_globalVariables,
     globalObjects = susyMultilepton_globalObjects,
     collections = susyMultilepton_collections,
)

#-------- SAMPLES AND TRIGGERS -----------

#-------- SEQUENCE
from CMGTools.TTHAnalysis.samples.samples_13TeV_PHYS14 import *
from CMGTools.TTHAnalysis.samples.samples_13TeV_CSA14v2 import SingleMu

selectedComponents = [
   ] + WJetsToLNuHT + DYJetsM50HT + [DYJetsToLL_M50,
    TTJets ]+ SingleTop +[
    TTWJets,TTZJets, TTH,
    WZJetsTo3LNu, ZZTo4L,
    GGHZZ4L,
    SMS_T1tttt_2J_mGl1500_mLSP100, SMS_T1tttt_2J_mGl1200_mLSP800
]

sequence = cfg.Sequence(susyCoreSequence+[
    ttHEventAna,
    treeProducer,
    ])

# -- fine splitting, for some private MC samples with a single file
#for comp in selectedComponents:
#    comp.splitFactor = 1
#    comp.fineSplitFactor = 40
    
test = 1
if test == 1:
    comp = TTH; comp.name = "TTH"
    #comp = SMS_T1tttt_2J_mGl1500_mLSP100
    comp.files = comp.files[:1]
    #comp.files = [ '/afs/cern.ch/work/g/gpetrucc/micro/CMSSW_7_2_0/src/step3.root' ]
    comp.splitFactor = 1
    comp.fineSplitFactor = 4
    #ttHLepSkim.minLeptons = 0
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
    comp = SMS_T1tttt_2J_mGl1200_mLSP800
    comp.files = [ 'root://eoscms//eos/cms/store/mc/Phys14DR/SMS-T1tttt_2J_mGl-1200_mLSP-800_Tune4C_13TeV-madgraph-tauola/MINIAODSIM/PU20bx25_tsg_PHYS14_25_V1-v1/00000/0CD15D7F-4E6B-E411-AEB4-002590DB9216.root' ]
    comp.splitFactor = 1
    #comp.fineSplitFactor = 10
    selectedComponents = [ comp ]




# the following is declared in case this cfg is used in input to the heppy.py script
from PhysicsTools.HeppyCore.framework.eventsfwlite import Events
config = cfg.Config( components = selectedComponents,
                     sequence = sequence,
                     services = [],  
                     events_class = Events)


