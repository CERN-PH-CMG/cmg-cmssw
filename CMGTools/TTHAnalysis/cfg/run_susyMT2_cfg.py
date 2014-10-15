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
ttHLepAna.inclusive_electron_lostHits = 999 # no cut
ttHLepAna.ele_isoCorr = "deltaBeta"
ttHLepAna.ele_tightId = "Cuts_2012"
ttHLepAna.notCleaningElectrons = True

# JET (for event variables do apply the jetID and not PUID yet)
ttHJetAna.relaxJetId = False
ttHJetAna.doPuId = False
ttHJetAna.jetEta = 5.2
ttHJetAna.jetEtaCentral = 2.5
ttHJetAna.jetPt = 10.
ttHJetAna.recalibrateJets = False
ttHJetAna.jetLepDR = 0.4
ttHJetMCAna.smearJets = False
ttHJetAna.jetGammaDR = 0.4
ttHJetAna.minGammaPt = 20
ttHJetAna.gammaEtaCentral = 2.4

# TAU 
ttHTauAna.etaMax = 2.3
ttHTauAna.dxyMax = 99999.
ttHTauAna.dzMax = 99999.
ttHTauAna.vetoLeptons = False
ttHTauAna.vetoLeptonsPOG = True

# Photon
ttHPhoAna.etaCentral = 2.4



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
##  CONTROL VARIABLES
##------------------------------------------ 

ttHMT2Control = cfg.Analyzer(
            'ttHMT2Control'
            )


##------------------------------------------
##  TOLOLOGIAL VARIABLES: MT, MT2
##------------------------------------------

ttHTopoJetAna = cfg.Analyzer(
            'ttHTopoVarAnalyzer',
            doOnlyDefault = True
            )


##------------------------------------------
##  PRODUCER
##------------------------------------------

####from CMGTools.TTHAnalysis.samples.samples_8TeV_v517 import triggers_mumu, triggers_ee, triggers_mue, triggers_1mu,
from CMGTools.TTHAnalysis.samples.samples_8TeV_v517 import triggers_HT650, triggers_MET150, triggers_HTMET, triggers_MT2_mumu, triggers_MT2_ee, triggers_MT2_mue, triggers_1mu, triggers_RA1_Photon

# Tree Producer
treeProducer = cfg.Analyzer(
        'treeProducerSusyFullHad',
        vectorTree = True,
        PDFWeights = PDFWeights,
##        triggerBits = { }
        triggerBits = {
            'HT650' : triggers_HT650,
            'MET150' : triggers_MET150,
            'ht350met100' : triggers_HTMET,
            'SingleMu' : triggers_1mu,
            'DoubleMu' : triggers_MT2_mumu,
            'DoubleEl' : triggers_MT2_ee,
            'MuEG'     : triggers_MT2_mue,
            'Photons'  : triggers_RA1_Photon,
            'htXprescale' : triggers_HTMET
            },
        )

###---- to switch off the comptrssion
#treeProducer.isCompressed = 0


#-------- SAMPLES AND TRIGGERS -----------
from CMGTools.TTHAnalysis.samples.samples_13TeV_CSA14 import * 

#selectedComponents = [ SingleMu, DoubleElectron, TTHToWW_PUS14, DYJetsM50_PU20bx25, TTJets_PUS14 ]

#selectedComponents = [ TTJets_MSDecaysCKM_central_PU_S14_POSTLS170 ]
#selectedComponents = [ WJetsToLNu_HT100to200_PU_S14_POSTLS170, WJetsToLNu_HT200to400_PU_S14_POSTLS170, WJetsToLNu_HT400to600_PU_S14_POSTLS170, WJetsToLNu_HT600toInf_PU_S14_POSTLS170 ]

#selectedComponents = [ QCD_Pt1000to1400_PU_S14_POSTLS170, QCD_Pt10to15_PU_S14_POSTLS170, QCD_Pt15to30_PU_S14_POSTLS170, QCD_Pt120to170_PU_S14_POSTLS170, QCD_Pt170to300_PU_S14_POSTLS170, QCD_Pt1400to1800_PU_S14_POSTLS170, QCD_Pt1800_PU_S14_POSTLS170, QCD_Pt300to470_PU_S14_POSTLS170, QCD_Pt30to50_PU_S14_POSTLS170, QCD_Pt470to600_PU_S14_POSTLS170, QCD_Pt50to80_PU_S14_POSTLS170, QCD_Pt5to10_PU_S14_POSTLS170, QCD_Pt600to800_PU_S14_POSTLS170, QCD_Pt800to1000_PU_S14_POSTLS170, QCD_Pt80to120_PU_S14_POSTLS170 ]

