##########################################################
##       CONFIGURATION FOR SUSY MULTILEPTON TREES       ##
## skim condition: >= 2 loose leptons, no pt cuts or id ##
##########################################################

import CMGTools.RootTools.fwlite.Config as cfg
from CMGTools.RootTools.fwlite.Config import printComps
from CMGTools.RootTools.RootTools import *

#Load all analyzers
from CMGTools.TTHAnalysis.analyzers.susyCore_modules_cff import * 

# Redefine what I need

# --- LEPTON SKIMMING ---
ttHLepSkim.minLeptons = 1
ttHLepSkim.maxLeptons = 999
ttHLepSkim.idCut  = '(lepton.muonID("POG_ID_Loose") and lepton.relIso04 < 1.0) if abs(lepton.pdgId())==13 else (lepton.electronID("POG_MVA_ID_Trig"))'
ttHLepSkim.ptCuts = [20]


# Event Analyzer for w mass (at the moment, it's the TTH one)
ttHEventAna = cfg.Analyzer(
    'ttHLepEventAnalyzer',
    minJets25 = 0,
    )


from CMGTools.TTHAnalysis.samples.samples_8TeV_v517 import triggers_mumu, triggers_ee, triggers_mue, triggers_1mu, triggers_1e
# Tree Producer
treeProducer = cfg.Analyzer(
    'treeProducerWMassEle',
    vectorTree = True,
    saveTLorentzVectors = False,  # can set to True to get also the TLorentzVectors, but trees will be bigger
    PDFWeights = PDFWeights,
    triggerBits = {
            'SingleEl' : triggers_1e,
            'SingleMu' : triggers_1mu,
            'DoubleMu' : triggers_mumu,
            'DoubleEl' : [ t for t in triggers_ee if "Ele15_Ele8_Ele5" not in t ],
            'TripleEl' : [ t for t in triggers_ee if "Ele15_Ele8_Ele5"     in t ],
            'MuEG'     : [ t for t in triggers_mue if "Mu" in t and "Ele" in t ]
        }
    )


#-------- SAMPLES AND TRIGGERS -----------
from CMGTools.TTHAnalysis.samples.samples_8TeV_v517 import * 
mcSamplesW_1 = [WJets,DYJetsM50]
mcSamplesW_2 = [TTJets,TtW,TbartW,WZJets,WWJets, Tsch,Tbarsch,Ttch,Tbartch]
mcSamplesW_3 = [QCDMuPt15]
mcSamplesW = mcSamplesW_1 + mcSamplesW_2
dataSamplesW = dataSamples2L+dataSamples1Mu+dataSamples1E

for mc in mcSamplesW:
    mc.triggers = triggersMC_mue
for data in dataSamplesMu:
    data.triggers = triggers_mumu
for data in dataSamplesE:
    data.triggers = triggers_ee
for data in dataSamples1Mu:
    data.triggers = triggers_1mu
for data in dataSamples1E:
    data.triggers = triggers_1e


selectedComponents = mcSamplesW + dataSamplesW

#-------- MODULES CUSTOMISATION
ttHLepAna.doElectronScaleCorrections = True
ttHLepAna.loose_muon_relIso = 1.0
ttHLepAna.loose_electron_eta = 2.5
ttHLepAna.loose_electron_relIso = 999
ttHLepAna.loose_electron_id = "POG_MVA_ID_Trig"
ttHLepAna.triggerBitsMuons = { 'SingleMu' : triggers_1mu,
                               'DoubleMu' : triggers_mumu }
ttHLepAna.triggerBitsElectrons = { 'SingleEl' : triggers_1e,
                                   'DoubleEl' : [ t for t in triggers_ee if "Ele15_Ele8_Ele5" not in t ] }

#-------- SEQUENCE

susyCoreSequence.remove(susyScanAna)
susyCoreSequence.remove(ttHTauAna)
susyCoreSequence.remove(ttHTauMCAna)

sequence = cfg.Sequence(susyCoreSequence+[
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
