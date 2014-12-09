##########################################################
##   CONFIGURATION FOR SOFT DILEPTON + MONOJET TREES    ##
## skim condition: >= 2 loose leptons, no pt cuts or id ##
##########################################################

import CMGTools.RootTools.fwlite.Config as cfg
from CMGTools.RootTools.fwlite.Config import printComps
from CMGTools.RootTools.RootTools import *

#Load all analyzers
from CMGTools.TTHAnalysis.analyzers.susyCore_modules_cff import * 

# Redefine what I need

# --- LEPTON DEFINITION ---

ttHLepAna.loose_muon_pt = 3
ttHLepAna.loose_muon_dxy = 0.02,
ttHLepAna.loose_muon_dz = 0.5,
ttHLepAna.loose_muon_relIso = 0.2
ttHLepAna.loose_muon_absIso = 5
ttHLepAna.loose_muon_ptIsoThreshold = 25
ttHLepAna.loose_electron_pt = 5
ttHLepAna.loose_electron_dxy = 0.02,
ttHLepAna.loose_electron_dz = 0.5,
ttHLepAna.loose_electron_relIso = 0.2
ttHLepAna.loose_electron_absIso = 5
ttHLepAna.loose_electron_ptIsoThreshold = 25

# --- LEPTON SKIMMING ---
ttHLepSkim.minLeptons = 0
ttHLepSkim.maxLeptons = 5
ttHLepSkim.ptCuts = [5,3]

# --- JET-LEPTON CLEANING ---
# ttHJetAna.minLepPt = 20 
ttHJetAna.jetPt = 30.0 

# --- JET-ENERGY-SCALE SYSTEMATICS ---
ttHJetAna.shiftJEC = 0, # set to +1 or -1 to get +/-1 sigma shifts

# --- JET-MET SKIMMING ---
#ttHJetMETSkim.jetPtCuts = [100,]
#ttHJetMETSkim.metCut    = 100

# Event Analyzer for susy multi-lepton (at the moment, it's the TTH one)
ttHEventAna = cfg.Analyzer(
    'ttHLepEventAnalyzer',
    minJets25 = 0,
    )

# MET Filter Analyzer for susy soft-lepton.
metFilterAna = cfg.Analyzer(
    'metFilterAnalyzer',
    )

# ST Filter Analyzer for susy soft-lepton.
stFilterAna = cfg.Analyzer(
    'stFilterAnalyzer',
    )

from CMGTools.TTHAnalysis.samples.samples_8TeV_v517 import *
# Tree Producer
treeProducer = cfg.Analyzer(
    'treeProducerSusySoftlepton',
    vectorTree = True,
    PDFWeights = PDFWeights,
    triggerBits = {
            'SingleMu' : triggers_1mu,
            'DoubleMu' : triggers_mumu,
            'DoubleEl' : [ t for t in triggers_ee if "Ele15_Ele8_Ele5" not in t ],
            'TripleEl' : [ t for t in triggers_ee if "Ele15_Ele8_Ele5"     in t ],
            'MuEG'     : [ t for t in triggers_mue if "Mu" in t and "Ele" in t ],
#             'MET15'    : triggers_MET150,
#             'HT650'    : triggers_HT650,
#             'triggers_HTMET' : triggers_HTMET,
            'SingleMuNonIso' : ["HLT_Mu40_v*"],
            'MetTrigger' : ["HLT_PFMET150_v*"],
            'MetTriggerParked' : ["HLT_MET80_Parked_v*"],
            'MET_120_HBHENoiseCleaned' : ["HLT_MET120_HBHENoiseCleaned*"],
            'Mono_CentralPFJet80_PFMETnoMu95_NHEF0p95': ["HLT_MonoCentralPFJet80_PFMETnoMu95_NHEF0p95*"],
            'Mono_CentralPFJet80_PFMETnoMu105_NHEF0p95':  ["HLT_MonoCentralPFJet80_PFMETnoMu105_NHEF0p95*"]
        }
    )


#-------- SAMPLES AND TRIGGERS -----------
from CMGTools.TTHAnalysis.samples.samples_8TeV_v517 import * 

# NO TRIGGERS FOR NOW
#for mc in mcSamplesAll:
#    mc.triggers = ["HLT_PFMET_150_v*" ]


#selectedComponents = [ TTJetsLep ]                                                                                 ->  newTuples000*
#selectedComponents = [ TbartW,TTH,TtW,TTWJets,TTZJets,WJets_HT250To300,WJets_HT300To400,WJets_HT400ToInf ]         ->  newTuples001*
#selectedComponents = [ DYJetsM10,DYJetsM50,DY1JetsM50,DY2JetsM50,DY3JetsM50,DY4JetsM50 ]                           ->  newTuples002
#selectedComponents = [ T2DegenerateStop_2J_1,T2DegenerateStop_2J_2,T2DegenerateStop_2J_3,T2DegenerateStop_2J_4 ]   ->  newTuples003*
#selectedComponents = [ WJetsPtW50To70,WJetsPtW70To100,WJetsPtW100,WJets,W1Jets,W2Jets,W3Jets,W4Jets ]              ->  newTuples004
#selectedComponents = [ WW,WZ,ZZ,WWJets,WZJets,ZZJets4L,ZZ2e2mu,ZZ2e2tau,ZZ2mu2tau,ZZTo4mu,ZZTo4tau,ZZTo4e ]        ->  newTuples005
#selectedComponents = [ TTLep,TTJets,TTJetsSem,TTJetsSem2,TTJetsHad ]                                               ->  newTuples006
#selectedComponents = [ ZNuNu50HT100,ZNuNu100HT200,ZNuNu200HT400,ZNuNu400 ]                                         ->  newTuples007 the QCDMuPt15 was originaly here but failed and submited as new set 008 with larger split
#selectedComponents = [ QCDMuPt15 ]                                                                                 ->  newTuples008

selectedComponents = [ TTJetsLep ]


#-------- SEQUENCE

#  ORIGINAL
sequence = cfg.Sequence( [ eventSelector ] + susyCoreSequence+[
    ttHEventAna,
    stFilterAna,
    metFilterAna,
    treeProducer,
    ])

# #   Comment-out for debug purposes. Provide a list of event #'s to process.
# eventSelector.toSelect = [ 1157146 ]
# 
# sequence = cfg.Sequence( [ eventSelector ] + susyCoreSequence+[
#     ttHEventAna,
#     stFilterAna,
#     metFilterAna,
#     treeProducer,
#     ])


#-------- HOW TO RUN
test = 1
if test==1:
    # test a single component, using a single thread.
    comp = selectedComponents[0]
    comp.files = comp.files[:1]         #   files to run interactively with "multiloop"
    selectedComponents = [comp]
    comp.splitFactor = 1
elif test==2:    
    # test all components (1 thread per component).
    for comp in selectedComponents:
        comp.splitFactor = 500
        comp.files = comp.files[:1]
elif test==3:    
    # run all components on lxbatch ( no need to set "comp.files = comp.files[A:B]" in this case ).
    for comp in selectedComponents:
        comp.splitFactor = 500


config = cfg.Config( components = selectedComponents,
                     sequence = sequence )

printComps(config.components, True)