#selectedComponents = [ DYJetsM50_HT100to200_PU_S14_POSTLS170, DYJetsM50_HT200to400_PU_S14_POSTLS170, DYJetsM50_HT400to600_PU_S14_POSTLS170, DYJetsM50_HT600toInf_PU_S14_POSTLS170 ]

#selectedComponents = [ GJets_HT100to200_PU_S14_POSTLS170, GJets_HT200to400_PU_S14_POSTLS170, GJets_HT400to600_PU_S14_POSTLS170, ZJetsToNuNu_HT200to400_PU_S14_POSTLS170, ZJetsToNuNu_HT400to600_PU_S14_POSTLS170, ZJetsToNuNu_HT600toInf_PU_S14_POSTLS170 ]

#selectedComponents = [ SMS_T1bbbb_2J_mGl1000_mLSP900_PU_S14_POSTLS170, SMS_T1bbbb_2J_mGl1500_mLSP100_PU_S14_POSTLS170, SMS_T1qqqq_2J_mGl1400_mLSP100_PU_S14_POSTLS170, SMS_T1tttt_2J_mGl1200_mLSP800_PU_S14_POSTLS170, SMS_T1tttt_2J_mGl1500_mLSP100_PU_S14_POSTLS170 ]

#selectedComponents = [ DYJetsM50_HT100to200_PU_S14_POSTLS170, DYJetsM50_HT200to400_PU_S14_POSTLS170, DYJetsM50_HT400to600_PU_S14_POSTLS170, DYJetsM50_HT600toInf_PU_S14_POSTLS170, SMS_T1bbbb_2J_mGl1000_mLSP900_PU_S14_POSTLS170, SMS_T1bbbb_2J_mGl1500_mLSP100_PU_S14_POSTLS170, SMS_T1qqqq_2J_mGl1400_mLSP100_PU_S14_POSTLS170, SMS_T1tttt_2J_mGl1200_mLSP800_PU_S14_POSTLS170, SMS_T1tttt_2J_mGl1500_mLSP100_PU_S14_POSTLS170 ]

#selectedComponents = [ GJets_HT600toInf_PU_S14_POSTLS170 ]
#, ZJetsToNuNu_HT100to200_PU_S14_POSTLS170 ]

selectedComponents = [ TTJets_PU20bx25 ]

#selectedComponents = [ SMS_T1qqqq_2J_mGl1000_mLSP800_PU_S14_POSTLS170 ]

#-------- SEQUENCE

sequence = cfg.Sequence(susyCoreSequence+[
    ttHIsoTrackAna,
    ttHMT2Control,
    ttHTopoJetAna,
    treeProducer,
    ])

#-------- HOW TO RUN
test = 1
if test==1:
    # test a single component, using a single thread.
    comp=TTJets_PU20bx25 #TTJets_forSynch
    #comp=SMS_T1qqqq_2J_mGl1400_mLSP100_PU_S14_POSTLS170 # small files for testing
    #comp=SMS_T1bbbb_2J_mGl1000_mLSP900_PU_S14_POSTLS170
    #comp.files = ['/afs/cern.ch/work/p/pandolf/CMSSW_7_0_6_patch1_2/src/CMGTools/TTHAnalysis/cfg/pickevents.root']
    comp.files = ['/afs/cern.ch/user//m/mmasciov/public/TTJets_forSynch_1.root']
    comp.files = comp.files[:1]
    selectedComponents = [comp]
    comp.splitFactor = 10
elif test==2:
    selectedComponents = [ SingleMu, DoubleElectron, TTHToWW_PUS14, DYJetsM50_PU20bx25, TTJets_PUS14 ]
    # test all components (1 thread per component).
    for comp in selectedComponents:
        comp.splitFactor = 251
        comp.files = comp.files[:]
        #comp.files = comp.files[:1]

config = cfg.Config( components = selectedComponents,
                     sequence = sequence )

printComps(config.components, True)
        
