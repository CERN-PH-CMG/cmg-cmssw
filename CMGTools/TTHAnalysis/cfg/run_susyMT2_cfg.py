import PhysicsTools.HeppyCore.framework.config as cfg


#Load all analyzers
from CMGTools.TTHAnalysis.analyzers.susyCore_modules_cff import *

# Comment this line if you want the diagnostic folders produced along with the output root file
cfg.Analyzer.nosubdir = True


##------------------------------------------
## Redefine what I need
##------------------------------------------

### jet pt treshold for mt2 calculation
mt2JPt = 30.0

#JSON
jsonAna.useLumiBlocks = True

#Vertex
vertexAna.keepFailingEvents = True # keep events with no good vertices

#Lepton
lepAna.loose_muon_dxy = 0.5
lepAna.loose_muon_dz  = 1.0
lepAna.loose_muon_relIso  = 0.15
lepAna.loose_muon_isoCut = lambda muon :muon.miniRelIso < 0.2

lepAna.loose_electron_pt  = 5
lepAna.loose_electron_eta    = 2.4
lepAna.loose_electron_relIso = 0.15
lepAna.loose_electron_isoCut = lambda electron : electron.miniRelIso < 0.1

lepAna.loose_electron_id  = "POG_Cuts_ID_PHYS14_25ns_v1_ConvVetoDxyDz_Veto_full5x5"
lepAna.loose_electron_lostHits = 999. # no cut
lepAna.loose_electron_dxy    = 999.
lepAna.loose_electron_dz     = 999.

lepAna.inclusive_electron_id  = "POG_Cuts_ID_PHYS14_25ns_v1_ConvVetoDxyDz_Veto_full5x5"
lepAna.inclusive_electron_lostHits = 999. # no cut since embedded in ID
lepAna.inclusive_electron_dxy    = 999. # no cut since embedded in ID
lepAna.inclusive_electron_dz     = 999. # no cut since embedded in ID

lepAna.mu_isoCorr = "deltaBeta"
lepAna.ele_isoCorr = "deltaBeta"
lepAna.ele_tightId = "Cuts_PHYS14_25ns_v1_ConvVetoDxyDz"
lepAna.notCleaningElectrons = True
lepAna.doMiniIsolation = True
lepAna.miniIsolationPUCorr = 'rhoArea'
lepAna.doIsoAnnulus = True

# JET (for event variables do apply the jetID and not PUID yet)
jetAna.relaxJetId = False
jetAna.doPuId = False
jetAna.doQG = True
jetAna.jetEta = 4.7
jetAna.jetEtaCentral = 2.5
jetAna.jetPt = 10.
#jetAna.mcGT     = "Summer15_50nsV4_MC" # jec corrections
#jetAna.dataGT   = "Summer15_50nsV4_DATA" # jec corrections
jetAna.mcGT     = "Summer15_25nsV2_MC" # jec corrections
jetAna.dataGT   = "Summer15_25nsV2_DATA" # jec corrections
jetAna.recalibrateJets = False # True
jetAna.applyL2L3Residual = False # 'Data'
jetAna.jetLepDR = 0.4
jetAna.smearJets = False
jetAna.jetGammaDR = 0.4
jetAna.minGammaPt = 20.
jetAna.gammaEtaCentral = 2.4
jetAna.cleanJetsFromFirstPhoton = True
jetAna.cleanJetsFromIsoTracks = True ## added for Dominick

# TAU 
tauAna.inclusive_ptMin = 20.0
tauAna.inclusive_etaMax = 2.3
tauAna.inclusive_dxyMax = 99999.
tauAna.inclusive_dzMax = 99999.
tauAna.inclusive_vetoLeptons = False
tauAna.inclusive_vetoLeptonsPOG = True
tauAna.inclusive_decayModeID = "byLooseCombinedIsolationDeltaBetaCorr3Hits" # ignored if not set or ""
#tauAna.inclusive_tauAntiMuonID = "againstMuonTight"
tauAna.inclusive_tauAntiMuonID = "againstMuonTight3"
#tauAna.inclusive_tauAntiElectronID = "againstElectronLoose"
tauAna.inclusive_tauAntiElectronID = "againstElectronLooseMVA5"

