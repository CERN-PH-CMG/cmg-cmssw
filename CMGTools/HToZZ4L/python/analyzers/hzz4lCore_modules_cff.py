import PhysicsTools.HeppyCore.framework.config as cfg
from PhysicsTools.Heppy.analyzers.core.all import *
from PhysicsTools.Heppy.analyzers.objects.all import *
from PhysicsTools.Heppy.analyzers.gen.all import *
from CMGTools.HToZZ4L.analyzers.FourLeptonAnalyzer import *
from CMGTools.HToZZ4L.analyzers.FourLeptonAnalyzer2P2F import *
from CMGTools.HToZZ4L.analyzers.FourLeptonAnalyzerRelaxIdIso import *
from CMGTools.HToZZ4L.analyzers.FourLeptonAnalyzer3P1F import *
from CMGTools.HToZZ4L.analyzers.FourLeptonAnalyzerSS import *
from CMGTools.HToZZ4L.analyzers.FourLeptonEventSkimmer import *

from CMGTools.HToZZ4L.analyzers.FSRPhotonMaker import *
from CMGTools.HToZZ4L.analyzers.GenFSRAnalyzer import *
from CMGTools.HToZZ4L.analyzers.fourLeptonTree import *
from CMGTools.HToZZ4L.analyzers.GenDPhiZZWeight import GenDPhiZZWeight
from CMGTools.HToZZ4L.samples.samples_13TeV_Spring15 import *

from CMGTools.TTHAnalysis.analyzers.ttHFastLepSkimmer import ttHFastLepSkimmer
fastSkim2LnoSip = cfg.Analyzer( ttHFastLepSkimmer, name="fastLepSkim2LnoSIP",
        muons = 'slimmedMuons', muCut = lambda mu : mu.pt() > 5,
        electrons = 'slimmedElectrons', eleCut = lambda ele : ele.pt() > 7,
        minLeptons = 2,
)
fastSkim2L = fastSkim2LnoSip.clone(name="fastLepSkim2L",
        muCut = lambda mu : mu.pt() > 5 and abs(mu.dB(mu.PV3D) / mu.edB(mu.PV3D)) < 4,
        eleCut = lambda ele : ele.pt() > 7 and (abs(ele.dB(ele.PV3D)) <= 4*ele.edB(ele.PV3D)),
)
fastSkim3L = fastSkim2L.clone(name="fastLepSkim3L", minLeptons = 3)
fastSkim4L = fastSkim2L.clone(name="fastLepSkim3L", minLeptons = 4)

genAna = cfg.Analyzer(
    GeneratorAnalyzer, name="GeneratorAnalyzer",
    # BSM particles that can appear with status <= 2 and should be kept
    stableBSMParticleIds = [ 1000022 ],
    # Particles of which we want to save the pre-FSR momentum (a la status 3).
    # Note that for quarks and gluons the post-FSR doesn't make sense,
    # so those should always be in the list
    savePreFSRParticleIds = [ 1,2,3,4,5, 11,12,13,14,15,16, 21 ],
    # Make also the list of all genParticles, for other analyzers to handle
    makeAllGenParticles = True,
    # Make also the splitted lists
    makeSplittedGenLists = True,
    allGenTaus = False,
    # Save LHE weights from LHEEventProduct
    makeLHEweights = True,
    # Print out debug information
    verbose = False,
    )


genFSRAna = cfg.Analyzer(
    GenFSRAnalyzer, name="GenFSRAnalyzer"
    )
genDPhiZZWeight = cfg.Analyzer(
    GenDPhiZZWeight, name="genDPhiZZWeight"
)

# Find the initial events before the skim
skimAnalyzer = cfg.Analyzer(
    SkimAnalyzerCount, name='skimAnalyzerCount',
    useLumiBlocks = False,
    )

