import copy
import os 
import CMGTools.RootTools.fwlite.Config as cfg
from CMGTools.RootTools.fwlite.Config import printComps
from CMGTools.RootTools.RootTools import *


jsonAna = cfg.Analyzer(
    'JSONAnalyzer',
    )

triggerAna = cfg.Analyzer(
    'TriggerAnalyzer',
    usePrescaled = True,
    )


# this analyzer is just there to select a list of good primary vertices.
ttHVertexAna = cfg.Analyzer(
    'VertexAnalyzer',
    goodVertices = 'offlinePrimaryVertices',
    vertexWeight = None,
    fixedWeight = 1,
    verbose = False
    )

# Gen Info Analyzer
ttHGenAna = cfg.Analyzer(
    'ttHGenLevelAnalyzer',
    filterHiggsDecays = [0, 15, 23, 24],
    verbose = False,
    )



# Lepton Analyzer
ttHLepAna = cfg.Analyzer(
    'ttHLepAnalyzerBase',
    rhoMuon= 'kt6PFJetsCentralNeutral',
    rhoElectron = 'kt6PFJets',
    muons='cmgMuonSel',
    electrons='cmgElectronSel',
    photons='cmgPhotonSel',
    isolationCut=0.4, 
    minGoodLeptons=1,
    doSSLeptons=False,
    doRochesterCorrections=True,
    doElectronScaleCorrections=True,
    doRecomputeSIP3D=True,
    doSegmentBasedMuonCleaning=True,
    doEleMuCrossCleaning=True,
    )

# Lepton MC Matching (must happen earlier to allow for MVA corrections)
ttHLepMCAna = cfg.Analyzer(
    'ttHLepMCMatchAnalyzer',
    )


# Jets Analyzer 
ttHJetAna = cfg.Analyzer(
    'JetAnalyzer',
    jetCol = 'cmgPFJetSel',
    jetPt = 25.,
    jetEta = 4.7,
    cjvPtCut = 30.,
    btagSFseed = 123456,
    relaxJetId = False,  
    )

## MET Analyzer
#ttHMETAna = cfg.Analyzer(
#    'ttHMETAnalyzer',
#    )

# Jet MC Match Analyzer
ttHJetMCAna = cfg.Analyzer(
    'ttHJetMCMatchAnalyzer',
    )


# Tree Producer
treeProducer = cfg.Analyzer(
    'ttHLepFRAnalyzer',
    )


#-------- SAMPLES

from CMGTools.TTHAnalysis.samples.samples_8TeV import * 

for mc in mcSamples+[QCDMuPt15]:
    mc.triggers = triggersFR_MC
#selectedComponents=mcSamples
#selectedComponents=[TTH,DYJetsM10,DYJetsM50,TTLep]
#selectedComponents=[DY1JetsM50,DY2JetsM50,DY3JetsM50,DY4JetsM50,W2Jets,WGToLNuG,WJets_HT250To300,WJets_HT300To400,WJets_HT400ToInf]
#selectedComponents=[W1Jets,W2Jets,WGToLNuG]

for data in dataSamples1Mu:
    data.triggers = triggersFR_1mu
    data.vetoTriggers = ['HLT_Ele15_Ele8_Ele5_CaloIdL_TrkIdVL_v*' ]
for data in dataSamplesE:
    data.triggers = triggersFR_1e
    data.vetoTriggers = triggersFR_1mu
for data in dataSamplesMu:
    data.triggers = triggersFR_mumu
    data.vetoTriggers = triggersFR_1e + triggersFR_1mu
for data in dataSamplesMuE:
    data.triggers = triggersFR_mue
    data.vetoTriggers = triggersFR_1e + triggersFR_1mu + triggersFR_mumu

selectedComponents=mcSamples+dataSamplesMu+dataSamplesE+dataSamplesMuE


#-------- SEQUENCE

sequence = cfg.Sequence([
    jsonAna,
    triggerAna,
    ttHGenAna,
    ttHVertexAna,
    ttHLepAna,
    ttHLepMCAna,
    ttHJetAna,
    ttHJetMCAna,
    treeProducer,
    ])


#-------- HOW TO RUN

# set test = 0 to run all jobs, in case you are using pybatch.py
test = 2
if test==1:
    # test a single component, using a single thread.
    # necessary to debug the code, until it doesn't crash anymore
    comp = SingleMuD
    comp.files = comp.files[:24]
    selectedComponents = [comp]
    comp.splitFactor = 8
elif test==2:    
    # test all components (1 thread per component.
    # important to make sure that your code runs on any kind of component
    selectedComponents = dataSamples1Mu
    for comp in selectedComponents:
        comp.splitFactor = 1
        comp.files = comp.files[:1]
elif test==3:
    # test two components, using many threads, to check if variables are ok
    comp = SingleMuD
    comp.files = comp.files[:20]
    comp.splitFactor = 4
    selectedComponents = [comp]
    comp = QCDMuPt15
    comp.files = comp.files[:20]
    comp.splitFactor = 4
    selectedComponents += [comp]

# creation of the processing configuration.
# we define here on which components to run, and
# what is the sequence of analyzers to run on each event. 
config = cfg.Config( components = selectedComponents,
                     sequence = sequence )

printComps(config.components, True)


