tauAna.loose_ptMin = 20.0
tauAna.loose_etaMax = 2.3
tauAna.loose_dxyMax = 99999.
tauAna.loose_dzMax = 99999.
tauAna.loose_vetoLeptons = False
tauAna.loose_vetoLeptonsPOG = True
tauAna.loose_decayModeID = "byLooseCombinedIsolationDeltaBetaCorr3Hits" # ignored if not set or ""
#tauAna.loose_tauAntiMuonID = "againstMuonTight"
tauAna.loose_tauAntiMuonID = "againstMuonTight3"
#tauAna.loose_tauAntiElectronID = "againstElectronLoose"
tauAna.loose_tauAntiElectronID = "againstElectronLooseMVA5"

# Photon
photonAna.etaCentral = 2.5
photonAna.ptMin = 20
photonAna.gammaID = "POG_PHYS14_25ns_Loose_old_looseSieie_NoIso"
photonAna.do_randomCone = True
photonAna.do_mc_match = True

# Isolated Track
isoTrackAna.setOff=False
isoTrackAna.doIsoAnnulus = True

# recalibrate MET
metAna.recalibrate = False

# store all taus by default
genAna.allGenTaus = True

# Core Analyzer
ttHCoreEventAna.mhtForBiasedDPhi = "mhtJetXjvec"
ttHCoreEventAna.jetPt = mt2JPt ### jet pt 30: this will change ht and mht

# switch off the SV and MC matching
#ttHSVAna.do_mc_match = False

##------------------------------------------ 
##  CONTROL VARIABLES
##------------------------------------------ 

from CMGTools.TTHAnalysis.analyzers.ttHMT2Control import ttHMT2Control

ttHMT2Control = cfg.Analyzer(
            ttHMT2Control, name = 'ttHMT2Control',
            jetPt = mt2JPt, ### this will change control variables (gamma_ and zll_)
            )

##------------------------------------------
##  TOPOLOGICAL VARIABLES: minMT, MT2
##------------------------------------------

from CMGTools.TTHAnalysis.analyzers.ttHTopoVarAnalyzer import ttHTopoVarAnalyzer

ttHTopoJetAna = cfg.Analyzer(
            ttHTopoVarAnalyzer, name = 'ttHTopoVarAnalyzer',
            doOnlyDefault = True,
            jetPt = mt2JPt, ### this will change diffMetMht and deltaPhiMin
            )

from PhysicsTools.Heppy.analyzers.eventtopology.MT2Analyzer import MT2Analyzer

MT2Ana = cfg.Analyzer(
    MT2Analyzer, name = 'MT2Analyzer',
    metCollection     = "slimmedMETs",
    doOnlyDefault = True,
    #    jetPt = 40.,
    jetPt = mt2JPt, ### jet pt 30: this will change MT2 and pseudo-jets
    collectionPostFix = "",
    )

MT2AnaNoHF = cfg.Analyzer(
    MT2Analyzer, name = 'MT2Analyzer',
    metCollection     = "slimmedMETsNoHF",
    doOnlyDefault = True,
    jetPt = mt2JPt, ### this will change MT2 and pseudo-jets
    collectionPostFix = "NoHF",
    )

##------------------------------------------
##  Z skim
##------------------------------------------

from CMGTools.TTHAnalysis.analyzers.ttHmllSkimmer import ttHmllSkimmer
# Tree Producer                                                                                                                                                                         
ttHZskim = cfg.Analyzer(
            ttHmllSkimmer, name='ttHmllSkimmer',
            lepId=[13],
            maxLeps=3,
            massMin=60,
            massMax=120,
            doZGen = False,
            doZReco = True
            )

from CMGTools.TTHAnalysis.analyzers.hbheAnalyzer import hbheAnalyzer
hbheFilterAna = cfg.Analyzer(
    hbheAnalyzer, name = 'hbheAnalyzer',
    IgnoreTS4TS5ifJetInLowBVRegion=False,
)


##------------------------------------------
##  PRODUCER
##------------------------------------------

from CMGTools.RootTools.samples.triggers_13TeV_Spring15 import triggers_HT900, triggers_HT800, triggers_AllMET170, triggers_HTMET100, triggers_HTMET120, triggers_MT2_mumu, triggers_MT2_ee, triggers_MT2_e, triggers_MT2_mu, triggers_MT2_emu, triggers_MT2_mue, triggers_dijet, triggers_dijet70met120, triggers_dijet55met110, triggers_ht350, triggers_ht475,  triggers_ht600, triggers_photon75, triggers_photon90, triggers_photon120, triggers_photon75ps, triggers_photon90ps, triggers_photon120ps, triggers_photon155, triggers_photon165_HE10, triggers_photon175