# Pick individual events (normally not in the path)
eventSelector = cfg.Analyzer(
    EventSelector,name="EventSelector",
    toSelect = []  # here put the event numbers (actual event numbers from CMSSW)
    )
# Apply json file (if the dataset has one)
jsonAna = cfg.Analyzer(
    JSONAnalyzer, name="JSONAnalyzer",
    )

# Filter using the 'triggers' and 'vetoTriggers' specified in the dataset
triggerAna = cfg.Analyzer(
    TriggerBitFilter, name="TriggerBitFilter",
    )

# Create flags for trigger bits
triggerFlagsAna = cfg.Analyzer(
    TriggerBitAnalyzer, name="TriggerFlags",
    processName = 'HLT',
    prescaleProcessName = 'PAT',
    prescaleFallbackProcessName = 'RECO',
    unrollbits = False,
    saveIsUnprescaled = False,
    checkL1prescale = False,
    triggerBits = {
        # Doubles
        'DoubleMu' : triggers_mumu,
        'DoubleEl' : triggers_ee,
        'MuEG'     : triggers_mue,
        # Triples
        'TripleEl' : triggers_3e,
        'TripleMu' : triggers_3mu,
        'DoubleMuEl' : triggers_2mu1e,
        'DoubleElMu' : triggers_2e1mu,
        # Singles
        'SingleEl' : triggers_1e,
        'SingleMu' : triggers_1mu,
        # Old Sync
        'DoubleMuSync' : triggers_mumu_sync,
        'DoubleElSync' : triggers_ee_sync,
        'MuEGSync'     : triggers_mue_sync,
        'SingleElSync' : triggers_1e_sync,
        # Summaries 
        'Signal' : triggers_signal_real,
        'SignalSync' : triggers_signal_sync,
        }
    )


# Select a list of good primary vertices (generic)
vertexAna = cfg.Analyzer(
    VertexAnalyzer, name="VertexAnalyzer",
    vertexWeight = None,
    fixedWeight = 1,
    verbose = False
    )


# This analyzer actually does the pile-up reweighting (generic)
pileUpAna = cfg.Analyzer(
    PileUpAnalyzer, name="PileUpAnalyzer",
    true = True,  # use number of true interactions for reweighting
    makeHists=False
    )

