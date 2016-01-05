import PhysicsTools.HeppyCore.framework.config as cfg
from PhysicsTools.Heppy.analyzers.core.all import *
from PhysicsTools.Heppy.analyzers.objects.all import *
from PhysicsTools.Heppy.analyzers.gen.all import *
from CMGTools.VVResonances.analyzers.LNuJJ import *
from CMGTools.VVResonances.analyzers.LLJJ import *
from CMGTools.VVResonances.analyzers.JJ import *
from CMGTools.VVResonances.analyzers.MultiFinalState  import *
from CMGTools.VVResonances.analyzers.PackedCandidateLoader import *
from CMGTools.VVResonances.analyzers.LeptonicVMaker import *
from CMGTools.VVResonances.analyzers.Skimmer import *
from CMGTools.VVResonances.tools.leptonID  import *
from PhysicsTools.HeppyCore.utils.deltar import *
import os





# Pick individual events (normally not in the path)
eventSelector = cfg.Analyzer(
    EventSelector,name="EventSelector",
    toSelect = []  # here put the event numbers (actual event numbers from CMSSW)
    )

skimAnalyzer = cfg.Analyzer(
    SkimAnalyzerCount, name='skimAnalyzerCount',
    useLumiBlocks = False,
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
    triggerBits = {
    }
    )


# Create flags for MET filter bits