triggerFlagsAna.triggerBits = {
'PFHT900' : triggers_HT900,
'PFHT800' : triggers_HT800,
'PFMET170' : triggers_AllMET170,
'PFHT350_PFMET100' : triggers_HTMET100,
'PFHT350_PFMET120' : triggers_HTMET120,
'SingleMu' : triggers_MT2_mu,
'SingleEl' : triggers_MT2_e,
'DoubleMu' : triggers_MT2_mumu,
'DoubleEl' : triggers_MT2_ee,
'MuX_Ele12' : triggers_MT2_emu,
'Mu8_EleX'  : triggers_MT2_mue,
#'MuEG' : triggers_MT2_mue,
'DiCentralPFJet70_PFMET120' : triggers_dijet70met120,
'DiCentralPFJet55_PFMET110' : triggers_dijet55met110,
##
'PFHT350_Prescale' : triggers_ht350,
'PFHT475_Prescale' : triggers_ht475,
'PFHT600_Prescale'  : triggers_ht600,
'DiJet' : triggers_dijet,
'ht350prescale' : triggers_ht350,
'ht475prescale' : triggers_ht475,
'Photon75_R9Id90_HE10_IsoM' : triggers_photon75,
'Photon90_R9Id90_HE10_IsoM' : triggers_photon90,
'Photon120_R9Id90_HE10_IsoM' : triggers_photon120,
'Photon75' : triggers_photon75ps,
'Photon90' : triggers_photon90ps,
'Photon120' : triggers_photon120ps,
'Photon155' : triggers_photon155,
'Photon165_HE10' : triggers_photon165_HE10,
'Photon175' : triggers_photon175,
}

### Temporary replacement for hbheFilter
eventFlagsAna.triggerBits = {
    #        "HBHENoiseFilter" : [ "Flag_HBHENoiseFilter" ], ### hbheFilter temporary replaced
    "CSCTightHaloFilter" : [ "Flag_CSCTightHaloFilter" ],
    "hcalLaserEventFilter" : [ "Flag_hcalLaserEventFilter" ],
    "EcalDeadCellTriggerPrimitiveFilter" : [ "Flag_EcalDeadCellTriggerPrimitiveFilter" ],
    "goodVertices" : [ "Flag_goodVertices" ],
    "trackingFailureFilter" : [ "Flag_trackingFailureFilter" ],
    "eeBadScFilter" : [ "Flag_eeBadScFilter" ],
    "ecalLaserCorrFilter" : [ "Flag_ecalLaserCorrFilter" ],
    "trkPOGFilters" : [ "Flag_trkPOGFilters" ],
    "trkPOG_manystripclus53X" : [ "Flag_trkPOG_manystripclus53X" ],
    "trkPOG_toomanystripclus53X" : [ "Flag_trkPOG_toomanystripclus53X" ],
    "trkPOG_logErrorTooManyClusters" : [ "Flag_trkPOG_logErrorTooManyClusters" ],
    "METFilters" : [ "Flag_METFilters" ],
}


#-------- SEQUENCE

from CMGTools.TTHAnalysis.analyzers.treeProducerSusyFullHad import *

treeProducer = cfg.Analyzer(
     AutoFillTreeProducer, name='treeProducerSusyFullHad',
##     AutoFillTreeProducer, name='treeProducerSusyCore',
     vectorTree = True,
     saveTLorentzVectors = False,  # can set to True to get also the TLorentzVectors, but trees will be bigger
     PDFWeights = PDFWeights,
     globalVariables = susyFullHad_globalVariables,
     globalObjects = susyFullHad_globalObjects,
     collections = susyFullHad_collections,
     defaultFloatType = 'F',
     treename = 'mt2'
)

susyCoreSequence.insert(susyCoreSequence.index(skimAnalyzer),
                        susyCounter)

#susyCoreSequence.insert(susyCoreSequence.index(ttHLepSkim),
#                        ttHZskim)