lepAna = cfg.Analyzer(
    LeptonAnalyzer, name="leptonAnalyzer",
    # input collections
    muons='slimmedMuons',
    electrons='slimmedElectrons',
    rhoMuon= 'fixedGridRhoFastjetAll',
    rhoElectron = 'fixedGridRhoFastjetAll',
    # energy scale corrections and ghost muon suppression (off by default)
    doMuonScaleCorrections=False,
    doElectronScaleCorrections=False, # "embedded" in 5.18 for regression
    doSegmentBasedMuonCleaning=True,
    notCleaningElectrons=True, # no deltaR(ele,mu) cleaning at this step
    # inclusive very loose muon selection
    inclusive_muon_id  = "POG_Global_OR_TMArbitrated",
    inclusive_muon_pt  = 5,
    inclusive_muon_eta = 2.4,
    inclusive_muon_dxy = 0.5,
    inclusive_muon_dz  = 1.0,
    muon_dxydz_track   = "muonBestTrack",
    # loose muon selection
    loose_muon_id     = "POG_Global_OR_TMArbitrated",
    loose_muon_pt     = 5,
    loose_muon_eta    = 2.4,
    loose_muon_dxy    = 0.5,
    loose_muon_dz     = 1,
    loose_muon_isoCut = lambda muon : muon.sip3D() < 4 and muon.muonBestTrackType() != 2,
    # inclusive very loose electron selection
    inclusive_electron_id  = "",
    inclusive_electron_pt  = 7,
    inclusive_electron_eta = 2.5,
    inclusive_electron_dxy = 0.5,
    inclusive_electron_dz  = 1.0,
    inclusive_electron_lostHits = 10000.0,
    # loose electron selection
    loose_electron_id     = "",
    loose_electron_pt     = 7,
    loose_electron_eta    = 2.5,
    loose_electron_dxy    = 0.5,
    loose_electron_dz     = 1.0,
    loose_electron_isoCut = lambda x: x.sip3D() < 4,
    loose_electron_lostHits = 1000.0,
    # muon isolation correction method (can be "rhoArea" or "deltaBeta")
    mu_isoCorr = "deltaBeta" ,
    mu_effectiveAreas = "Phys14_25ns_v1", #(can be 'Data2012' or 'Phys14_25ns_v1')
    mu_tightId = "POG_ID_Loose",
    # electron isolation correction method (can be "rhoArea" or "deltaBeta")
    ele_isoCorr = "rhoArea" ,
    ele_effectiveAreas = "Phys14_25ns_v1" , #(can be 'Data2012' or 'Phys14_25ns_v1')
    ele_tightId = "MVA_ID_NonTrig_Spring15_HZZ",
    # Mini-isolation, with pT dependent cone: will fill in the miniRelIso, miniRelIsoCharged, miniRelIsoNeutral variables of the leptons (see https://indico.cern.ch/event/368826/ )
    doMiniIsolation = False, # off by default since it requires access to all PFCandidates 
    packedCandidates = 'packedPFCandidates',
    miniIsolationPUCorr = 'rhoArea', # Allowed options: 'rhoArea' (EAs for 03 cone scaled by R^2), 'deltaBeta', 'raw' (uncorrected), 'weights' (delta beta weights; not validated)
    miniIsolationVetoLeptons = None, # use 'inclusive' to veto inclusive leptons and their footprint in all isolation cones
    # minimum deltaR between a loose electron and a loose muon (on overlaps, discard the electron)
    min_dr_electron_muon = 100.0,
    # do MC matching 
    do_mc_match = True, # note: it will in any case try it only on MC, not on data
    match_inclusiveLeptons = False, # match to all inclusive leptons
    do_mc_match_photons = "all", # do MC matching to all photons (packed gen particles) 
    )

from CMGTools.HToZZ4L.analyzers.ElectronMuonCleaner import ElectronMuonCleaner
eleMuClean = cfg.Analyzer(
    ElectronMuonCleaner, name='eleMuClean',
    selectedMuCut = lambda mu : mu.tightId(), #isPFMuon() or mu.isGlobalMuon(),
    otherMuCut    = lambda mu : False, # (mu.isPFMuon() or mu.isGlobalMuon()) and muon.muonBestTrackType() != 2, # uncomment to include also muons with sip > 4
    mustClean = lambda ele, mu, dr: dr < 0.05
)

fsrRecovery = cfg.Analyzer(
    FSRPhotonMaker, name="fsrPhotonMaker",
    leptons="selectedLeptons",
    electronID = lambda x: True, #x.electronID("POG_MVA_ID_Run2_NonTrig_HZZ")
    electronVeto = "superclusterEta", # alternatives: "electronEta" and in the future "pfCandReference"
    drOverET2Cut = 0.012,
    relIsoCut = 1.8, 
)

