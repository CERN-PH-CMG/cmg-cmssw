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
ttHLepAna.inclusive_muon_pt  = 3
ttHLepAna.loose_muon_pt  = 3
ttHLepAna.loose_muon_relIso = 99.0
ttHLepAna.loose_muon_absIso = 10.0
ttHLepAna.inclusive_electron_pt  = 5
ttHLepAna.loose_electron_pt  = 5
ttHLepAna.loose_electron_relIso = 99.0
ttHLepAna.loose_electron_absIso = 10.0


# --- LEPTON SKIMMING ---
ttHLepSkim.minLeptons = 2
ttHLepSkim.maxLeptons = 999
ttHLepSkim.ptCuts = [5,3]

# --- JET-LEPTON CLEANING ---
ttHJetAna.minLepPt = 20 
# otherwise with only absIso cut at 10 GeV and no relIso we risk cleaning away good jets

# --- JET-MET SKIMMING ---
ttHJetMETSkim.jetPtCuts = [100,]
ttHJetMETSkim.metCut    = 100

# Event Analyzer for susy multi-lepton (at the moment, it's the TTH one)
ttHEventAna = cfg.Analyzer(
    'ttHLepEventAnalyzer',
    minJets25 = 0,
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
            'MET150'    : triggers_MET150,
            'HT650'    : triggers_HT650,
            'triggers_HTMET' : triggers_HTMET,
            'MetTriggerParked' : triggers_METParked 
        }
    )


#-------- SAMPLES AND TRIGGERS -----------
from CMGTools.TTHAnalysis.samples.samples_8TeV_v517 import * 

# NO TRIGGERS FOR NOW
#for mc in mcSamplesAll:
#    mc.triggers = ["HLT_PFMET_150_v*" ]


#selectedComponents = [ DY1JetsM50,DY2JetsM50,DY3JetsM50,DY4JetsM50,TTH122,TTH127,TTJetsSem1,TTJetsSem2 ] 
#selectedComponents = [ T2DegenerateStop_2J_4 ]
selectedComponents = [WJetsPtW100]

#-------- SEQUENCE

sequence = cfg.Sequence(susyCoreSequence+[
    ttHEventAna,
    treeProducer,
    ])


#-------- HOW TO RUN
test = 1
if test==1:
    # test a single component, using a single thread.
    comp = selectedComponents[0]
    comp.files =  comp.files[:1]
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