#susyCoreSequence.insert(susyCoreSequence.index(ttHCoreEventAna),
#                        ttHSVAna)


sequence = cfg.Sequence(
    susyCoreSequence+[
    ttHMT2Control,
    MT2Ana,
    ttHTopoJetAna,
    ttHFatJetAna,
    hbheFilterAna,
    treeProducer,
    ])

## NoHF add on
#sequence.insert(sequence.index(metAna),
#                metNoHFAna)
#sequence.insert(sequence.index(MT2Ana),
#                MT2AnaNoHF)


###---- to switch off the compression
#treeProducer.isCompressed = 0





from PhysicsTools.HeppyCore.framework.heppy_loop import getHeppyOption

#-------- HOW TO RUN
# choose 0 for quick validations tests. It doesn't require to load the sample files
# choose 2 for full mc production
# choose 3 for data production
# choose 4 for signal production
test = 3
isData = False # will be changed accordingly if chosen to run on data
doSpecialSettingsForMECCA = 1 # set to 1 for comparisons with americans

if test==0:
    # ------------------------------------------------------------------------------------------- #
    # --- all this lines taken from CMGTools.RootTools.samples.samples_13TeV_PHYS14
    # --- They may not be in synch anymore 
    from CMGTools.RootTools.samples.ComponentCreator import ComponentCreator
    kreator = ComponentCreator()
    testComponent = kreator.makeMCComponent("testComponent", "/GJets_HT-200to400_Tune4C_13TeV-madgraph-tauola/Phys14DR-PU20bx25_PHYS14_25_V1-v1/MINIAODSIM", "CMS", ".*root",489.9)
    samples=[testComponent]

    dataDir = os.environ['CMSSW_BASE']+"/src/CMGTools/TTHAnalysis/data"
    #json=dataDir+'/json/Cert_Run2012ABCD_22Jan2013ReReco.json'
    #json=dataDir+'/json/json_DCSONLY_Run2015B.txt'
    json=dataDir+'/json/json_DCSONLY.txt'
    from CMGTools.TTHAnalysis.setup.Efficiencies import *

    for comp in samples:
#        comp.isMC = True
#        comp.isData = False
        comp.splitFactor = 250 
        comp.puFileMC=dataDir+"/puProfile_Summer12_53X.root"
        comp.puFileData=dataDir+"/puProfile_Data12.root"
        comp.efficiency = eff2012
        comp.json = json
    # ------------------------------------------------------------------------------------------- #

    #eventSelector.toSelect = [ 442430994 ]
    #sequence = cfg.Sequence([eventSelector] + sequence)
    comp=testComponent
    # 74X TTbar
    comp.files = ['/afs/cern.ch/user/d/dalfonso/public/SYNCHfiles/0066F143-F8FD-E411-9A0B-D4AE526A0D2E.root']

    # 74X GJets
    #comp.files = ['root://xrootd.unl.edu//store/mc/RunIISpring15DR74/GJets_HT-400To600_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/MINIAODSIM/Asympt25ns_MCRUN2_74_V9-v1/00000/16E31BE7-7C18-E511-A551-00266CF2454C.root']

    # 747 Data
    #comp.files = ['/afs/cern.ch/user/m/mangano/public/MECCA/dataset/74X/data/JetHT_promptReco_Run2015B.root']
    #comp.files = ['/afs/cern.ch/work/m/mmasciov/CMSSW_7_4_7_MT2/src/CMGTools/TTHAnalysis/cfg/pickevents.root']

    # 7_4_12 data
    #isData = True
    #comp.files = ['/afs/cern.ch/user/c/casal/public/synch/86ACFECD-3C5F-E511-B8F2-02163E014374.root']

    selectedComponents = [comp]
#    comp.splitFactor = 10
#    comp.fineSplitFactor = 100

elif test==1:
    # Uncomment the two following lines to run on a specific event
    #eventSelector.toSelect = [ 84142401 ]
    #sequence = cfg.Sequence([eventSelector] + sequence)
    
#    from CMGTools.RootTools.samples.samples_13TeV_PHYS14 import *
    from CMGTools.RootTools.samples.samples_13TeV_74X import *
#    from CMGTools.RootTools.samples.samples_8TeVReReco_74X import *

