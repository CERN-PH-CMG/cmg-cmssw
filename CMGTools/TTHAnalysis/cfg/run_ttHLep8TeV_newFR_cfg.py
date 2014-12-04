import copy
import os 
import CMGTools.RootTools.fwlite.Config as cfg
from CMGTools.RootTools.fwlite.Config import printComps
from CMGTools.RootTools.RootTools import *

# this analyzer finds the initial events before the skim
skimAnalyzer = cfg.Analyzer(
    'skimAnalyzerCount'
    )

eventSelector = cfg.Analyzer(
    'EventSelector',
    toSelect = [ ]
    )


jsonAna = cfg.Analyzer(
    'JSONAnalyzer',
    )

triggerAna = cfg.Analyzer(
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


# this analyzer actually does the pile-up reweighting.
pileUpAna = cfg.Analyzer(
    "PileUpAnalyzer",
    true = True, # use  number of true interactions
    makeHists=False
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
    'ttHLepAnalyzerFR',
    rhoMuon= 'kt6PFJetsCentralNeutral',
    rhoElectron = 'kt6PFJets',
    muons='cmgMuonSel',
    electrons='cmgElectronSel',
    photons='cmgPhotonSel',
    isolationCut=9999.0, 
    sip3dCut=9999.0,
    sip3dCutVeryLoose=9999.0,
    minGoodLeptons=1, # note: these are loose
    doSSLeptons=False,
    doMuScleFitCorrections=False, #"rereco"
    doRochesterCorrections=False,
    doElectronScaleCorrections=False, #"embedded"
    doRecomputeSIP3D=False,
    doSegmentBasedMuonCleaning=False, #True
    doEleMuCrossCleaning=False, #True
    )



# Lepton MC Matching (must happen earlier to allow for MVA corrections)
ttHLepMCAna = cfg.Analyzer(
    'ttHLepMCMatchAnalyzer',
    smearJets=False
    )

# Jets Analyzer 
ttHJetAna = cfg.Analyzer(
    'ttHJetAnalyzer',
    #jetCol = 'cmgPFJetSelCHS',
    jetCol = 'cmgPFJetSel',
    jetCol4MVA = 'cmgPFJetSel',
    jetPt = 30.,
    jetEta = 4.7,
    jetEtaCentral = 2.5,
    relaxJetId = True,  
    doPuId = False,
    recalibrateJets = True,
    mcGT   = "START53_V20",  # Moriond
    dataGT = "GR_P_V42_AN4", # Moriond
    shiftJEC = 0, # set to +1 or -1 to get +/-1 sigma shifts
    cleanJetsFromTaus = False,
    )



## MET Analyzer
#ttHMETAna = cfg.Analyzer(
#    'ttHMETAnalyzer',
#    )

# Jet MC Match Analyzer
ttHJetMCAna = cfg.Analyzer(
    'ttHJetMCMatchAnalyzer',
    smearJets = False,
    shiftJER = 0, # set to +1 or -1 to get +/-1 sigma shifts
    )


# Tree Producer
treeProducer = cfg.Analyzer(
    'ttHLepFRAnalyzer',
    vectorTree = False,
    triggerBits = dict(
        [(x,["HLT_%s_v*"%x]) for x in  [ "Mu5", "RelIso1p0Mu5", "Mu8", "Mu12", "Mu17", "Mu24_eta2p1", "Mu40_eta2p1", "Mu40"] ] +
        [('Ele17T_Jet30', [ "HLT_Ele17_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL_Jet30_v*" ]),
         ('Ele17T',       [ "HLT_Ele17_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL_v*" ]),
         ('Ele8T',        [ "HLT_Ele8_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL_v*" ]) ]
    )
)



#-------- SAMPLES

from CMGTools.TTHAnalysis.samples.samples_8TeV import * 

for mc in mcSamples+[QCDMuPt15,QCDElPt30To80,QCDElPt80To170]:
    mc.triggers = triggersFR_MC
#selectedComponents=mcSamples
#selectedComponents=[TTH,DYJetsM10,DYJetsM50,TTLep]
#selectedComponents=[DY1JetsM50,DY2JetsM50,DY3JetsM50,DY4JetsM50,W2Jets,WGToLNuG,WJets_HT250To300,WJets_HT300To400,WJets_HT400ToInf]
#selectedComponents=[W1Jets,W2Jets,WGToLNuG]
mcSamples = [QCDMuPt15,QCDElPt30To80,QCDElPt80To170,WJets,DYJetsM50,DYJetsM10,W1Jets,W2Jets,W3Jets,DY1JetsM50,DY2JetsM50]

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
JetMonABCD.splitFactor = 500
selectedComponents=mcSamples+dataSamples1Mu+dataSamplesMu+dataSamplesE+dataSamplesMuE+[JetMonABCD]
SingleMuD.splitFactor = 1200
SingleMuC.splitFactor = 1200

QCDMuPt15.splitFactor = 1000
TTJets.splitFactor = 1000
WJets.splitFactor = 1000
DYJetsM50.splitFactor = 1000
DYJetsM10.splitFactor = 250

#selectedComponents = [TTJets, QCDMuPt15, WJets, DYJetsM50]
#selectedComponents = [ DYJetsM10 ]

for comp in [DoubleMuD]:
    comp.json = '/afs/cern.ch/user/g/gpetrucc/scratch0/cmgprod/CMSSW_5_3_14/src/CMGTools/TTHAnalysis/data/json/sync-Run2012D-Prompt.json'


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
    ttHJetMCAna,
    treeProducer,
    ])


