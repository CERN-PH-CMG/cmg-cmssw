import CMGTools.RootTools.fwlite.Config as cfg
from CMGTools.RootTools.fwlite.Config import printComps
from CMGTools.RootTools.RootTools import *

#Load all analyzers
from CMGTools.TTHAnalysis.analyzers.susyCore_modules_cff import *

##------------------------------------------
## Redefine what I need
##------------------------------------------

#Lepton
ttHLepAna.loose_muon_dxy = 0.5
ttHLepAna.loose_muon_dz  = 1.0
ttHLepAna.loose_muon_relIso  = 0.15
ttHLepAna.loose_electron_id  = "POG_Cuts_ID_2012_Veto"
ttHLepAna.loose_electron_pt  = 5
ttHLepAna.loose_electron_eta    = 2.4
ttHLepAna.loose_electron_dxy    = 0.04
ttHLepAna.loose_electron_dz     = 0.2
ttHLepAna.loose_electron_relIso = 0.15
ttHLepAna.loose_electron_lostHits = 999 # no cut
ttHLepAna.ele_isoCorr = "deltaBeta"

# JET (for event variables do apply the jetID and not PUID yet)
ttHJetAna.relaxJetId = False
ttHJetAna.doPuId = False
ttHJetAna.jetEta = 5.2
ttHJetAna.jetEtaCentral = 2.5
ttHJetAna.jetPt = 10.
ttHJetAna.recalibrateJets = False
ttHJetAna.jetLepDR = 0.4
ttHJetMCAna.smearJets = False

# TAU 
ttHTauAna.etaMax = 2.3
ttHTauAna.dxyMax = 99999.
ttHTauAna.dzMax = 99999.
ttHTauAna.vetoLeptons = False
ttHTauAna.vetoLeptonsPOG = True

##------------------------------------------
##  ISOLATED TRACK
##------------------------------------------

# those are the cuts for the nonEMu
ttHIsoTrackAna = cfg.Analyzer(
            'ttHIsoTrackAnalyzer',
#            candidates='cmgCandidates',
#            candidatesTypes='std::vector<cmg::Candidate>',
            candidates='packedPFCandidates',
            candidatesTypes='std::vector<pat::PackedCandidate>',
            ptMin = 5, # for pion 
            ptMinEMU = 5, # for EMU
            dzMax = 0.1,
            #####
            isoDR = 0.3,
            ptPartMin = 0,
            dzPartMax = 0.1,
            maxAbsIso = 8,
            #####
            MaxIsoSum = 0.1, ### unused
            MaxIsoSumEMU = 0.2, ### unused
            doSecondVeto = False
            )


##------------------------------------------
##  TOLOLOGIAL VARIABLES: MT, MT2
##------------------------------------------

# Tree Producer
ttHTopoJetAna = cfg.Analyzer(
            'ttHTopoVarAnalyzer'
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


#-------- SAMPLES AND TRIGGERS -----------
from CMGTools.TTHAnalysis.samples.samples_13TeV_CSA14 import * 

selectedComponents = [ SingleMu, DoubleElectron, TTHToWW_PUS14, DYJetsM50_PU20bx25, TTJets_PUS14 ]

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
    comp=TTJets_PU20bx25
    comp.files = comp.files[:2]
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
        