#    comp=GJets_HT200to400
#    comp.files = ['/afs/cern.ch/user/d/dalfonso/public/TESTfilesPHY14/gjets_ht200to400_miniaodsim_fix.root']

#    comp=TTJets
#    #comp.files = ['/afs/cern.ch/user/d/dalfonso/public/TESTfilesPHY14/TTJets_miniAOD_fixPhoton_forSynch.root']
#    comp.files = ['/afs/cern.ch/user/d/dalfonso/public/TESTspring/ttbar25nsmad_1ECE44F9-5F02-E511-9A65-02163E00EA1F.root']
#    #comp.files = ['/afs/cern.ch/user/d/dalfonso/public/74samples/JetHT_GR_R_74_V12_19May_RelVal/1294BDDB-B7FE-E411-8028-002590596490.root']
#    comp.files = ['/afs/cern.ch/user/m/mangano/public/MECCA/dataset/74X/data/JetHT_promptReco_Run2015B.root']

    #synche file MC
    comp=comp=TTJets_LO_50ns
    comp.files = ['/afs/cern.ch/user/d/dalfonso/public/SYNCHfiles/0066F143-F8FD-E411-9A0B-D4AE526A0D2E.root']
   
    selectedComponents = [comp]
    comp.splitFactor = 1
#    comp.triggers = triggers_HT900 + triggers_HTMET + triggers_photon155 + triggers_1mu_isolow + triggers_MT2_mumu + triggers_MT2_ee + triggers_MT2_mue # to apply trigger skimming

elif test==2:

    #from CMGTools.TTHAnalysis.samples.samples_13TeV_PHYS14 import *
    # full production
#    selectedComponents = [ 
#TTJets, # TTJets
#TToLeptons_tch, TToLeptons_sch, TBarToLeptons_tch, TBarToLeptons_sch, TBar_tWch, T_tWch, #singleTop
#TTWJets, TTZJets, TTH, #TT+boson
#ZJetsToNuNu_HT100to200, ZJetsToNuNu_HT200to400, ZJetsToNuNu_HT400to600, ZJetsToNuNu_HT600toInf, # ZJetsToNuNu_HT
#WJetsToLNu_HT100to200, WJetsToLNu_HT200to400, WJetsToLNu_HT400to600, WJetsToLNu_HT600toInf, # WJetsToLNu_HT
#GJets_HT100to200_fixPhoton, GJets_HT200to400_fixPhoton, GJets_HT400to600_fixPhoton, GJets_HT600toInf_fixPhoton, # GJets_HT
#QCD_HT_100To250_fixPhoton, QCD_HT_250To500_fixPhoton, QCD_HT_500To1000_fixPhoton, QCD_HT_1000ToInf_fixPhoton, QCD_HT_250To500_ext1_fixPhoton, QCD_HT_500To1000_ext1_fixPhoton,QCD_HT_1000ToInf_ext1_fixPhoton, # QCD_HT
#QCD_Pt170to300_fixPhoton, QCD_Pt300to470_fixPhoton, QCD_Pt470to600_fixPhoton, QCD_Pt600to800_fixPhoton, QCD_Pt800to1000_fixPhoton, QCD_Pt1000to1400_fixPhoton, QCD_Pt1400to1800_fixPhoton, QCD_Pt1800to2400_fixPhoton, QCD_Pt2400to3200_fixPhoton, QCD_Pt3200_fixPhoton, # QCD_Pt
#QCD_Pt50to80, QCD_Pt80to120, QCD_Pt120to170, #For QCD Estimate
#SMS_T2tt_2J_mStop850_mLSP100, SMS_T2tt_2J_mStop650_mLSP325, SMS_T2tt_2J_mStop500_mLSP325, SMS_T2tt_2J_mStop425_mLSP325, SMS_T2qq_2J_mStop600_mLSP550, SMS_T2qq_2J_mStop1200_mLSP100, SMS_T2bb_2J_mStop900_mLSP100, SMS_T2bb_2J_mStop600_mLSP580, SMS_T1tttt_2J_mGl1500_mLSP100, SMS_T1tttt_2J_mGl1200_mLSP800, SMS_T1qqqq_2J_mGl1400_mLSP100, SMS_T1qqqq_2J_mGl1000_mLSP800, SMS_T1bbbb_2J_mGl1500_mLSP100, SMS_T1bbbb_2J_mGl1000_mLSP900, # SMS
#DYJetsToLL_M50_HT100to200, DYJetsToLL_M50_HT200to400, DYJetsToLL_M50_HT400to600, DYJetsToLL_M50_HT600toInf # DYJetsToLL_M50_HT
#]

    from CMGTools.RootTools.samples.samples_13TeV_74X import *
