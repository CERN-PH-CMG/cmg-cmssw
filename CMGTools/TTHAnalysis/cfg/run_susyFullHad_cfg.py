import CMGTools.RootTools.fwlite.Config as cfg
from CMGTools.RootTools.fwlite.Config import printComps
from CMGTools.RootTools.RootTools import *

#Load all analyzers
from CMGTools.TTHAnalysis.analyzers.susyCore_modules_cff import *

##------------------------------------------
## Redefine what I need
##------------------------------------------

# JET (do not apply the jetID and PUID yet)
ttHJetAna.relaxJetId = True
ttHJetAna.doPuId = False
ttHJetAna.jetEtaCentral = 2.5

##------------------------------------------
##  ISOLATED TRACK
##------------------------------------------

# those are the cuts for the nonEMu
ttHIsoTrackAna = cfg.Analyzer(
            'ttHIsoTrackAnalyzer',
            candidates='cmgCandidates',
            candidatesTypes='std::vector<cmg::PackedCandidate>',
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


##------------------------------------------
##  PRODUCER
##------------------------------------------

####from CMGTools.TTHAnalysis.samples.samples_8TeV_v517 import triggers_mumu, triggers_ee, triggers_mue, triggers_1mu,
from CMGTools.TTHAnalysis.samples.samples_8TeV_v517 import triggers_HT650, triggers_MET150, triggers_HTMET

# Tree Producer
treeProducer = cfg.Analyzer(
        'treeProducerSusyFullHad',
        vectorTree = True,
        PDFWeights = PDFWeights,
##        triggerBits = { }
        triggerBits = {
            'HT650' : triggers_HT650,
            'MET150' : triggers_MET150,
            'ht350met100' : triggers_HTMET
            }
        )

#-------- SAMPLES AND TRIGGERS (to skim) -----------
from CMGTools.TTHAnalysis.samples.samples_8TeV_v517 import *

#for mc in mcSamplesAll:
#    mc.triggers = triggers_HT650+triggers_MET150+triggers_HTMET
#for data in dataSamples:
#    data.triggers = triggers_HT

mcSamples_mainFullHad = [ TTJetsSem,TTJetsLep ]
selectedComponents = mcSamples_mainFullHad 

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
    comp = TTJetsSem
##    comp = TTJetslep
##    comp=WJetsPtW100
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
        
