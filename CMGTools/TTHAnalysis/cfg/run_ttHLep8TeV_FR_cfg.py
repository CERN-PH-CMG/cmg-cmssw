import copy
import os 
import CMGTools.RootTools.fwlite.Config as cfg
from CMGTools.RootTools.fwlite.Config import printComps
from CMGTools.RootTools.RootTools import *

# this analyzer finds the initial events before the skim
skimAnalyzer = cfg.Analyzer(
    'skimAnalyzerCount'
    )


jsonAna = cfg.Analyzer(
    'JSONAnalyzer',
    )

triggerAna = cfg.Analyzer(
    #'TriggerAnalyzer',
    'triggerBitFilter',
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
    PDFWeights = [],
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
    'ttHJetAnalyzer',
    jetCol = 'cmgPFJetSelCHS',
    jetCol4MVA = 'cmgPFJetSel',
    jetPt = 25.,
    jetEta = 4.7,
    jetEtaCentral = 2.4,
    relaxJetId = False,  
    doPuId = True,
    recalibrateJets = True,
    shiftJEC = 0, # set to +1 or -1 to get +/-1 sigma shifts
    )

## MET Analyzer
#ttHMETAna = cfg.Analyzer(
#    'ttHMETAnalyzer',
#    )

# Jet MC Match Analyzer
ttHJetMCAna = cfg.Analyzer(
    'ttHJetMCMatchAnalyzer',
    smearJets = True,
    shiftJER = 0, # set to +1 or -1 to get +/-1 sigma shifts
    )


# Tree Producer
treeProducer = cfg.Analyzer(
    'ttHLepFRAnalyzer',
    jetCol4MVA = 'cmgPFJetSel',
    tagLeptonSel = { 'minSip3D' : 7, 'minRelIso' : 0.5 },
    tagJetSel = { 'minPt' : 40, 'minBTag' : 0.8980 },
    )


#-------- SAMPLES

from CMGTools.TTHAnalysis.samples.samples_8TeV import * 

for mc in mcSamples+[QCDMuPt15,QCDElPt30To80,QCDElPt80To170]:
    mc.triggers = triggersFR_MC
#selectedComponents=mcSamples
#selectedComponents=[TTH,DYJetsM10,DYJetsM50,TTLep]
#selectedComponents=[DY1JetsM50,DY2JetsM50,DY3JetsM50,DY4JetsM50,W2Jets,WGToLNuG,WJets_HT250To300,WJets_HT300To400,WJets_HT400ToInf]
#selectedComponents=[W1Jets,W2Jets,WGToLNuG]
mcSamples = [QCDMuPt15,QCDElPt30To80,QCDElPt80To170,WJets,DYJetsM50,DYJetsM10]

for data in dataSamples1Mu:
    data.triggers = triggersFR_1mu
    data.vetoTriggers = triggersFR_mumu
for data in dataSamplesE:
    data.triggers = triggersFR_1e
    data.vetoTriggers =  triggersFR_1mu + triggersFR_mumu
for data in dataSamplesMu:
    data.triggers = triggersFR_mumu
    data.vetoTriggers = [] 
for data in dataSamplesMuE:
    data.triggers = triggersFR_mue
    data.vetoTriggers = []
JetMonABCD.triggers = [ 'HLT_PFJet40_v*' ]

selectedComponents=mcSamples+dataSamples1Mu+dataSamplesMu+dataSamplesE+dataSamplesMuE


#-------- SEQUENCE

sequence = cfg.Sequence([
    skimAnalyzer,
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
#selectedComponents = dataSamples1Mu + dataSamplesE + [ JetMonABCD, QCDMuPt15, QCDElPt30To80,QCDElPt80To170  ]
selectedComponents = [ QCDMuPt15, QCDElPt30To80,QCDElPt80To170,SingleMuD  ]
test = 1
if test==1:
    # test a single component, using a single thread.
    # necessary to debug the code, until it doesn't crash anymore
    comp = selectedComponents[2]
    comp.files = comp.files[:40]
    selectedComponents = [comp]
    comp.splitFactor = 4
elif test==2:    
    # test all components (1 thread per component.
    # important to make sure that your code runs on any kind of component
    #selectedComponents = dataSamples1Mu
    for comp in selectedComponents:
        comp.splitFactor = 1
        comp.files = comp.files[:3]
elif test==3:
    # test two components, using many threads, to check if variables are ok
    comp = SingleMuD
    comp.files = comp.files[:20]
    comp.splitFactor = 3
    selectedComponents = [comp]
    comp = QCDMuPt15
    comp.files = comp.files[:20]
    comp.splitFactor = 3
    selectedComponents += [comp]
elif test==4:
    for c in selectedComponents:
        c.files = c.files[:100]
        c.splitFactor = 7


# creation of the processing configuration.
# we define here on which components to run, and
# what is the sequence of analyzers to run on each event. 
config = cfg.Config( components = selectedComponents,
                     sequence = sequence )

printComps(config.components, True)