### 25 ns
#    selectedComponents = [ 
#TTJets, TTJets_LO, # TTJets
#QCD_Pt80to120, QCD_Pt120to170, QCD_Pt300to470, QCD_Pt470to600, QCD_Pt1000to1400, QCD_Pt1400to1800, QCD_Pt1800to2400, QCD_Pt2400to3200, QCD_Pt3200toInf, # QCD_Pt
#]

### 25
#    selectedComponents = SignalSUSY + SignalEXO ### Signal Spring15
    selectedComponents = ZJetsToNuNuHT + DYJetsM50HT + QCDPt + QCDHT + [ 
GJets_HT100to200,
GJets_HT200to400,
GJets_HT400to600,
GJets_HT600toInf,
TTJets_LO, 
WJetsToLNu_HT100to200,
WJetsToLNu_HT200to400,
WJetsToLNu_HT400to600,
WJetsToLNu_HT600toInf,
] + SingleTop ### Full SM BG Spring15
    
    # test all components (1 thread per component).
    for comp in selectedComponents:
        comp.splitFactor = 1200
        #comp.fineSplitFactor = 2 # to run two jobs per file
        comp.files = comp.files[:]
        #comp.files = comp.files[:1]
        #comp.files = comp.files[57:58]  # to process only file [57]  
        # triggers on MC
        #comp.triggers = triggers_HT900 + triggers_HTMET + triggers_photon155 + triggers_1mu_isolow + triggers_MT2_mumu + triggers_MT2_ee + triggers_MT2_mue # to apply trigger skimming

elif test==3:
    # run on data
    isData = True
    from CMGTools.RootTools.samples.samples_13TeV_DATA2015 import *

    #synche file DATA
    #comp = JetHT_Run2015B_PromptReco
    #comp.files = ['/afs/cern.ch/user/m/mangano/public/MECCA/dataset/74X/data/JetHT_promptReco_Run2015B.root']
    #comp.files = ['root://eoscms.cern.ch//eos/cms/store/data/Run2015B/JetHT/MINIAOD/PromptReco-v1/000/251/643/00000/0AF95D60-992C-E511-8D36-02163E0146A4.root']
    #selectedComponents = [comp]

    ##selectedComponents = [JetHT_Run2015B, HTMHT_Run2015B, MET_Run2015B, SingleElectron_Run2015B, SingleMuon_Run2015B, SinglePhoton_Run2015B, DoubleEG_Run2015B, DoubleMuon_Run2015B, MuonEG_Run2015B]
    #selectedComponents = [JetHT_Run2015B_17Jul2015, HTMHT_Run2015B_17Jul2015, MET_Run2015B_17Jul2015, SingleElectron_Run2015B_17Jul2015, SingleMuon_Run2015B_17Jul2015, SinglePhoton_Run2015B_17Jul2015, DoubleEG_Run2015B_17Jul2015, MuonEG_Run2015B_17Jul2015, DoubleMuon_Run2015B_17Jul2015, JetHT_Run2015B_PromptReco, HTMHT_Run2015B_PromptReco, MET_Run2015B_PromptReco, SingleElectron_Run2015B_PromptReco, SingleMuon_Run2015B_PromptReco, SinglePhoton_Run2015B_PromptReco, DoubleEG_Run2015B_PromptReco, MuonEG_Run2015B_PromptReco, DoubleMuon_Run2015B_PromptReco]

    selectedComponents = [JetHT_Run2015D, HTMHT_Run2015D, MET_Run2015D, SingleElectron_Run2015D, SingleMuon_Run2015D, SinglePhoton_Run2015D, DoubleEG_Run2015D, MuonEG_Run2015D, DoubleMuon_Run2015D]

elif test==4:

    from CMGTools.RootTools.samples.samples_13TeV_signals import *