## Jets Analyzer (generic)
jetAna = cfg.Analyzer(
    JetAnalyzer, name='jetAnalyzer',
    jetCol = 'slimmedJets',
    copyJetsByValue = False,      #Whether or not to copy the input jets or to work with references (should be 'True' if JetAnalyzer is run more than once)
    genJetCol = 'slimmedGenJets',
    rho = ('fixedGridRhoFastjetAll','',''),
    jetPt = 30.,
    jetEta = 4.7,
    jetEtaCentral = 4.7,
    jetLepDR = 0.4,
    jetLepArbitration = (lambda jet,lepton : lepton), # you can decide which to keep in case of overlaps; e.g. if the jet is b-tagged you might want to keep the jet
    cleanSelectedLeptons = False, #Whether to clean 'selectedLeptons' after disambiguation. Treat with care (= 'False') if running Jetanalyzer more than once
    minLepPt = 0,
    lepSelCut = lambda lepton : lepton.tightId() and lepton.relIsoAfterFSR < (0.4 if abs(lepton.pdgId())==13 else 0.5),
    relaxJetId = False,  
    doPuId = False,
    recalibrateJets = False, # True, False, 'MC', 'Data'
    applyL2L3Residual = True, # Switch to 'Data' when they will become available for Data
    recalibrationType = "AK4PFchs",
    mcGT     = "Summer15_25nsV2_MC",
    dataGT   = "Summer15_25nsV5_DATA",
    jecPath = "${CMSSW_BASE}/src/CMGTools/RootTools/data/jec/",
    shiftJEC = 0, # set to +1 or -1 to get +/-1 sigma shifts
    addJECShifts = False,
    smearJets = False,
    shiftJER = 0, # set to +1 or -1 to get +/-1 sigma shifts  
    alwaysCleanPhotons = True,
    cleanGenJetsFromPhoton = False,
    cleanJetsFromFirstPhoton = False,
    cleanJetsFromTaus = False,
    cleanJetsFromIsoTracks = False,
    doQG = False,
    do_mc_match = True,
    collectionPostFix = "",
    calculateSeparateCorrections = False,
    calculateType1METCorrection  = False,
    type1METParams = { 'jetPtThreshold':15., 'skipEMfractionThreshold':0.9, 'skipMuons':True },
    )


metAna = cfg.Analyzer(
    METAnalyzer, name="metAnalyzer",
    metCollection     = "slimmedMETs",
    noPUMetCollection = "slimmedMETs",    
    copyMETsByValue = False,
    doTkMet = False,
    doMetNoPU = False,
    doMetNoMu = False,
    doMetNoEle = False,
    doMetNoPhoton = False,
    recalibrate = False, #"type1", # or "type1", or True
    applyJetSmearing = False, # does nothing unless the jet smearing is turned on in the jet analyzer
    old74XMiniAODs = False, # set to True to get the correct Raw MET when running on old 74X MiniAODs
    jetAnalyzerPostFix = "",
    candidates='packedPFCandidates',
    candidatesTypes='std::vector<pat::PackedCandidate>',
    dzMax = 0.1,
    collectionPostFix = "",
    )
metNoHFAna = metAna.clone( 
    name="metNoHFAnalyzer",
    metCollection     = "slimmedMETsNoHF",
    noPUMetCollection = "slimmedMETsNoHF",
    collectionPostFix = "NoHF",
    )




fourLeptonAnalyzerSignal = cfg.Analyzer(
    FourLeptonAnalyzer, name="fourLeptonAnalyzerSignal",
    tag = "Signal",
    attachFsrToGlobalClosestLeptonOnly = True
)

fourLeptonAnalyzer2P2F = cfg.Analyzer(
    FourLeptonAnalyzer2P2F, name="fourLeptonAnalyzer2P2F",
    tag = "2P2F",
    maxCand = 999, # save all, not just the best one
    attachFsrToGlobalClosestLeptonOnly = True
)

fourLeptonAnalyzer3P1F = cfg.Analyzer(
    FourLeptonAnalyzer3P1F, name="fourLeptonAnalyzer3P1F",
    tag = "3P1F",
    maxCand = 999, # save all, not just the best one
    attachFsrToGlobalClosestLeptonOnly = True
)

fourLeptonAnalyzerSS = cfg.Analyzer(
    FourLeptonAnalyzerSS, name="fourLeptonAnalyzerSS",
    tag = "SS",
    maxCand = 999, # save all, not just the best one
    attachFsrToGlobalClosestLeptonOnly = True
)

fourLeptonAnalyzerRelaxIdIso = cfg.Analyzer(
    FourLeptonAnalyzerRelaxIdIso, name="fourLeptonAnalyzerRelaxIdIso",
    tag = "RelaxIdIso",
    maxCand = 999, # save all, not just the best one
    attachFsrToGlobalClosestLeptonOnly = True
)

