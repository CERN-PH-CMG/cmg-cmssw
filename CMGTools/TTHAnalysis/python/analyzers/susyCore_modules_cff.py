##########################################################
##          SUSY COMMON MODULES ARE DEFINED HERE        ##
## skimming modules are configured to not cut anything  ##
##########################################################

import CMGTools.RootTools.fwlite.Config as cfg

PDFWeights = []
#PDFWeights = [ ("CT10",53), ("MSTW2008lo68cl",41), ("NNPDF21_100",101) ]

# Find the initial events before the skim
skimAnalyzer = cfg.Analyzer(
    'skimAnalyzerCount'
    )

# Pick individual events (normally not in the path)
eventSelector = cfg.Analyzer(
    'EventSelector',
    toSelect = []  # here put the event numbers (actual event numbers from CMSSW)
    )

# Apply json file (if the dataset has one)
jsonAna = cfg.Analyzer(
    'JSONAnalyzer',
    )

# Filter using the 'triggers' and 'vetoTriggers' specified in the dataset
triggerAna = cfg.Analyzer(
    #'TriggerAnalyzer',
    'triggerBitFilter',
    )


# Select a list of good primary vertices (generic)
ttHVertexAna = cfg.Analyzer(
    'VertexAnalyzer',
    vertexWeight = None,
    fixedWeight = 1,
    verbose = False
    )


# This analyzer actually does the pile-up reweighting (generic)
pileUpAna = cfg.Analyzer(
    "PileUpAnalyzer",
    true = True,  # use number of true interactions for reweighting
    makeHists=False
    )


# Gen Info Analyzer (generic, but should be revised)
ttHGenAna = cfg.Analyzer(
    'ttHGenLevelAnalyzer',
    filterHiggsDecays = False, 
    verbose = False,
    PDFWeights = [ pdf for pdf,num in PDFWeights ]
    )

# Lepton Analyzer (generic)
susyScanAna = cfg.Analyzer(
    'susyParameterScanAnalyzer',
    )

# gen particles in Pythia 6
susyPythia6Gen  = cfg.Analyzer(
    'susyGenStatusThree',
    pythia6only = False, # True = get status 3; False = try get interesting event whatever generator is used
    verbose     = False,
    )

# Lepton Analyzer (generic)
ttHLepAna = cfg.Analyzer(
    'ttHLepAnalyzerSusy',
    # input collections
    muons='slimmedMuons',
    electrons='slimmedElectrons',
    rhoMuon= 'fixedGridRhoFastjetAll',
    rhoElectron = 'fixedGridRhoFastjetAll',
##    photons='slimmedPhotons',
    # energy scale corrections and ghost muon suppression (off by default)
    doMuScleFitCorrections=False, # "rereco"
    doRochesterCorrections=False,
    doElectronScaleCorrections=False, # "embedded" in 5.18 for regression
    doSegmentBasedMuonCleaning=False,
    # inclusive very loose muon selection
    inclusive_muon_id  = "POG_ID_Loose",
    inclusive_muon_pt  = 3,
    inclusive_muon_eta = 2.4,
    inclusive_muon_dxy = 0.5,
    inclusive_muon_dz  = 1.0,
    # loose muon selection
    loose_muon_id     = "POG_ID_Loose",
    loose_muon_pt     = 5,
    loose_muon_eta    = 2.4,
    loose_muon_dxy    = 0.05,
    loose_muon_dz     = 0.2,
    loose_muon_relIso = 0.4,
    # inclusive very loose electron selection
    inclusive_electron_id  = "",
    inclusive_electron_pt  = 5,
    inclusive_electron_eta = 2.5,
    inclusive_electron_dxy = 0.5,
    inclusive_electron_dz  = 1.0,
    inclusive_electron_lostHits = 1.0,
    # loose electron selection
    loose_electron_id     = "POG_MVA_ID_NonTrig_full5x5",
    loose_electron_pt     = 7,
    loose_electron_eta    = 2.4,
    loose_electron_dxy    = 0.05,
    loose_electron_dz     = 0.2,
    loose_electron_relIso = 0.4,
    loose_electron_lostHits = 1.0,
    # electron isolation correction method (can be "rhoArea" or "deltaBeta")
    ele_isoCorr = "rhoArea" ,
    ele_tightId = "MVA" ,
    # minimum deltaR between a loose electron and a loose muon (on overlaps, discard the electron)
    min_dr_electron_muon = 0.02
    )

# Lepton-based Skim (generic, but requirements depend on the final state)
ttHLepSkim = cfg.Analyzer(
    'ttHLepSkimmer',
    minLeptons = 0,
    maxLeptons = 999,
    #idCut  = "lepton.relIso03 < 0.2" # can give a cut
    #ptCuts = [20,10],                # can give a set of pt cuts on the leptons
    )

# Lepton MC Matching (generic, must happen early to allow for MVA corrections)
ttHLepMCAna = cfg.Analyzer(
    'ttHLepMCMatchAnalyzer',
    matchAllInclusiveLeptons = False,
    )

# Photon Analyzer (generic)
ttHPhoAna = cfg.Analyzer(
    'ttHPhotonAnalyzerSusy',
    photons='slimmedPhotons',
    ptMin = 20,
    etaMax = 2.5,
    gammaID = "PhotonCutBasedIDLoose",
)

