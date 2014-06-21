import CMGTools.RootTools.fwlite.Config as cfg
from CMGTools.RootTools.fwlite.Config import printComps
from CMGTools.RootTools.RootTools import *

PDFWeights = []
#PDFWeights = [ ("CT10",53), ("MSTW2008lo68cl",41), ("NNPDF21_100",101) ]

# Find the initial events before the skim
skimAnalyzer = cfg.Analyzer(
    'skimAnalyzerCount'
    )

# Pick individual events (normally not in the path)
eventSelector = cfg.Analyzer(
    'EventSelector',
    toSelect = []  # here put the event numbers (actual event numbers from CMSSW)
    )

# Apply json file (if the dataset has one)
jsonAna = cfg.Analyzer(
    'JSONAnalyzer',
    )

# Filter using the 'triggers' and 'vetoTriggers' specified in the dataset
triggerAna = cfg.Analyzer(
    #'TriggerAnalyzer',
    'triggerBitFilter',
    )


# Select a list of good primary vertices (generic)
ttHVertexAna = cfg.Analyzer(
    'VertexAnalyzer',
    vertexWeight = None,
    fixedWeight = 1,
    verbose = False
    )


# This analyzer actually does the pile-up reweighting (generic)
pileUpAna = cfg.Analyzer(
    "PileUpAnalyzer",
    true = True,  # use number of true interactions for reweighting
    makeHists=False
    )


# Gen Info Analyzer (generic, but should be revised)
ttHGenAna = cfg.Analyzer(
    'ttHGenLevelAnalyzer',
    filterHiggsDecays = [0, 15, 23, 24],
    verbose = False,
    PDFWeights = [ pdf for pdf,num in PDFWeights ]
    )

# Lepton Analyzer (generic)
susyScanAna = cfg.Analyzer(
    'susyParameterScanAnalyzer',
    )

# Lepton Analyzer (generic)
ttHLepAna = cfg.Analyzer(
    'ttHLepAnalyzerSusy',
    # input collections
    muons='cmgMuonSel',
    electrons='cmgElectronSel',
    rhoMuon= 'kt6PFJetsCentralNeutral',
    rhoElectron = 'kt6PFJets',
    photons='cmgPhotonSel',
    # energy scale corrections and ghost muon suppression (off by default)
    doMuScleFitCorrections=False, # "rereco"
    doRochesterCorrections=False,
    doElectronScaleCorrections=False, # "embedded" in 5.18 for regression
    doSegmentBasedMuonCleaning=False,
    # inclusive very loose muon selection
    inclusive_muon_id  = "POG_ID_Loose",
    inclusive_muon_pt  = 3,
    inclusive_muon_eta = 2.4,
    inclusive_muon_dxy = 0.5,
    inclusive_muon_dz  = 1.0,
    # loose muon selection
    loose_muon_id     = "POG_ID_Loose",
    loose_muon_pt     = 5,
    loose_muon_eta    = 2.4,
    loose_muon_dxy    = 0.05,
    loose_muon_dz     = 0.2,
    loose_muon_relIso = 0.4,
    # inclusive very loose electron selection
    inclusive_electron_id  = "",
    inclusive_electron_pt  = 5,
    inclusive_electron_eta = 2.5,
    inclusive_electron_dxy = 0.5,
    inclusive_electron_dz  = 1.0,
    inclusive_electron_lostHits = 1.0,
    # loose electron selection
    loose_electron_id     = "POG_MVA_ID_NonTrig",
    loose_electron_pt     = 7,
    loose_electron_eta    = 2.4,
    loose_electron_dxy    = 0.05,
    loose_electron_dz     = 0.2,
    loose_electron_relIso = 0.4,
    loose_electron_lostHits = 1.0,
    # minimum deltaR between a loose electron and a loose muon (on overlaps, discard the electron)
    min_dr_electron_muon = 0.02
    )

# Skim (generic, but requirements depend on the final state)
ttHLepSkim = cfg.Analyzer(
    'ttHLepSkimmer',
    minLeptons = 2,
    maxLeptons = 999,
    #idCut  = "lepton.relIso03 < 0.2" # can give a cut
    #ptCuts = [20,10],                # can give a set of pt cuts on the leptons
    )