fourLeptonEventSkimmer = cfg.Analyzer(
    FourLeptonEventSkimmer, name="fourLeptonEventSkimmer",
    required = ['bestFourLeptonsSignal','bestFourLeptons2P2F','bestFourLeptons3P1F','bestFourLeptonsSS', 'bestFourLeptonsRelaxIdIso' ]

)

from PhysicsTools.HeppyCore.framework.services.tfile import TFileService
output_service = cfg.Service( TFileService, 'outputfile', name="outputfile",
    fname='tree.root',
    option='recreate'
    )    

treeProducer = cfg.Analyzer(
     AutoFillTreeProducer, name='fourLeptonTreeProducer',
     vectorTree = False,
     saveTLorentzVectors = False,  # can set to True to get also the TLorentzVectors, but trees will be bigger
     globalVariables = hzz_globalVariables,
     globalObjects = hzz_globalObjects,
     collections = hzz_collections,
     defaultFloatType = 'F',
)


def doECalCorrections(sync=False,era="25ns"):
    global lepAna, fastSkim4L, fastSkim2L, fastSkim3L
    lepAna.doElectronScaleCorrections = {
        'GBRForest': ('$CMSSW_BASE/src/CMGTools/RootTools/data/egamma_epComb_GBRForest_74Xv2.root',
                      'gedelectron_p4combination_'+era),
        'isSync': sync
    }
    fastSkim2L.eleCut = lambda ele : ele.pt() > 7*0.97/(1+10*0.032) and (abs(ele.dB(ele.PV3D)) <= 4*ele.edB(ele.PV3D))
    fastSkim3L.eleCut = lambda ele : ele.pt() > 7*0.97/(1+10*0.032) and (abs(ele.dB(ele.PV3D)) <= 4*ele.edB(ele.PV3D))
    fastSkim4L.eleCut = lambda ele : ele.pt() > 7*0.97/(1+10*0.032) and (abs(ele.dB(ele.PV3D)) <= 4*ele.edB(ele.PV3D))
def doKalmanMuonCorrections(sync=False):
    global lepAna, fastSkim4L, fastSkim2L, fastSkim3L
    lepAna.doMuonScaleCorrections = ( 'Kalman', {
        'MC': 'MC_74X_13TeV',
        'Data': 'DATA_Prompt_13TeV',
        'isSync': sync
    })
    fastSkim2L.muCut = lambda mu : mu.pt() > 3 and (abs(mu.dB(mu.PV3D)) <= 4*mu.edB(mu.PV3D))
    fastSkim3L.muCut = lambda mu : mu.pt() > 3 and (abs(mu.dB(mu.PV3D)) <= 4*mu.edB(mu.PV3D))
    fastSkim4L.muCut = lambda mu : mu.pt() > 3 and (abs(mu.dB(mu.PV3D)) <= 4*mu.edB(mu.PV3D))

# Core sequence of all common modules
hzz4lPreSequence = [
    skimAnalyzer,
    jsonAna,
    triggerAna,
]
hzz4lObjSequence = [
    genAna,
    genDPhiZZWeight,
    pileUpAna,
    vertexAna,
    lepAna,
    eleMuClean,
    fsrRecovery,
    jetAna,
    metAna,
    metNoHFAna,
    triggerFlagsAna,
]
hzz4lCoreSequence = hzz4lPreSequence +  [ fastSkim4L ] + hzz4lObjSequence + [
    fourLeptonAnalyzerSignal, 
    fourLeptonAnalyzer2P2F,
    fourLeptonAnalyzer3P1F,
    fourLeptonAnalyzerSS,
    fourLeptonAnalyzerRelaxIdIso,
    fourLeptonEventSkimmer,
    treeProducer
]