# Tau Analyzer (generic)
ttHTauAna = cfg.Analyzer(
    'ttHTauAnalyzer',
    ptMin = 20,
    etaMax = 9999,
    dxyMax = 0.5,
    dzMax = 1.0,
    vetoLeptons = True,
    leptonVetoDR = 0.4,
    vetoLeptonsPOG = False,
    tauID = "byLooseCombinedIsolationDeltaBetaCorr3Hits",
    tauLooseID = "decayModeFinding",
)

# Tau MC Matching (generic)
ttHTauMCAna = cfg.Analyzer(
    'ttHTauMCMatchAnalyzer',
)


#------------------------------------------
##  ISOLATED TRACK
##------------------------------------------                                                                                                                                                                

# those are the cuts for the nonEMu                                                                                                                                                                         
ttHIsoTrackAna = cfg.Analyzer(
            'ttHIsoTrackAnalyzer',
            setOff=True,
            #####
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
            doSecondVeto = False,
            #####
            doPrune = True
            )


# Jets Analyzer (generic)
ttHJetAna = cfg.Analyzer(
    'ttHJetAnalyzer',
    jetCol = 'slimmedJets',
    jetCol4MVA = 'slimmedJets',
    jetPt = 25.,
    jetEta = 4.7,
    jetEtaCentral = 2.4,
    jetLepDR = 0.4,
    minLepPt = 10,
    relaxJetId = False,  
    doPuId = False, # Not commissioned in 7.0.X
    recalibrateJets = False,
    shiftJEC = 0, # set to +1 or -1 to get +/-1 sigma shifts
    cleanJetsFromTaus = False,
    cleanJetsFromIsoTracks = False,
    doQG = False,
    )

# Jet MC Match Analyzer (generic)
ttHJetMCAna = cfg.Analyzer(
    'ttHJetMCMatchAnalyzer',
    smearJets = True,
    shiftJER = 0, # set to +1 or -1 to get +/-1 sigma shifts
    )

# Fat Jets Analyzer (generic)
ttHFatJetAna = cfg.Analyzer(
    'ttHFatJetAnalyzer',
    jetCol = 'slimmedJetsAK8',
    jetPt = 100.,
    jetEta = 2.4,
    # v--- not implemented for AK8
    #jetLepDR = 0.4,
    #minLepPt = 10,
    relaxJetId = False,  
    # v--- not implemented for AK8
    #doPuId = False, # Not commissioned in 7.0.X
    #recalibrateJets = False,
    #shiftJEC = 0, # set to +1 or -1 to get +/-1 sigma shifts
    )


# Secondary vertex analyzer
ttHSVAnalyzer = cfg.Analyzer(
    'ttHSVAnalyzer'
)

# Secondary vertex analyzer
ttHHeavyFlavourHadronAnalyzer = cfg.Analyzer(
    'ttHHeavyFlavourHadronAnalyzer'
)


ttHMetAna = cfg.Analyzer(
    'ttHMetEventAnalyzer',
    doTkMet = False,
    doMetNoMu = False,
    candidates='packedPFCandidates',
    candidatesTypes='std::vector<pat::PackedCandidate>',
    dzMax = 0.1,
    )

# Core Event Analyzer (computes basic quantities like HT, dilepton masses)
ttHCoreEventAna = cfg.Analyzer(
    'ttHCoreEventAnalyzer',
    maxLeps = 4, ## leptons to consider
    )

# Jet-MET based Skim (generic, but requirements depend on the final state)
ttHJetMETSkim = cfg.Analyzer(
    'ttHJetMETSkimmer',
    jets      = "cleanJets", # jet collection to use
    jetPtCuts = [],  # e.g. [60,40,30,20] to require at least four jets with pt > 60,40,30,20
    jetVetoPt =  0,  # if non-zero, veto additional jets with pt > veto beyond the ones in jetPtCuts
    metCut    =  0,  # MET cut
    htCut     = ('htJet40j', 0), # cut on HT defined with only jets and pt cut 40, at zero; i.e. no cut
                                 # see ttHCoreEventAnalyzer for alternative definitions
    mhtCut    = ('mhtJet40', 0), # cut on MHT defined with all leptons, and jets with pt > 40.
    nBJet     = ('CSVM', 0, "jet.pt() > 30"),     # require at least 0 jets passing CSVM and pt > 30
    )


# Core sequence of all common modules
susyCoreSequence = [
    skimAnalyzer,
   #eventSelector,
    jsonAna,
    triggerAna,
    pileUpAna,
    ttHGenAna,
    susyScanAna,
    susyPythia6Gen,
    ttHVertexAna,
    ttHLepAna,
    ttHLepSkim,
    ttHLepMCAna,
    ttHPhoAna,
    ttHTauAna,
    ttHTauMCAna,
    ttHIsoTrackAna,
    ttHJetAna,
    ttHJetMCAna,
    #ttHFatJetAna,  # out of core sequence for now
    #ttHSVAnalyzer, # out of core sequence for now
    ttHMetAna,
    ttHCoreEventAna,
    ttHJetMETSkim
]