#-------- HOW TO RUN

# set test = 0 to run all jobs, in case you are using pybatch.py
# selectedComponents.remove(DoubleMuD)
# selectedComponents.remove(DYJetsM50)
# selectedComponents.remove(WJets)
# #selectedComponents.remove(TTJets)

test = 10

if test==1:
    # test a single component, using a single thread.
    # necessary to debug the code, until it doesn't crash anymore
    comp = selectedComponents[0]
    comp.files = comp.files[:1]
    selectedComponents = [comp]
    comp.splitFactor = 1
elif test==2:    
    # test all components (1 thread per component.
    # important to make sure that your code runs on any kind of component
    #selectedComponents = dataSamples1Mu
    printComps(selectedComponents, True)
    for comp in selectedComponents:
        comp.splitFactor = 1
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
        c.files = c.files[10:610]
        c.splitFactor = 6
elif test==5:
    # MC sync sample
    comp = QCDMuPt15
    comp.files = ['root://eoscms//eos/cms/store/cmst3/user/gpetrucc/SusyFakes/cmgTuple_V5_10_qcd_mu_file1.full.root']
    selectedComponents = [comp]
    comp.splitFactor = 1
    comp.triggers = [] 
elif test==6:
    # MC sync sample
    comp = selectedComponents[0]
    comp.files = [
        'root://eoscms//eos/cms/store/cmst3/user/gpetrucc/SusyFakes/cmgTuple_file1.root',
        'root://eoscms//eos/cms/store/cmst3/user/gpetrucc/SusyFakes/cmgTuple_file2.root',
        'root://eoscms//eos/cms/store/cmst3/user/gpetrucc/SusyFakes/cmgTuple_file3.root',
        'root://eoscms//eos/cms/store/cmst3/user/gpetrucc/SusyFakes/cmgTuple_file4.root',
        'root://eoscms//eos/cms/store/cmst3/user/gpetrucc/SusyFakes/cmgTuple_file5.root',
    ]
    comp.files = comp.files[:2]
    comp.splitFactor = len(comp.files)
    selectedComponents = [comp]
    comp.triggers = [] 
elif test==7:
    # Data sync sample
    comp = DoubleMuD
    comp.files = [
        'root://eoscms//eos/cms/store/cmst3/user/gpetrucc/SusyFakes/cmgTuple_doubleMu_goodfileRunD.root',
    ]
    comp.splitFactor = len(comp.files)
    selectedComponents = [comp]
    comp.triggers = [] 
    comp.json = '/afs/cern.ch/user/g/gpetrucc/scratch0/cmgprod/CMSSW_5_3_14/src/CMGTools/TTHAnalysis/data/json/sync-Run2012D-Prompt.json'
elif test==8:
    # Data sync sample, processed with V5_10
    comp = DoubleMuD
    comp.files = [
        'root://eoscms//eos/cms/store/cmst3/user/gpetrucc/SusyFakes/cmgTuple_V5_10_doubleMu_goodfileRunD.root',
    ]
    comp.splitFactor = len(comp.files)
    selectedComponents = [comp]
    comp.triggers = [] 
    comp.json = '/afs/cern.ch/user/g/gpetrucc/scratch0/cmgprod/CMSSW_5_3_14/src/CMGTools/TTHAnalysis/data/json/sync-Run2012D-Prompt.json'
elif test==9:
    # MC sync sample for electrons
    comp = QCDMuPt15
    comp.files = ['root://eoscms//eos/cms/store/cmst3/user/gpetrucc/SusyFakes/cmgTuple_sync_electrons_MC.root']
    selectedComponents = [comp]
    comp.splitFactor = 1
    comp.triggers = [] 
elif test==10:
    # Data sync sample for electrons
    comp = DoubleElectronD
    comp.files = ['root://eoscms//eos/cms/store/cmst3/user/gpetrucc/SusyFakes/cmgTuple_sync_electrons_data.root']
    selectedComponents = [comp]
    comp.splitFactor = 1
    comp.triggers = [] 
    comp.json = '/afs/cern.ch/user/g/gpetrucc/scratch0/cmgprod/CMSSW_5_3_14/src/CMGTools/TTHAnalysis/data/json/sync-Run2012D-Prompt.json'





 
 


# creation of the processing configuration.
# we define here on which components to run, and
# what is the sequence of analyzers to run on each event. 
config = cfg.Config( components = selectedComponents,
                     sequence = sequence )

printComps(config.components, True)