# Lepton MC Matching (generic, must happen early to allow for MVA corrections)
ttHLepMCAna = cfg.Analyzer(
    'ttHLepMCMatchAnalyzer',
    matchAllInclusiveLeptons = False,
    )

# Tau Analyzer (generic)
ttHTauAna = cfg.Analyzer(
    'ttHTauAnalyzer',
    ptMin = 20,
    vetoLeptons = True,
    leptonVetoDR = 0.4,
    tauID = "byMediumIsolationMVA2",
    tauLooseID = "decayModeFinding",
)

# Tau MC Matching (generic)
ttHTauMCAna = cfg.Analyzer(
    'ttHTauMCMatchAnalyzer',
)


# Jets Analyzer (generic)
ttHJetAna = cfg.Analyzer(
    'ttHJetAnalyzer',
    jetCol = 'cmgPFJetSelCHS',
    jetCol4MVA = 'cmgPFJetSel',
    jetPt = 25.,
    jetEta = 4.7,
    jetEtaCentral = 2.4,
    jetLepDR = 0.4,
    minLepPt = 10,
    relaxJetId = False,  
    doPuId = True,
    recalibrateJets = False,
    shiftJEC = 0, # set to +1 or -1 to get +/-1 sigma shifts
    cleanJetsFromTaus = False,
    )

# Jet MC Match Analyzer (generic)
ttHJetMCAna = cfg.Analyzer(
    'ttHJetMCMatchAnalyzer',
    smearJets = True,
    shiftJER = 0, # set to +1 or -1 to get +/-1 sigma shifts
    )

# Event Analyzer (for susy multi-lepton)
ttHEventAna = cfg.Analyzer(
    'ttHLepEventAnalyzer',
    maxLeps = 4, ## leptons to use
    verbose = False,
    minJets25 = 0,
    )


from CMGTools.TTHAnalysis.samples.samples_8TeV_v517 import triggers_mumu, triggers_ee, triggers_mue, triggers_1mu
# Tree Producer
treeProducer = cfg.Analyzer(
    'treeProducerSusyMultilepton',
    vectorTree = True,
    PDFWeights = PDFWeights,
    triggerBits = {
            'SingleMu' : triggers_1mu,
            'DoubleMu' : triggers_mumu,
            'DoubleEl' : [ t for t in triggers_ee if "Ele15_Ele8_Ele5" not in t ],
            'TripleEl' : [ t for t in triggers_ee if "Ele15_Ele8_Ele5"     in t ],
            'MuEG'     : [ t for t in triggers_mue if "Mu" in t and "Ele" in t ]
        }
    )


#-------- SAMPLES AND TRIGGERS -----------
from CMGTools.TTHAnalysis.samples.samples_8TeV_v517 import * 

for mc in mcSamples+mcSamples+extraMcSamples+fastSimSamples:
    mc.triggers = triggersMC_mue
for data in dataSamplesMu:
    data.triggers = triggers_mumu
for data in dataSamplesE:
    data.triggers = triggers_ee
    data.vetoTriggers = triggers_mumu
for data in dataSamplesMuE:
    data.triggers = triggers_mue
    data.vetoTriggers=triggers_ee+triggers_mumu


selectedComponents = [ DY1JetsM50,DY2JetsM50,DY3JetsM50,DY4JetsM50,TTH122,TTH127,TTJetsSem1,TTJetsSem2 ] 

#-------- SEQUENCE

sequence = cfg.Sequence([
    skimAnalyzer,
   #eventSelector,
    jsonAna,
    triggerAna,
    pileUpAna,
    ttHGenAna,
    susyScanAna,
    ttHVertexAna,
    ttHLepAna,
    ttHLepSkim,
    ttHLepMCAna,
    ttHTauAna,
    ttHTauMCAna,
    ttHJetAna,
    ttHJetMCAna,
    ttHEventAna,
    treeProducer,
    ])


#-------- HOW TO RUN
test = 1
if test==1:
    # test a single component, using a single thread.
    comp = TTJets
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