### 25
    selectedComponents = SignalSUSY + SignalEXO ### Signal Spring15
    
    # test all components (1 thread per component).
    for comp in selectedComponents:
        comp.splitFactor = 1200
        #comp.fineSplitFactor = 2 # to run two jobs per file
        comp.files = comp.files[:]
        # triggers on MC
        #comp.triggers = triggers_HT900 + triggers_HTMET + triggers_photon155 + triggers_1mu_isolow + triggers_MT2_mumu + triggers_MT2_ee + triggers_MT2_mue # to apply trigger skimming

# ------------------------------------------------------------------------------------------- #


if doSpecialSettingsForMECCA:
    jetAna.doQG = False
    photonAna.do_randomCone = False
    # Below slow things note: it will in any case try it only on MC, not on data
    photonAna.do_mc_match = False
    jetAna.do_mc_match = False
    lepAna.do_mc_match = False
    isoTrackAna.do_mc_match = False
    genAna.makeLHEweights = False

if isData:
    for comp in samples:
        comp.isMC = False
        comp.isData = True
        #comp.files = ['/afs/cern.ch/user/d/dalfonso/public/74samples/JetHT_GR_R_74_V12_19May_RelVal/1294BDDB-B7FE-E411-8028-002590596490.root']



# ------------------------------------------------------------------------------------------- #


from PhysicsTools.HeppyCore.framework.services.tfile import TFileService 
output_service = cfg.Service(
      TFileService,
      'outputfile',
      name="outputfile",
      fname='mt2.root',
      option='recreate'
    )

# -------------------- Running Download from EOS

# the following is declared in case this cfg is used in input to the heppy.py script
from PhysicsTools.HeppyCore.framework.eventsfwlite import Events
from CMGTools.TTHAnalysis.tools.EOSEventsWithDownload import EOSEventsWithDownload
event_class = EOSEventsWithDownload
if getHeppyOption("nofetch"):
    event_class = Events



removeResiduals = False

# -------------------- Running pre-processor
import subprocess

if isData:
    uncFile = '$CMSSW_BASE/src/CMGTools/RootTools/data/jec/Summer15_50nsV4_DATA_UncertaintySources_AK4PFchs.txt'
    jecDBFile = '$CMSSW_BASE/src/CMGTools/RootTools/data/jec/Summer15_50nsV4_DATA.db'
    jecEra    = 'Summer15_50nsV4_DATA'
else:
#    uncFile = '$CMSSW_BASE/src/CMGTools/RootTools/data/jec/Summer15_50nsV4_DATA_UncertaintySources_AK4PFchs.txt'
#    jecDBFile = '$CMSSW_BASE/src/CMGTools/RootTools/data/jec/Summer15_50nsV4_MC.db'
#    jecEra    = 'Summer15_50nsV4_MC'
    uncFile = '$CMSSW_BASE/src/CMGTools/RootTools/data/jec/Summer15_25nsV2_MC_UncertaintySources_AK4PFchs.txt'
    jecDBFile = '$CMSSW_BASE/src/CMGTools/RootTools/data/jec/Summer15_25nsV2_MC.db'
    jecEra    = 'Summer15_25nsV2_MC'
preprocessorFile = "$CMSSW_BASE/tmp/MetType1_jec_%s.py"%(jecEra)
extraArgs=[]
if isData:
  extraArgs.append('--isData')
  GT= '74X_dataRun2_Prompt_v1'
else:
  GT= 'MCRUN2_74_V9A'
if removeResiduals:extraArgs.append('--removeResiduals')
args = ['python',
  os.path.expandvars('$CMSSW_BASE/python/CMGTools/ObjectStudies/corMETMiniAOD_cfgCreator.py'),\
  '--GT='+GT,
  '--outputFile='+preprocessorFile,
  '--jecDBFile='+jecDBFile,
  '--uncFile='+uncFile,
  '--jecEra='+jecEra
  ] + extraArgs
#print "Making pre-processorfile:"
#print " ".join(args)
subprocess.call(args)
from PhysicsTools.Heppy.utils.cmsswPreprocessor import CmsswPreprocessor
preprocessor = CmsswPreprocessor(preprocessorFile)


config = cfg.Config( components = selectedComponents,
                     sequence = sequence,
                     services = [output_service],
                     #preprocessor=preprocessor, # comment if pre-processor non needed
#                     events_class = event_class)
                     events_class = Events)
#printComps(config.components, True)
