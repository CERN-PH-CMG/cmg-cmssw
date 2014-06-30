import CMGTools.RootTools.fwlite.Config as cfg
from CMGTools.RootTools.fwlite.Config import printComps
from CMGTools.RootTools.RootTools import *

#Load all analyzers
from CMGTools.TTHAnalysis.analyzers.susyCore_modules_cff import *


##------------------------------------------
##  MET, JETS, TOLOLOGIAL VARIABLES: MT, MT2
##------------------------------------------

## those are the cuts for the nonEMu
ttHIsoTrackAna = cfg.Analyzer(
            'ttHIsoTrackAnalyzer',
            candidates='cmgCandidates',
            candidatesTypes='std::vector<cmg::Candidate>',
            ptMin = 10,
            ptMinEMU = 5,
            dzMax = 0.1,
            #####
            isoDR = 0.3,
            ptPartMin = 0,
            dzPartMax = 0.1,
            #####
            MaxIsoSum = 0.1,
            MaxIsoSumEMU = 0.2,
            doSecondVeto = True
            )


##------------------------------------------
##  TOLOLOGIAL VARIABLES: MT, MT2
##------------------------------------------

# Tree Producer
ttHTopoJetAna = cfg.Analyzer(
            'ttHTopoVarAnalyzer',
            muons='cmgMuonSel',
            electrons='cmgElectronSel',
            taus='cmgTauSel',
            jetCol = 'cmgPFJetSelCHS'
            )

# Core Event Analyzer (computes basic quantities like HT, dilepton masses)
ttHCoreEventAna = cfg.Analyzer(
        'ttHCoreEventAnalyzer',
        maxLeps = 4, ## leptons to consider
        )

## Event Analyzer for susy multi-lepton (at the moment, it's the TTH one)
#ttHEventAna = cfg.Analyzer(
#        'ttHLepEventAnalyzer',
#            minJets25 = 0,
#  )


##------------------------------------------
##  PRODUCER
##------------------------------------------

####from CMGTools.TTHAnalysis.samples.samples_8TeV_v517 import triggers_mumu, triggers_ee, triggers_mue, triggers_1mu,
from CMGTools.TTHAnalysis.samples.samples_8TeV_v517 import triggers_HT, triggers_MET, triggers_HTMET

# Tree Producer
treeProducer = cfg.Analyzer(
        'treeProducerSusyFullHad',
        vectorTree = True,
        PDFWeights = PDFWeights,
        triggerBits = { }
        #    triggerBits = {
        #            'HT' : triggers_HT,
        #            'MET' : triggers_MET,
        #            'HTMET' : triggers_HTMET
        #        }
        )


#-------- SAMPLES AND TRIGGERS -----------
from CMGTools.TTHAnalysis.samples.samples_8TeV_v517 import *

#for mc in mcSamples+mcSamples+extraMcSamples+fastSimSamples:
#    mc.triggers = triggers_HT+triggers_MET+triggers_HTMET
#for data in dataSamples:
#    data.triggers = triggers_HT

selectedComponents = [ DY1JetsM50,DY2JetsM50,DY3JetsM50,DY4JetsM50,TTH122,TTH127,TTJetsSem1,TTJetsSem2 ]

#-------- SEQUENCE

sequence = cfg.Sequence(susyCoreSequence+[
    ttHIsoTrackAna,
    ttHTopoJetAna,
    treeProducer,
    ])

#-------- HOW TO RUN
test = 1
if test==1:
    # test a single component, using a single thread.
    ##    comp = TTJets
    comp=WJetsPtW100
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
        
