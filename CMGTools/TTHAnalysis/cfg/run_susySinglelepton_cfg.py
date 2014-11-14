##########################################################
##       CONFIGURATION FOR SUSY MULTILEPTON TREES       ##
## skim condition: >= 2 loose leptons, no pt cuts or id ##
##########################################################

import CMGTools.RootTools.fwlite.Config as cfg
from CMGTools.RootTools.fwlite.Config import printComps
from CMGTools.RootTools.RootTools import *

#Load all analyzers
from CMGTools.TTHAnalysis.analyzers.susyCore_modules_cff import * 

ttHLepAna.loose_muon_pt  = 10
ttHLepAna.loose_muon_relIso = 0.2
ttHLepAna.loose_electron_pt  = 10
ttHLepAna.loose_electron_relIso = 0.2
ttHLepAna.ele_isoCorr = "deltaBeta" 


# Redefine what I need

# --- LEPTON SKIMMING ---
ttHLepSkim.minLeptons = 1
ttHLepSkim.maxLeptons = 999
#ttHLepSkim.idCut  = ""
#ttHLepSkim.ptCuts = []

# --- JET-LEPTON CLEANING ---
ttHJetAna.minLepPt = 10 
ttHJetMCAna.smearJets     = False # do we need to smear the jets?

# Event Analyzer for susy multi-lepton (at the moment, it's the TTH one)
ttHEventAna = cfg.Analyzer(
    'ttHLepEventAnalyzer',
    minJets25 = 0,
    )


ttHIsoTrackAna = cfg.Analyzer(
            'ttHIsoTrackAnalyzer',
            candidates='packedPFCandidates',
            candidatesTypes='std::vector<pat::PackedCandidate>',
            ptMin = 5, # for pion 
            ptMinEMU = 5, # for EMU
            dzMax = 0.1,
            isoDR = 0.3,
            ptPartMin = 0,
            dzPartMax = 0.1,
            maxAbsIso = 8,
            MaxIsoSum = 0.1, ### unused
            MaxIsoSumEMU = 0.2, ### unused
            doSecondVeto = False
            )

from CMGTools.TTHAnalysis.samples.samples_8TeV_v517 import triggers_1mu, triggers_1muHT, triggers_1eleHT

# Tree Producer
treeProducer = cfg.Analyzer(
    'treeProducerSusySingleLepton',
    vectorTree = True,
    saveTLorentzVectors = False,  # can set to True to get also the TLorentzVectors, but trees will be bigger
    PDFWeights = PDFWeights,
    triggerBits = {
            'MuHT' : triggers_1muHT,
            'eleHT' : triggers_1eleHT
        }
    )


#-------- SAMPLES AND TRIGGERS -----------
from CMGTools.TTHAnalysis.samples.samples_13TeV_CSA14 import *
selectedComponents = [ SingleMu, DoubleElectron, TTHToWW_PUS14, DYJetsM50_PU20bx25, TTJets_PUS14 ]


#-------- SEQUENCE

sequence = cfg.Sequence(susyCoreSequence+[
    ttHIsoTrackAna,
    ttHEventAna,
    treeProducer,
    ])


#-------- HOW TO RUN
test = 1
if test==1:
    # test a single component, using a single thread.
    comp = TTJets_PU20bx25
    comp.files = comp.files[:1]
    selectedComponents = [comp]
    comp.splitFactor = 1
elif test==2:    
    # test all components (1 thread per component).
    for comp in selectedComponents:
        comp.splitFactor = 1
        comp.files = comp.files[:1]


config = cfg.Config( components = selectedComponents,
                     sequence = sequence )

printComps(config.components, True)
