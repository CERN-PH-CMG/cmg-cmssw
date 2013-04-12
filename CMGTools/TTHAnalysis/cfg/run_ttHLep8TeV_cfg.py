import copy
import os 
import CMGTools.RootTools.fwlite.Config as cfg
from CMGTools.RootTools.fwlite.Config import printComps
from CMGTools.RootTools.RootTools import *

PDFWeights = []
#PDFWeights = [ ("cteq66",44), ("NNPDF21_100",101), ("MSTW2008lo68cl",41) ]
#PDFWeights = [ ("CT10",53), ("MSTW2008lo68cl",41), ("NNPDF21_100",101) ]

# this analyzer finds the initial events before the skim
skimAnalyzer = cfg.Analyzer(
    'skimAnalyzerCount'
    )

eventSelector = cfg.Analyzer(
    'EventSelector',
    toSelect = [
    # here put the event numbers (actual event numbers from CMSSW)
    ]
    )

jsonAna = cfg.Analyzer(
    'JSONAnalyzer',
    )

triggerAna = cfg.Analyzer(
    'TriggerAnalyzer',
    )


# this analyzer is just there to select a list of good primary vertices.
ttHVertexAna = cfg.Analyzer(
    'VertexAnalyzer',
    goodVertices = 'offlinePrimaryVertices',
    vertexWeight = None,
    fixedWeight = 1,
    verbose = False
    )


# this analyzer actually does the pile-up reweighting.
pileUpAna = cfg.Analyzer(
    "PileUpAnalyzer",
    # build unweighted pu distribution using number of pile up interactions if False
    # otherwise, use fill the distribution using number of true interactions
    true = True,
    makeHists=False
    )


# Gen Info Analyzer
ttHGenAna = cfg.Analyzer(
    'ttHGenLevelAnalyzer',
    filterHiggsDecays = [0, 15, 23, 24],
    verbose = False,
    PDFWeights = [ pdf for pdf,num in PDFWeights ]
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
    minGoodLeptons=2,
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
    jetCol = 'cmgPFJetSelCHS',
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

# Event Analyzer
ttHEventAna = cfg.Analyzer(
    'ttHLepEventAnalyzer',
    maxLeps = 4, ## leptons to use
    jetCol4MVA = 'cmgPFJetSel',
    verbose = False,
    )

# Tree Producer
treeProducer = cfg.Analyzer(
    'ttHLepTreeProducerBase',
    doLooseLeptons = False,
    PDFWeights = PDFWeights,
    )


#-------- SAMPLES

from CMGTools.TTHAnalysis.samples.samples_8TeV import * 

for mc in mcSamples+fastSimSamples:
    mc.triggers = triggersMC_mue
#selectedComponents=mcSamples
#selectedComponents=[TTH,DYJetsM10,DYJetsM50,TTLep]
#selectedComponents=[DY1JetsM50,DY2JetsM50,DY3JetsM50,DY4JetsM50,W2Jets,WGToLNuG,WJets_HT250To300,WJets_HT300To400,WJets_HT400ToInf]
#selectedComponents=[W1Jets,W2Jets,WGToLNuG]

for data in dataSamplesE:
    data.triggers = triggers_ee
for data in dataSamplesMu:
    data.triggers = triggers_mumu
    data.vetoTriggers = triggers_ee
for data in dataSamplesMuE:
    data.triggers = triggers_mue
    data.vetoTriggers=triggers_ee+triggers_mumu

selectedComponents=mcSamples+dataSamplesMu+dataSamplesE+dataSamplesMuE

isSingleMu = False
if isSingleMu:
    ttHLepAna.minGoodLeptons=1
    ttHLepAna.maxGoodLeptons=1
    ttHEventAna.minJets25 = 3
    mcSamples = [ TTJets, W2Jets, W3Jets, W4Jets, TtW, TbartW, Ttch,Tbartch,Tsch,Tbarsch, QCDMuPt15 ]
    for mc in mcSamples+fastSimSamples:
        mc.triggers = triggersMC_1mu
    for data in dataSamplesMu:
        data.triggers = triggers_1mu
    selectedComponents = dataSamples1Mu+mcSamples


#-------- SEQUENCE

sequence = cfg.Sequence([
    skimAnalyzer,
    #eventSelector,
    jsonAna,
    triggerAna,
    pileUpAna,
    ttHGenAna,
    ttHVertexAna,
    ttHLepAna,
    ttHLepMCAna,
    ttHJetAna,
    #ttHMETAna,
    ttHJetMCAna,
    ttHEventAna,
    treeProducer,
    
    ])


#-------- HOW TO RUN

# selectedComponents = [ FastSim_TTWJets, FastSim_TTWJets_MUp, FastSim_TTWJets_MDn ]
# set test = 0 to run all jobs, in case you are using pybatch.py
selectedComponents = mcSamples
test = 1
if test==1:
    # test a single component, using a single thread.
    # necessary to debug the code, until it doesn't crash anymore
    comp = TTH
    comp.files = comp.files[:4]
    selectedComponents = [comp]
    comp.splitFactor = 1
elif test==2:    
    # test all components (1 thread per component.
    # important to make sure that your code runs on any kind of component
    for comp in selectedComponents:
        comp.splitFactor = 1
        comp.files = comp.files[:2]
elif test==3:
    # test two components, using many threads, to check if variables are ok
    #comp = DoubleElectronC
    #comp.files = comp.files[:20]
    #comp.splitFactor = 5
    #selectedComponents = [comp]
    comp = DYJetsM50
    comp.files = comp.files[:20]
    comp.splitFactor = 5
    selectedComponents = [comp]

# creation of the processing configuration.
# we define here on which components to run, and
# what is the sequence of analyzers to run on each event. 
config = cfg.Config( components = selectedComponents,
                     sequence = sequence )

printComps(config.components, True)


