eventFlagsAna = cfg.Analyzer(
    TriggerBitAnalyzer, name="EventFlags",
    processName = 'PAT',
    fallbackProcessName = 'RECO', 
    outprefix   = 'Flag',
    triggerBits = {
        "HBHENoiseFilter" : [ "Flag_HBHENoiseFilter" ],
        "HBHENoiseIsoFilter" : [ "Flag_HBHENoiseIsoFilter" ],
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


## Gen Info Analyzer (generic, but should be revised)
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

#pdfwAna = cfg.Analyzer(
#    PDFWeightsAnalyzer, name="PDFWeightsAnalyzer",
#    PDFWeights = [ pdf for pdf,num in PDFWeights ]
#    )

lepAna = cfg.Analyzer(
    LeptonAnalyzer, name="leptonAnalyzer",
    # input collections
    muons='slimmedMuons',
    electrons='slimmedElectrons',
    rhoMuon= 'fixedGridRhoFastjetAll',
    rhoElectron = 'fixedGridRhoFastjetAll',
    # energy scale corrections and ghost muon suppression (off by default)
    doMuScleFitCorrections=False, # "rereco"
    doRochesterCorrections=False,
    doElectronScaleCorrections=False, # "embedded" in 5.18 for regression
    doSegmentBasedMuonCleaning=False,
    # inclusive very loose muon selection
    inclusive_muon_id  = "",
    inclusive_muon_pt  = 15.0,
    inclusive_muon_eta = 2.4,
    inclusive_muon_dxy = 0.2,
    inclusive_muon_dz  = 0.2,
    muon_dxydz_track = "innerTrack",
    # loose muon selection
    loose_muon_id     = "",
    loose_muon_pt     = 20.0,
    loose_muon_eta    = 2.4,
    loose_muon_dxy    = 0.02,
    loose_muon_dz     = 0.2,
    loose_muon_isoCut = muonIDCommon,
    # inclusive very loose electron selection
    inclusive_electron_id  = "",
    inclusive_electron_pt  = 15.0,
    inclusive_electron_eta = 2.5,
    inclusive_electron_dxy = 0.2,
    inclusive_electron_dz  = 0.2,
    inclusive_electron_lostHits = 1.0,
    # loose electron selection
    loose_electron_id     = "",
    loose_electron_pt     = 35.0,
    loose_electron_eta    = 2.5,
    loose_electron_dxy    = 0.2,
    loose_electron_dz     = 0.2,
    loose_electron_isoCut = electronID,
    loose_electron_lostHits = 1.0,
    # muon isolation correction method (can be "rhoArea" or "deltaBeta")
    mu_isoCorr = "deltaBeta",
    mu_effectiveAreas = "Spring15_25ns_v1", #(can be 'Data2012' or 'Phys14_25ns_v1')
    # electron isolation correction method (can be "rhoArea" or "deltaBeta")
    ele_isoCorr = "rhoArea" ,
    el_effectiveAreas = "Spring15_25ns_v1" , #(can be 'Data2012' or 'Phys14_25ns_v1')
    ele_tightId = "" ,
    # Mini-isolation, with pT dependent cone: will fill in the miniRelIso, miniRelIsoCharged, miniRelIsoNeutral variables of the leptons (see https://indico.cern.ch/event/368826/ )
    doMiniIsolation = False, # off by default since it requires access to all PFCandidates 
    packedCandidates = 'packedPFCandidates',
    miniIsolationPUCorr = 'deltaBeta', # Allowed options: 'rhoArea' (EAs for 03 cone scaled by R^2), 'deltaBeta', 'raw' (uncorrected), 'weights' (delta beta weights; not validated)
    miniIsolationVetoLeptons = 'inclusive', # use 'inclusive' to veto inclusive leptons and their footprint in all isolation cones
    # minimum deltaR between a loose electron and a loose muon (on overlaps, discard the electron)
    min_dr_electron_muon = 0.0,
    # do MC matching 
    do_mc_match = True, # note: it will in any case try it only on MC, not on data
    match_inclusiveLeptons = False, # match to all inclusive leptons
    )

tauAna = cfg.Analyzer(
    TauAnalyzer, name="tauAnalyzer",
    # inclusive very loose hadronic tau selection
    inclusive_ptMin = 18,
    inclusive_etaMax = 9999,
    inclusive_dxyMax = 1000.,
    inclusive_dzMax = 0.4,
    inclusive_vetoLeptons = False,
    inclusive_leptonVetoDR = 0.4,
    inclusive_decayModeID = "decayModeFindingNewDMs", # ignored if not set or ""
    inclusive_tauID = "decayModeFindingNewDMs",
    inclusive_vetoLeptonsPOG = False, # If True, the following two IDs are required
    inclusive_tauAntiMuonID = "",
    inclusive_tauAntiElectronID = "",
    # loose hadronic tau selection
    loose_ptMin = 18,
    loose_etaMax = 9999,
    loose_dxyMax = 1000.,
    loose_dzMax = 0.2,
    loose_vetoLeptons = True,
    loose_leptonVetoDR = 0.4,
    loose_decayModeID = "decayModeFindingNewDMs", # ignored if not set or ""
    loose_tauID = "byLooseCombinedIsolationDeltaBetaCorr3Hits",
    loose_vetoLeptonsPOG = False, # If True, the following two IDs are required
    loose_tauAntiMuonID = "againstMuonLoose3",
    loose_tauAntiElectronID = "againstElectronLooseMVA5",
    loose_tauLooseID = "decayModeFindingNewDMs"

)



metAna = cfg.Analyzer(
    METAnalyzer, name="metAnalyzer",
    metCollection     = "slimmedMETs",
    noPUMetCollection = "slimmedMETs",    
    copyMETsByValue = False,
    doTkMet = False,
    doMetNoPU = True,
    doMetNoMu = False,
    doMetNoEle = False,
    doMetNoPhoton = False,
    recalibrate = False, # or "type1", or True
    applyJetSmearing = False, # does nothing unless the jet smearing is turned on in the jet analyzer
    old74XMiniAODs = False, # set to True to get the correct Raw MET when running on old 74X MiniAODs
    jetAnalyzerPostFix = "",
    candidates='packedPFCandidates',
    candidatesTypes='std::vector<pat::PackedCandidate>',
    dzMax = 0.1,
    collectionPostFix = "",
    )


leptonicVAna = cfg.Analyzer(
    LeptonicVMaker,
    name='leptonicVMaker',
    selectLNuPair=(lambda x:  isolationW(x) and leptonIDW(x) ),
    selectLLPair=(lambda x: x.mass()>60.0 and x.mass()<120.0 and isolationZ(x) )
    )


packedAna = cfg.Analyzer(
    PackedCandidateLoader,
    name = 'PackedCandidateLoader',
    select=lambda x: x.pt()<13000.0

)

multiStateAna = cfg.Analyzer(
    MultiFinalState,
    name='MultiFinalStateMaker',
    ktPowerFat = -1.0,
    rFat = 0.8,
    massdrop=True,
    subjets=2,
    doCHS = True,
    prunning=True,
    softdrop = True,
    softdrop_beta=0.0,
    softdrop_zeta=0.1,
    selectFat = (lambda x: x.pt()>200.0 and abs(x.eta())<2.4 and x.prunedJet.mass()>0.0 and len(x.subjets)>1 and x.looseID),
    ktPower=-1.0,
    r = 0.4,
    selectPairLL = (lambda x:  x.mass()>0 and x.deltaPhi()>1.5 and x.leg1.pt()>200  and ((abs(x.leg1.leg1.pdgId())==11 and max(x.leg1.leg1.pt(),x.leg1.leg2.pt())>80) or (abs(x.leg1.leg1.pdgId())==13 and max(x.leg1.leg1.pt(),x.leg1.leg2.pt())>50))),
    selectPairLNu = (lambda x: x.deltaPhi()>1.5 and x.leg1.pt()>200 and ((abs(x.leg1.leg1.pdgId())==11 and x.leg1.leg2.pt()>80) or (abs(x.leg1.leg1.pdgId())==13 and x.leg1.leg2.pt()>40))),
    selectPairJJ = (lambda x:  x.mass()>1000 and x.leg1.tightID and x.leg2.tightID),
    selectPairJJNuNu = (lambda x: x.leg1.pt()>200 and x.deltaPhi()>1.5 ),
    suffix = '',
    recalibrateJets = True, # True, False, 'MC', 'Data'
    recalibrationType = "AK4PFchs",
    recalibrationTypeFAT = "AK8PFchs",
    jecPath = "%s/src/CMGTools/RootTools/data/jec/" % os.environ['CMSSW_BASE'],
    shiftJEC = 0, # set to +1 or -1 to get +/-1 sigma shifts
    rho = ('fixedGridRhoFastjetAll','',''),
    attachBTag = True,
    btagDiscriminator = "pfCombinedInclusiveSecondaryVertexV2BJetTags",
    standardJets = 'slimmedJets',
    fatJets = 'slimmedJetsAK8',
    subJets = 'slimmedJetsAK8PFCHSSoftDropPacked',
    doSkim = True
    )



    

coreSequence = [
   #eventSelector,
    skimAnalyzer,
    jsonAna,
    triggerAna,
    pileUpAna,
    genAna,
#    pdfwAna,
    vertexAna,
    lepAna,
    metAna,
    leptonicVAna,
#    tauAna,
    packedAna,
    multiStateAna,
    eventFlagsAna,
    triggerFlagsAna    
]
