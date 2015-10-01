#! /usr/bin/env python
import ROOT 
import os, copy
import PhysicsTools.HeppyCore.framework.config as cfg
from PhysicsTools.Heppy.analyzers.core.AutoFillTreeProducer  import *
from DMPD.Heppy.analyzers.ObjectsFormat import *
cfg.Analyzer.nosubdir=True


##############################
### LHEANALYZER         ###
##############################
from PhysicsTools.Heppy.analyzers.gen.LHEAnalyzer import LHEAnalyzer
lheAnalyzer= cfg.Analyzer(
    class_object=LHEAnalyzer,
    )

##############################
### GENANALYZER         ###
##############################
from PhysicsTools.Heppy.analyzers.gen.GeneratorAnalyzer import GeneratorAnalyzer
generatorAnalyzer= cfg.Analyzer(
    verbose=False,
    class_object=GeneratorAnalyzer,
    stableBSMParticleIds = [ 1000022, 9100000, 9000001, 9000002, -9000002, 9100012, 9100022, -9100022, 9900032, 1023 ], # BSM particles that can appear with status <= 2 and should be kept
    # Particles of which we want to save the pre-FSR momentum (a la status 3).
    # Note that for quarks and gluons the post-FSR doesn't make sense,
    # so those should always be in the list
    savePreFSRParticleIds = [ 1,2,3,4,5, 11,12,13,14,15,16, 21 ],
    makeAllGenParticles = True, # Make also the list of all genParticles, for other analyzers to handle
    makeSplittedGenLists = True, # Make also the splitted lists
    allGenTaus = False,
    makeLHEweights = True,
    )

##############################
### PDFANALYZER         ###
##############################
from PhysicsTools.Heppy.analyzers.gen.PDFWeightsAnalyzer import PDFWeightsAnalyzer
pdfAnalyzer= cfg.Analyzer(
    class_object=PDFWeightsAnalyzer,
    doPDFWeights = True,
    doPDFVars = True,
    PDFWeights = ["cteq6ll", "MSTW2008nlo68cl"],
    )

##############################
### TRIGGERANALYZER        ###
##############################
from PhysicsTools.Heppy.analyzers.core.TriggerBitAnalyzer import TriggerBitAnalyzer
triggerAnalyzer= cfg.Analyzer(
    verbose=False,
    class_object=TriggerBitAnalyzer,
    #grouping several paths into a single flag
    # v* can be used to ignore the version of a path
    triggerBits={
        'SingleMu'       : ['HLT_IsoMu24_eta2p1_v*', 'HLT_IsoMu27_v*', 'HLT_Mu45_eta2p1_v*', 'HLT_Mu50_v*'],
        'SingleElectron' : ['HLT_Ele105_CaloIdVT_GsfTrkIdT_v*'],#'HLT_Ele23_WPLoose_Gsf_v*', 'HLT_Ele27_eta2p1_WPLoose_Gsf_v*', 'HLT_Ele32_eta2p1_WPLoose_Gsf_v*', 'HLT_Ele23_CaloIdL_TrackIdL_IsoVL_v*', 
        'DoubleMu'       : ['HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_v*','HLT_Mu30_TkMu11_v*'],
        'DoubleElectron' : ['HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL_DZ_v*', 'HLT_DoubleEle33_CaloIdL_GsfTrkIdVL_v*'],
        'MET'            : ['HLT_PFMET120_NoiseCleaned_BTagCSV07_v*', 'HLT_PFHT350_PFMET120_NoiseCleaned_v*', 'HLT_PFMET170_NoiseCleaned_v*'],
        #'JET'            : ['HLT_PFJet260_v*'],
    },
#   processName='HLT',
#   outprefix='HLT'
    #setting 'unrollbits' to true will not only store the OR for each set of trigger bits but also the individual bits
    #caveat: this does not unroll the version numbers
    unrollbits=True
    )

filterAnalyzer= cfg.Analyzer(
    verbose=False,
    class_object=TriggerBitAnalyzer,
    triggerBits = {
        'FILTERS' : [ "Flag_HBHENoiseFilter", "Flag_CSCTightHaloFilter", "Flag_hcalLaserEventFilter", "Flag_EcalDeadCellTriggerPrimitiveFilter", "Flag_goodVertices", "Flag_trackingFailureFilter", "Flag_eeBadScFilter", "Flag_ecalLaserCorrFilter", "Flag_trkPOGFilters", "Flag_trkPOG_manystripclus53X", "Flag_trkPOG_toomanystripclus53X", "Flag_trkPOG_logErrorTooManyClusters", "Flag_METFilters" ],
    },
    processName = 'PAT',
    outprefix = 'Flag',
    #setting 'unrollbits' to true will not only store the OR for each set of trigger bits but also the individual bits
    #caveat: this does not unroll the version numbers
    unrollbits=True
    )


##############################
### JSONANALYZER         ###
##############################
from PhysicsTools.Heppy.analyzers.core.JSONAnalyzer import JSONAnalyzer
jsonAnalyzer = cfg.Analyzer(
    verbose=False,
    class_object=JSONAnalyzer,
    )

##############################
### PILEUPANALYZER         ###
##############################
from PhysicsTools.Heppy.analyzers.core.PileUpAnalyzer import PileUpAnalyzer
pileupAnalyzer = PileUpAnalyzer.defaultConfig

##############################
### VERTEXANALYZER         ###
##############################
from PhysicsTools.Heppy.analyzers.objects.VertexAnalyzer import VertexAnalyzer
vertexAnalyzer = cfg.Analyzer(
    verbose=False,
    class_object=VertexAnalyzer,
    vertexWeight = None,
    fixedWeight = 1,
    doHists = False,
    keepFailingEvents = True,
    )

##############################
### LEPTONANALYZER         ###
##############################
from PhysicsTools.Heppy.analyzers.objects.LeptonAnalyzer import LeptonAnalyzer
leptonAnalyzer = cfg.Analyzer(

    class_object                = LeptonAnalyzer,

    ### Lepton - General
    ##############################
    # energy scale corrections and ghost muon suppression (off by default)
    doMuScleFitCorrections=False, # "rereco"
    doRochesterCorrections=False,
    doElectronScaleCorrections=False, # "embedded" in 5.18 for regression
    doSegmentBasedMuonCleaning=False,
    # minimum deltaR between a loose electron and a loose muon (on overlaps, discard the electron)
    min_dr_electron_muon        = 0.02,
    # do MC matching
    do_mc_match                 = True, # note: it will in any case try it only on MC, not on data
    match_inclusiveLeptons      = False, # match to all inclusive leptons

    ### Electron - General
    ##############################
    electrons                   = 'slimmedElectrons',
    rhoElectron                 = 'fixedGridRhoFastjetAll',
    #ele_isoCorr                 = 'deltaBeta',
    ele_isoCorr                 = 'rhoArea',
    el_effectiveAreas           = 'Phys14_25ns_v1', #(can be 'Data2012' or 'Phys14_25ns_v1')
    ele_tightId                 = 'POG_Cuts_ID_PHYS14_25ns_v1_ConvVetoDxyDz_Veto',

    ### Electron selection - First step
    inclusive_electron_id       = '',
    #inclusive_electron_id       = 'POG_Cuts_ID_PHYS14_25ns_v1_Veto',
    inclusive_electron_pt       = 0.,
    inclusive_electron_eta      = 99.,
    inclusive_electron_dxy      = 1.e99,
    inclusive_electron_dz       = 1.e99,
    inclusive_electron_lostHits = 99.,
    #inclusive_electron_isoCut   = lambda electron : ( ( electron.isEB() and electron.relIso03 < 0.158721 ) or  ( electron.isEE() and electron.relIso03 < 0.177032 ) ) ,
    inclusive_electron_relIso   = 1.e99,

    ### Electron selection - Second step
    loose_electron_id           = 'POG_Cuts_ID_PHYS14_25ns_v1_ConvVetoDxyDz_Veto',
    loose_electron_pt           = 10,
    loose_electron_eta          = 2.5,
    loose_electron_dxy          = 1.e99,
    loose_electron_dz           = 1.e99,
    loose_electron_lostHits     = 9.0,
    loose_electron_isoCut       = lambda electron : ( ( electron.isEB() and electron.relIso03 < 0.158721 ) or  ( electron.isEE() and electron.relIso03 < 0.177032 ) ) ,
    loose_electron_relIso       = 1.e99,

    ### Muon - General
    ##############################
    muons                       = 'slimmedMuons',
    rhoMuon                     = 'fixedGridRhoFastjetAll',
    mu_isoCorr                  = 'deltaBeta' ,
    mu_effectiveAreas           = 'Phys14_25ns_v1', #(can be 'Data2012' or 'Phys14_25ns_v1')
    muon_dxydz_track            = 'muonBestTrack',

    ### Muon selection - First step
    inclusive_muon_id           = '',
    inclusive_muon_pt           = 0,
    inclusive_muon_eta          = 99,
    inclusive_muon_dxy          = 1.e99,
    inclusive_muon_dz           = 1.e99,
    #inclusive_muon_isoCut       = lambda muon : muon.relIso04 < 0.2,
    inclusive_muon_relIso       = 1.e99,

    ### Muon selection - Second step
    loose_muon_id               = 'POG_ID_Loose',
    loose_muon_pt               = 10,
    loose_muon_eta              = 2.4,
    loose_muon_dxy              = 1.e99,
    loose_muon_dz               = 1.e99,
    loose_muon_isoCut           = lambda muon : muon.relIso04 < 0.2,
    loose_muon_relIso           = 1.e99,

    # Mini-isolation, with pT dependent cone: will fill in the miniRelIso, miniRelIsoCharged, miniRelIsoNeutral variables of the leptons (see https://indico.cern.ch/event/368826/ )
    doMiniIsolation = True, # off by default since it requires access to all PFCandidates
    packedCandidates = 'packedPFCandidates',
    miniIsolationPUCorr = 'rhoArea', # Allowed options: 'rhoArea' (EAs for 03 cone scaled by R^2), 'deltaBeta', 'raw' (uncorrected), 'weights' (delta beta weights; not validated)
    miniIsolationVetoLeptons = None, # use 'inclusive' to veto inclusive leptons and their footprint in all isolation cones
    )

##############################
### JETANALYZER            ###
##############################
from PhysicsTools.Heppy.analyzers.objects.JetAnalyzer import JetAnalyzer
jetAnalyzer = cfg.Analyzer(

    class_object                = JetAnalyzer,

    ### Jet - General
    ##############################
    jetCol                      = 'slimmedJets',
    jetPt                       = 30.,
    jetEta                      = 4.7,
    jetEtaCentral               = 2.5,
    jetLepDR                    = 0.4,
    jetLepArbitration           = (lambda jet,lepton : jet), # you can decide which to keep in case of overlaps -> keeping the jet -> resolving it later
    minLepPt                    = 10,
    relaxJetId                  = False,
    doPuId                      = True,
    doQG                        = False,
    recalibrateJets             = False,
    shiftJEC                    = 0, # set to +1 or -1 to get +/-1 sigma shifts
    addJECShifts                = False,
    smearJets                   = False,
    shiftJER                    = 0, # set to +1 or -1 to get +/-1 sigma shifts
    cleanJetsFromFirstPhoton    = False,
    cleanJetsFromTaus           = False,
    cleanJetsFromIsoTracks      = False,
    recalibrationType           = 'AK4PFchs',
    jecPath                     = '',
    mcGT                        = '',
    dataGT                      = '',
    genJetCol                   = 'slimmedGenJets',
    rho                         = ('fixedGridRhoFastjetAll','',''),
    copyJetsByValue             = False, #Whether or not to copy the input jets or to work with references (should be 'True' if JetAnalyzer is run more than once)
    cleanSelectedLeptons        = False, #Whether to clean 'selectedLeptons' after disambiguation. Treat with care (= 'False') if running Jetanalyzer more than once
    lepSelCut                   = lambda lep : True,
    alwaysCleanPhotons          = False,
    cleanGenJetsFromPhoton      = False,
    collectionPostFix           = ''
    ### ====================== ###
    )

jetAnalyzerJERUp = copy.deepcopy(jetAnalyzer)
jetAnalyzerJERUp.shiftJER = +1
jetAnalyzerJERDown = copy.deepcopy(jetAnalyzer)
jetAnalyzerJERDown.shiftJER = -1

fatJetAnalyzer = cfg.Analyzer(

    class_object                = JetAnalyzer,

    ### Jet - General
    ##############################
    jetCol                      = 'slimmedJetsAK8',
    jetPt                       = 200.,
    jetEta                      = 4.7,
    jetEtaCentral               = 2.5,
    jetLepDR                    = 1.0,
    jetLepArbitration           = (lambda jet,lepton : jet), # you can decide which to keep in case of overlaps -> keeping the jet -> resolving it later
    minLepPt                    = 20,
    relaxJetId                  = False,
    doPuId                      = False, # Not commissioned in 7.0.X
    doQG                        = False,
    recalibrateJets             = False,
    shiftJEC                    = 0, # set to +1 or -1 to get +/-1 sigma shifts
    addJECShifts                = False,
    smearJets                   = True,
    shiftJER                    = 0, # set to +1 or -1 to get +/-1 sigma shifts
    cleanJetsFromFirstPhoton    = False,
    cleanJetsFromTaus           = False,
    cleanJetsFromIsoTracks      = False,
    recalibrationType           = 'AK8PFchs',
    jecPath                     = '%s/src/DMPD/Heppy/python/tools/JEC/' % os.environ['CMSSW_BASE'], 
    mcGT                        = 'Summer15_25nsV2_MC',
    dataGT                      = 'Summer15_25nsV2_DATA',
    genJetCol                   = 'slimmedGenJets',
    rho                         = ('fixedGridRhoFastjetAll','',''),
    copyJetsByValue             = False, #Whether or not to copy the input jets or to work with references (should be 'True' if JetAnalyzer is run more than once)
    cleanSelectedLeptons        = False, #Whether to clean 'selectedLeptons' after disambiguation. Treat with care (= 'False') if running Jetanalyzer more than once
    lepSelCut                   = lambda lep : True,
    alwaysCleanPhotons          = False,
    cleanGenJetsFromPhoton      = False,
    collectionPostFix           = 'AK8'
    ### ====================== ###
    )

fatJetAnalyzerJERUp = copy.deepcopy(fatJetAnalyzer)
fatJetAnalyzerJERUp.shiftJER = +1
fatJetAnalyzerJERDown = copy.deepcopy(fatJetAnalyzer)
fatJetAnalyzerJERDown.shiftJER = -1

##############################
### TAUANALYZER            ###
##############################
from PhysicsTools.Heppy.analyzers.objects.TauAnalyzer import TauAnalyzer
tauAnalyzer = cfg.Analyzer(

    class_object                = TauAnalyzer,

    ### Tau - General
    ##############################
    inclusive_ptMin = 18,
    inclusive_etaMax = 2.3,
    inclusive_dxyMax = 1000.,
    inclusive_dzMax = 1000, #0.4,
    inclusive_vetoLeptons = False,
    inclusive_leptonVetoDR = 0.4,
    #inclusive_decayModeID = "decayModeFindingNewDMs", # ignored if not set or ""
    #inclusive_tauID = "byLooseCombinedIsolationDeltaBetaCorr3Hits",
    inclusive_decayModeID = "decayModeFinding", # ignored if not set or ""
    inclusive_tauID = "byCombinedIsolationDeltaBetaCorrRaw3Hits",
    inclusive_tauIDnHits = 5,
    inclusive_vetoLeptonsPOG = False, # If True, the following two IDs are required
    inclusive_tauAntiMuonID = "",
    inclusive_tauAntiElectronID = "",
    # loose hadronic tau selection
    loose_ptMin = 18,
    loose_etaMax = 2.3,
    loose_dxyMax = 1000.,
    loose_dzMax = 1000, #0.2,
    loose_vetoLeptons = False,
    loose_leptonVetoDR = 0.4,
    #loose_decayModeID = "decayModeFinding", # ignored if not set or ""
    #loose_tauID = "byLooseCombinedIsolationDeltaBetaCorr3Hits",
    loose_decayModeID = "decayModeFinding", # ignored if not set or ""
    loose_tauID = "byCombinedIsolationDeltaBetaCorrRaw3Hits",
    loose_tauIDnHits = 5,
    loose_vetoLeptonsPOG = False, # If True, the following two IDs are required
    loose_tauAntiMuonID = "",
    loose_tauAntiElectronID = "",
    loose_tauLooseID = "decayModeFinding"
    ### ====================== ###
    )

    #if ( tau.tauID("byCombinedIsolationDeltaBetaCorrRaw3Hits") >= 5 ) continue;

##############################
### PHOTONANALYZER         ###
##############################
from PhysicsTools.Heppy.analyzers.objects.PhotonAnalyzer import PhotonAnalyzer
photonAnalyzer = cfg.Analyzer(

    class_object                = PhotonAnalyzer,

    ### Photon - General
    ##############################
    photons                     = 'slimmedPhotons',
    ptMin                       = 15,
    etaMax                      = 2.5,
    gammaID                     = 'POG_PHYS14_25ns_Loose',#'POG_PHYS14_25ns_Loose_hardcoded',
    gamma_isoCorr               = 'rhoArea',
    do_mc_match                 = True,
    do_randomCone               = False,
    rhoPhoton                   = 'fixedGridRhoFastjetAll',
    ### ====================== ###
    )

##############################
### METANALYZER            ###
##############################
from PhysicsTools.Heppy.analyzers.objects.METAnalyzer import METAnalyzer
MEtAnalyzer = cfg.Analyzer(

    class_object = METAnalyzer,
    
    ### MET - General
    ##############################
    metCollection     = "slimmedMETs",
    noPUMetCollection = "slimmedMETs",
    copyMETsByValue = False,
    recalibrate = False,
    jetAnalyzerCalibrationPostFix = "",
    doTkMet = False,
    doMetNoPU = False,  
    doMetNoMu = False,  
    doMetNoEle = False,  
    doMetNoPhoton = False,  
    candidates='packedPFCandidates',
    candidatesTypes='std::vector<pat::PackedCandidate>',
    dzMax = 0.1,
    collectionPostFix = "",
    ### ====================== ###
    )

from DMPD.Heppy.analyzers.METNoHFAnalyzer import METNoHFAnalyzer
METNoHFAnalyzer = cfg.Analyzer(

    class_object = METNoHFAnalyzer,
    
    ### MET - General
    ##############################
    recalibrate = False,
    jetAnalyzerCalibrationPostFix = "",
    candidates='packedPFCandidates',
    candidatesTypes='std::vector<pat::PackedCandidate>',
    collectionPostFix = "",
    ### ====================== ###
    )

##############################
### ANALYSIS ANALYZERS     ###
##############################

fake_met_cut = 0

from DMPD.Heppy.analyzers.GenAnalyzer import GenAnalyzer
GenAnalyzer = cfg.Analyzer(
    class_object = GenAnalyzer,
    phi = [9100000, 9900032, 9000001, 9000002, -9000002, 1023],
    chi = [9100022, -9100022, 9100012],
    )

from DMPD.Heppy.analyzers.PreselectionAnalyzer import PreselectionAnalyzer
PreselectionAnalyzer = cfg.Analyzer(
    verbose = False,
    class_object = PreselectionAnalyzer,
    recalibrateMass             = True,
    recalibrationType           = 'AK8PFchs',
    jecPath                     = '%s/src/DMPD/Heppy/python/tools/JEC/' % os.environ['CMSSW_BASE'], 
    #jecPath                     = 'dcap://t2-srm-02.lnl.infn.it/pnfs/lnl.infn.it/data/cms/store/user/zucchett/JEC/', 
    mcGT                        = 'Summer15_25nsV2_MC',
    dataGT                      = 'Summer15_25nsV2_DATA',
    )

from DMPD.Heppy.analyzers.XCleaningAnalyzer import XCleaningAnalyzer
XCleaningAnalyzer = cfg.Analyzer(
    verbose=False,
    class_object = XCleaningAnalyzer,
    cleanTaus = True,
    cleanJets = True,
    cleanJetsAK8 = False,
    cleanFromMuons = True,
    cleanFromElectrons = False,
    mu_clean_pt  = 20.,
    mu_clean_id  = 'POG_ID_Tight',
    mu_clean_iso = lambda x : x.relIso04 < 0.12,
    mu_tau_dr    = 0.4,
    mu_jet_dr    = 0.4,
    mu_fatjet_dr = 0.4,
    ele_clean_pt = 20.,
    ele_clean_id = 'POG_Cuts_ID_PHYS14_25ns_v1_ConvVetoDxyDz_Veto',
    ele_clean_iso= lambda x : ( ( x.isEB() and x.relIso03 <  0.069537 ) or  ( x.isEE() and x.relIso03 < 0.078265 ) ),
    ele_tau_dr   = 0.4,
    ele_jet_dr   = 0.4,
    ele_fatjet_dr= 0.4,
    )

from DMPD.Heppy.analyzers.SyncAnalyzer import SyncAnalyzer
SyncAnalyzer = cfg.Analyzer(
    verbose = False,
    class_object = SyncAnalyzer,
    )

from DMPD.Heppy.analyzers.SRAnalyzer import SRAnalyzer
SRAnalyzer = cfg.Analyzer(
    verbose = False,
    class_object = SRAnalyzer,
    jetAlgo = "ak8PFJetsCHSPrunedMass",
    met_pt = 0.,#200.,
    )

from DMPD.Heppy.analyzers.XZhAnalyzer import XZhAnalyzer
XZhAnalyzer = cfg.Analyzer(
    verbose = False,
    class_object = XZhAnalyzer,
    elec1pt = 115.,
    elec2pt = 35.,
    muon1pt = 50.,
    muon2pt = 20.,
    fatjet_pt = 200.,
    jetlep_dR = 0.8,
    Z_mass_low = 70.,
    Z_mass_high = 110.,
    Z_pt = 0.,#200.,
    jetAlgo = "ak8PFJetsCHSPrunedMass",#"ak8PFJetsCHSSoftDropMass"
    )


#from DMPD.Heppy.analyzers.CategorizationAnalyzer import CategorizationAnalyzer
#CategorizationAnalyzer = cfg.Analyzer(
#    verbose = False,
#    class_object = CategorizationAnalyzer,
#    jet1_pt = 30.,
#    jet1_eta = 2.5,
#    jet1_tag = -1e99,
#    jet1_chf_min = 0.2,
#    jet1_nhf_max = 0.7,
#    jet1_phf_max = 0.7,
#    jet2_pt = 30.,
#    jet2_eta = 2.5,
#    jet2_tag = -1e99,
#    deltaPhi12 = 2.5,
#    enableFatJets = True,
#    fatjet_pt = 250.,
#    fatjet_tag1 = 0.423,
#    fatjet_tag2 = 0.423,
#    fatjet_mass = 50.,
#    fatjet_mass_algo = 'ak8PFJetsCHSSoftDropMass',
#    fatjet_tau21 = -1.,
#    jetveto_pt = 0.,
#    jetveto_eta = 2.5,
#    )


from DMPD.Heppy.analyzers.SyncAnalyzerSR import SyncAnalyzerSR
SyncAnalyzerSR = cfg.Analyzer(
    verbose = False,
    class_object = SyncAnalyzerSR,
    )

from DMPD.Heppy.analyzers.SyncAnalyzerGCR import SyncAnalyzerGCR
SyncAnalyzerGCR = cfg.Analyzer(
    verbose = False,
    class_object = SyncAnalyzerGCR,
    )

from DMPD.Heppy.analyzers.SyncAnalyzerZCR import SyncAnalyzerZCR
SyncAnalyzerZCR = cfg.Analyzer(
    verbose = False,
    class_object = SyncAnalyzerZCR,
    )

from DMPD.Heppy.analyzers.SyncAnalyzerWCR import SyncAnalyzerWCR
SyncAnalyzerWCR = cfg.Analyzer(
    verbose = False,
    class_object = SyncAnalyzerWCR,
    )


globalEventVariables = [
    #NTupleVariable('isSR',      lambda x: x.isSR, int, help='Signal Region flag'),
    #NTupleVariable('isZCR',     lambda x: x.isZCR, int, help='Z+jets Control Region flag'),
    #NTupleVariable('isWCR',     lambda x: x.isWCR, int, help='W+jets Control Region flag'),
    #NTupleVariable('isTCR',     lambda x: x.isTCR, int, help='ttbar Control Region flag'),
    #NTupleVariable('isGCR',     lambda x: x.isGCR, int, help='Gamma+jets Control Region flag'),
    #NTupleVariable('Cat',       lambda x: x.Category, int, help='Category 1/2/3'),
    NTupleVariable('lheNb',            lambda x: getattr(x, "lheNb", -1.), int, help='Number of b-quarks at LHE level'),
    NTupleVariable('lheHT',            lambda x: getattr(x, "lheHT", -1.), int, help='HT at LHE level'),
    NTupleVariable('lheVpt',           lambda x: getattr(x, "lheV_pt", -1.), int, help='Vector boson pt at LHE level'),
    NTupleVariable('genNb',            lambda x: len(x.genbquarks) if hasattr(x, "genbquarks") else -1, int, help='Number of b-quarks at generator level'),
    NTupleVariable('rho',              lambda x: getattr(x, "rho", -1.), int, help='Energy density in the event'),
    NTupleVariable('nPV',              lambda x: len(x.vertices), int, help='Number of reconstructed primary vertices'),
]
globalDMVariables = globalEventVariables + [
    NTupleVariable('nMuons',           lambda x: len(x.selectedMuons), int, help='Number of selected muons'),
    NTupleVariable('nElectrons',       lambda x: len(x.selectedElectrons), int, help='Number of selected electrons'),
    NTupleVariable('nTaus',            lambda x: len(x.xcleanTaus), int, help='Number of xcleaned taus'),
    NTupleVariable('nPhotons',         lambda x: len(x.xcleanPhotons), int, help='Number of selected photons'),
    NTupleVariable('nJets',            lambda x: len(x.xcleanJets), int, help='Number of xcleaned jets'),
    NTupleVariable('nFatJets',         lambda x: len(x.xcleanJetsAK8), int, help='Number of xcleaned fat jets'),
    NTupleVariable('nBJets',           lambda x: len([jet for jet in x.xcleanJets if abs(jet.hadronFlavour()) == 5]), int, help='Number of xcleaned b-jets'),
]


##############################
### SIGNAL REGION TREE     ###
##############################
SignalRegionTreeProducer= cfg.Analyzer(
    class_object=AutoFillTreeProducer,
    name='SignalRegionTreeProducer',
    treename='SR',
    filter = lambda x: x.isSR and x.met.pt() >= fake_met_cut,
    verbose=False,
    vectorTree = False,
    globalVariables = globalDMVariables + [
        NTupleVariable('isZtoNN',  lambda x: x.isZ2NN, int, help='Z -> nu nu flag'),
    ],
    globalObjects = {
        'theX'      : NTupleObject('X', candidateFullType, help='Heavy resonance candidate'),
        'met'       : NTupleObject('met',  metFullType, help='PF MET without type 1 corrections'),
        'pfmet'     : NTupleObject('pfmet',  metType, help='PF MET, after default type 1 corrections'),
        #'tkMetPVchs': NTupleObject('met_tk',  metType, help='Tracker MET'),
        #'V'         : NTupleObject('V', candidateType, help='Boson candidate'),
        #'A'         : NTupleObject('A', candidateFullType, help='Resonance candidate'),
    },
    collections = {
        #'xcleanTaus'          : NTupleCollection('tau', tauType, 1, help='cleaned Tau collection'),
        #'xcleanPhotons'       : NTupleCollection('photon', photonType, 1, help='cleaned Photon collection'),
        'xcleanJets'          : NTupleCollection('jet', jetType, 3, help='cleaned Jet collection'),
#        'xcleanJetsJERUp'     : NTupleCollection('jetJERUp', lorentzVectorType, 3, help='cleaned Jet collection with JER +1 sigma'),
#        'xcleanJetsJERDown'   : NTupleCollection('jetJERDown', lorentzVectorType, 3, help='cleaned Jet collection with JER -1 sigma'),
        'xcleanJetsAK8'       : NTupleCollection('fatjet', fatjetType, 1, help='cleaned fatJet collection'),
#        'xcleanJetsAK8JERUp'  : NTupleCollection('fatjetJERUp', lorentzVectorType, 1, help='cleaned fatJet collection with JER +1 sigma'),
#        'xcleanJetsAK8JERDown': NTupleCollection('fatjetJERDown', lorentzVectorType, 1, help='cleaned fatJet collection with JER -1 sigma'),
    }
)


##############################
### Z CONTROL REGION TREE  ###
##############################
ZControlRegionTreeProducer= cfg.Analyzer(
    class_object=AutoFillTreeProducer,
    name='ZControlRegionTreeProducer',
    treename='ZCR',
    filter = lambda x: x.isZCR and x.fakemet.pt() >= fake_met_cut,
    verbose=False,
    vectorTree = False,
    globalVariables = globalDMVariables + [
        NTupleVariable('isZtoEE',  lambda x: x.isZtoEE, int, help='Z -> mu mu flag'),
        NTupleVariable('isZtoMM',  lambda x: x.isZtoMM, int, help='Z -> e e flag'),
    ],
    globalObjects = {
        'theZ'      : NTupleObject('Z', candidateType, help='Z boson candidate'),
        'met'       : NTupleObject('met',  metFullType, help='PF MET without type 1 corrections'),
        'pfmet'     : NTupleObject('pfmet',  metType, help='PF MET, after default type 1 corrections'),
        #'tkMetPVchs': NTupleObject('met_tk',  metType, help='Tracker MET'),
        'fakemet'   : NTupleObject('fakemet', metType, help='fake MET in Z events obtained removing the leptons'),
        #'V'         : NTupleObject('V', candidateType, help='Higgs boson candidate'),
        #'A'         : NTupleObject('A', candidateFullType, help='Resonance candidate'),
    },
    collections = {
        'xcleanLeptons'       : NTupleCollection('lepton', leptonType, 2, help='Muon or Electron collection'),
        #'xcleanTaus'          : NTupleCollection('tau', tauType, 1, help='cleaned Tau collection'),
        #'xcleanPhotons'       : NTupleCollection('photon', photonType, 1, help='cleaned Photon collection'),
        'xcleanJets'          : NTupleCollection('jet', jetType, 3, help='cleaned Jet collection'),
#        'xcleanJetsJERUp'     : NTupleCollection('jetJERUp', lorentzVectorType, 3, help='cleaned Jet collection with JER +1 sigma'),
#        'xcleanJetsJERDown'   : NTupleCollection('jetJERDown', lorentzVectorType, 3, help='cleaned Jet collection with JER -1 sigma'),
        'xcleanJetsAK8'       : NTupleCollection('fatjet', fatjetType, 1, help='cleaned fatJet collection'),
#        'xcleanJetsAK8JERUp'  : NTupleCollection('fatjetJERUp', lorentzVectorType, 1, help='cleaned fatJet collection with JER +1 sigma'),
#        'xcleanJetsAK8JERDown': NTupleCollection('fatjetJERDown', lorentzVectorType, 1, help='cleaned fatJet collection with JER -1 sigma'),
    }
)


##############################
### W CONTROL REGION TREE  ###
##############################
WControlRegionTreeProducer= cfg.Analyzer(
    class_object=AutoFillTreeProducer,
    name='WControlRegionTreeProducer',
    treename='WCR',
    filter = lambda x: x.isWCR and x.fakemet.pt() >= fake_met_cut,
    verbose=False,
    vectorTree = False,
    globalVariables = globalDMVariables + [
        NTupleVariable('isWtoEN',  lambda x: x.isWtoEN, int, help='W -> mu nu flag'),
        NTupleVariable('isWtoMN',  lambda x: x.isWtoMN, int, help='W -> e nu flag'),
    ],
    globalObjects = {
        'theW'      : NTupleObject('W', candidateType, help='W boson candidate'),
        'met'       : NTupleObject('met',  metFullType, help='PF MET without type 1 corrections'),
        'pfmet'     : NTupleObject('pfmet',  metType, help='PF MET, after default type 1 corrections'),
        #'tkMetPVchs': NTupleObject('met_tk',  metType, help='Tracker MET'),
        'fakemet'   : NTupleObject('fakemet', metType, help='fake MET in W -> mu nu event obtained removing the lepton'),
        #'V'         : NTupleObject('V', candidateType, help='Higgs boson candidate'),
        #'A'         : NTupleObject('A', candidateFullType, help='Resonance candidate'),
    },
    collections = {
        'xcleanLeptons'       : NTupleCollection('lepton', leptonType, 4, help='Muon or Electron collection'),
        #'xcleanTaus'          : NTupleCollection('tau', tauType, 1, help='cleaned Tau collection'),
        #'xcleanPhotons'       : NTupleCollection('photon', photonType, 1, help='cleaned Photon collection'),
        'xcleanJets'          : NTupleCollection('jet', jetType, 4, help='cleaned Jet collection'),
#        'xcleanJetsJERUp'     : NTupleCollection('jetJERUp', lorentzVectorType, 3, help='cleaned Jet collection with JER +1 sigma'),
#        'xcleanJetsJERDown'   : NTupleCollection('jetJERDown', lorentzVectorType, 3, help='cleaned Jet collection with JER -1 sigma'),
        'xcleanJetsAK8'       : NTupleCollection('fatjet', fatjetType, 1, help='cleaned fatJet collection'),
#        'xcleanJetsAK8JERUp'  : NTupleCollection('fatjetJERUp', lorentzVectorType, 1, help='cleaned fatJet collection with JER +1 sigma'),
#        'xcleanJetsAK8JERDown': NTupleCollection('fatjetJERDown', lorentzVectorType, 1, help='cleaned fatJet collection with JER -1 sigma'),
    }
)


##################################
### TTbar CONTROL REGION TREE  ###
##################################
TTbarControlRegionTreeProducer= cfg.Analyzer(
    class_object=AutoFillTreeProducer,
    name='TTbarControlRegionTreeProducer',
    treename='TCR',
    filter = lambda x: x.isTCR and x.fakemet.pt() >= fake_met_cut,
    verbose=False,
    vectorTree = False,
    globalVariables = globalDMVariables + [],
    globalObjects = {
        'met'       : NTupleObject('met',  metFullType, help='PF MET without type 1 corrections'),
        'pfmet'     : NTupleObject('pfmet',  metType, help='PF MET, after default type 1 corrections'),
        #'tkMetPVchs': NTupleObject('met_tk',  metType, help='Tracker MET'),
        'fakemet'   : NTupleObject('fakemet', metType, help='fake MET in ttbar events obtained removing the leptons'),
        #'V'         : NTupleObject('V', candidateType, help='Higgs boson candidate'),
    },
    collections = {
        'xcleanLeptons'       : NTupleCollection('lepton', leptonType, 2, help='Muon and Electron collection'),
        #'xcleanTaus'          : NTupleCollection('tau', tauType, 1, help='cleaned Tau collection'),
        #'xcleanPhotons'       : NTupleCollection('photon', photonType, 1, help='cleaned Photon collection'),
        'xcleanJets'          : NTupleCollection('jet', jetType, 3, help='cleaned Jet collection'),
#        'xcleanJetsJERUp'     : NTupleCollection('jetJERUp', lorentzVectorType, 3, help='cleaned Jet collection with JER +1 sigma'),
#        'xcleanJetsJERDown'   : NTupleCollection('jetJERDown', lorentzVectorType, 3, help='cleaned Jet collection with JER -1 sigma'),
        'xcleanJetsAK8'       : NTupleCollection('fatjet', fatjetType, 1, help='cleaned fatJet collection'),
#        'xcleanJetsAK8JERUp'  : NTupleCollection('fatjetJERUp', lorentzVectorType, 1, help='cleaned fatJet collection with JER +1 sigma'),
#        'xcleanJetsAK8JERDown': NTupleCollection('fatjetJERDown', lorentzVectorType, 1, help='cleaned fatJet collection with JER -1 sigma'),
    }
)

##############################
### G CONTROL REGION TREE  ###
##############################
GammaControlRegionTreeProducer= cfg.Analyzer(
    class_object=AutoFillTreeProducer,
    name='GammaControlRegionTreeProducer',
    treename='GCR',
    filter = lambda x: x.isGCR and x.fakemet.pt() >= fake_met_cut,
    verbose=False,
    vectorTree = False,
    globalVariables = globalDMVariables + [],
    globalObjects = {
        'met'       : NTupleObject('met',  metFullType, help='PF MET without type 1 corrections'),
        'pfmet'     : NTupleObject('pfmet',  metType, help='PF MET, after default type 1 corrections'),
        #'tkMetPVchs': NTupleObject('met_tk',  metType, help='Tracker MET'),
        'fakemet'   : NTupleObject('fakemet', metType, help='fake MET in gamma + jets event obtained removing the photon'),
        #'V' : NTupleObject('V', candidateType, help='Higgs boson candidate'),
    },
    collections = {
        #'selectedMuons'       : NTupleCollection('muon', muonType, 4, help='Muons after the preselection'),
        #'selectedElectrons'   : NTupleCollection('electron', electronType, 4, help='Electrons after the preselection'),
        #'xcleanTaus'          : NTupleCollection('tau', tauType, 1, help='cleaned Tau collection'),
        'xcleanPhotons'       : NTupleCollection('photon', photonType, 1, help='cleaned Photon collection'),
        'xcleanJets'          : NTupleCollection('jet', jetType, 3, help='cleaned Jet collection'),
#        'xcleanJetsJERUp'     : NTupleCollection('jetJERUp', lorentzVectorType, 3, help='cleaned Jet collection with JER +1 sigma'),
#        'xcleanJetsJERDown'   : NTupleCollection('jetJERDown', lorentzVectorType, 3, help='cleaned Jet collection with JER -1 sigma'),
        'xcleanJetsAK8'       : NTupleCollection('fatjet', fatjetType, 1, help='cleaned fatJet collection'),
#        'xcleanJetsAK8JERUp'  : NTupleCollection('fatjetJERUp', lorentzVectorType, 1, help='cleaned fatJet collection with JER +1 sigma'),
#        'xcleanJetsAK8JERDown': NTupleCollection('fatjetJERDown', lorentzVectorType, 1, help='cleaned fatJet collection with JER -1 sigma'),
    }
)


##############################
###    X -> Zh -> llbb     ###
##############################

XZhTreeProducer= cfg.Analyzer(
    class_object=AutoFillTreeProducer,
    name='XZhTreeProducer',
    treename='XZh',
    filter = lambda x: x.isXZh,
    verbose=False,
    vectorTree = False,
    globalVariables = globalEventVariables + [
        NTupleVariable('isZtoEE',  lambda x: x.isZ2EE, int, help='Z -> ee flag'),
        NTupleVariable('isZtoMM',  lambda x: x.isZ2MM, int, help='Z -> mumu flag'),
        NTupleVariable('isGenZtoEE',  lambda x: x.isGenZ2EE, int, help='Z -> ee at gen level flag'),
        NTupleVariable('isGenZtoMM',  lambda x: x.isGenZ2MM, int, help='Z -> mumu at gen level flag'),
        NTupleVariable('nMuons',           lambda x: len(x.highptIdIsoMuons), int, help='Number of selected muons'),
        NTupleVariable('nElectrons',       lambda x: len(x.highptIdIsoElectrons), int, help='Number of selected electrons'),
        NTupleVariable('nTaus',            lambda x: len(x.selectedTaus), int, help='Number of xcleaned taus'),
        NTupleVariable('nPhotons',         lambda x: len(x.selectedPhotons), int, help='Number of selected photons'),
        NTupleVariable('nJets',            lambda x: len(x.cleanJets), int, help='Number of xcleaned jets'),
        NTupleVariable('nFatJets',         lambda x: len(x.highptFatJets), int, help='Number of xcleaned fat jets'),
    ],
    globalObjects = {
        'X'         : NTupleObject('X', candidateFullType, help='Heavy resonance candidate'),
        'Z'         : NTupleObject('Z', candidateType, help='Z boson candidate'),
        'met'       : NTupleObject('met',  metType, help='PF MET, after default type 1 corrections'),
        },
    collections = {
        'highptLeptons' : NTupleCollection('lepton', leptonType, 2, help='Muons and Electrons after the preselection'),
        'highptFatJets' : NTupleCollection('fatjet', fatjetType, 1, help='fatJets after the preselection'),
        }
    )

##############################
### SEQUENCE               ###
##############################

sequence = [
    jsonAnalyzer, 
    lheAnalyzer,
    generatorAnalyzer,
    #pdfAnalyzer,
    triggerAnalyzer,
    filterAnalyzer,
    pileupAnalyzer,
    vertexAnalyzer,
    MEtAnalyzer,
    #METNoHFAnalyzer,
    photonAnalyzer,
    leptonAnalyzer,
    tauAnalyzer,
    jetAnalyzer,
    #jetAnalyzerJERUp,
    #jetAnalyzerJERDown,
    fatJetAnalyzer,
    #fatJetAnalyzerJERUp,
    #fatJetAnalyzerJERDown,
    GenAnalyzer,
    #### Preselection Analyzers
#    SyncAnalyzerSR,
#    SyncAnalyzerGCR,
#    SyncAnalyzerZCR,
#    SyncAnalyzerWCR,
    PreselectionAnalyzer,
    ##### Analysis Analyzers
#    ZeroLeptonAnalyzer,
#    ZeroLeptonGammaAnalyzer,
#    OneLeptonAnalyzer,
#    TwoLeptonOSSFAnalyzer,
#    TwoLeptonOSDFAnalyzer,
    ##### Categorization Analyzers
    XCleaningAnalyzer,
    SyncAnalyzer,
#    CategorizationAnalyzer,
    SRAnalyzer,
    XZhAnalyzer,
    ##### Tree producers
    SignalRegionTreeProducer,
    ZControlRegionTreeProducer,
    WControlRegionTreeProducer,
    TTbarControlRegionTreeProducer,
    GammaControlRegionTreeProducer,
    XZhTreeProducer,
    ]

##############################
### TFILESERVICE           ###
##############################
from PhysicsTools.HeppyCore.framework.services.tfile import TFileService
output_service = cfg.Service(
    TFileService,
    'outputfile',
    name='outputfile',
    fname='tree.root',
    option='recreate'
    )

##############################
### INPUT                  ###
##############################
from PhysicsTools.Heppy.utils.miniAodFiles import miniAodFiles
#from DMPD.Heppy.samples.Phys14.fileLists import samples
from DMPD.Heppy.samples.Spring15.fileLists import mcsamples
from DMPD.Heppy.samples.Data.fileLists import datasamples

maxlsftime   = 7.5  # in hours
eventspersec = 5 # in ev/s

sample = {}
for i in datasamples:
    sample[i] = cfg.Component(
        files   = datasamples[i]['files'],
        name    = i,
        #json    = '%s/src/DMPD/Heppy/python/tools/JSON/Cert_246908-251883_13TeV_PromptReco_Collisions15_JSON_v2.txt' % os.environ['CMSSW_BASE'],
        json    = '%s/src/DMPD/Heppy/python/tools/JSON/Cert_246908-256869_13TeV_PromptReco_Collisions15_25ns_JSON.txt' % os.environ['CMSSW_BASE'],
        splitFactor = int(datasamples[i]['nevents']/(maxlsftime*3600*eventspersec)),
    )

for i in mcsamples:
    sample[i] = cfg.MCComponent(
        files   = mcsamples[i]['files'],
        name    = i,
	    isMC    = True,
	    isEmbed = False,
	    splitFactor = int(mcsamples[i]['nevents']/(maxlsftime*3600*eventspersec)),
    )

##############################
### FWLITE                 ###
##############################
#from PhysicsTools.Heppy.utils.cmsswPreprocessor import CmsswPreprocessor
#preprocessor = CmsswPreprocessor('tagFatJets.py')
#preprocessor = CmsswPreprocessor('test.py')

from PhysicsTools.HeppyCore.framework.eventsfwlite import Events

### MC ###
#selectedComponents = [
   #sample['DYJetsToLL_M50_amcatnloFXFX_pythia8_v3'],
##    sample['DYJetsToLL_M50_madgraphMLM_pythia8_v1'],
##    sample['DYJetsToLL_M50_HT100to200_madgraphMLM_pythia8_v2'],
##    sample['DYJetsToLL_M50_HT200to400_madgraphMLM_pythia8_v2'],
##    sample['DYJetsToLL_M50_HT400to600_madgraphMLM_pythia8_v2'],
##    sample['DYJetsToLL_M50_HT600toInf_madgraphMLM_pythia8_v2'],
   #sample['ZJetsToNuNu_HT100to200_madgraphMLM_pythia8_v1'],
   #sample['ZJetsToNuNu_HT200to400_madgraphMLM_pythia8_v1'],
   #sample['ZJetsToNuNu_HT400to600_madgraphMLM_pythia8_v1'],
   #sample['ZJetsToNuNu_HT600toInf_madgraphMLM_pythia8_v1'],
###    sample['GJets_HT_100To200_madgraphMLM_pythia8_v2'],
###    sample['GJets_HT_200To400_madgraphMLM_pythia8_v2'],
###    sample['GJets_HT_400To600_madgraphMLM_pythia8_v1'],
###    sample['GJets_HT_600ToInf_madgraphMLM_pythia8_v1'],
##    sample['QCD_HT_1000to1500_madgraphMLM_pythia8_v2'],
##    sample['QCD_HT_100to200_madgraphMLM_pythia8_v2'],
##    sample['QCD_HT_1500to2000_madgraphMLM_pythia8_v1'],
##    sample['QCD_HT_2000toInf_madgraphMLM_pythia8_v1'],
##    sample['QCD_HT_200to300_madgraphMLM_pythia8_v2'],
##    sample['QCD_HT_300to500_madgraphMLM_pythia8_v2'],
##    sample['QCD_HT_500to700_madgraphMLM_pythia8_v1'],
##    sample['QCD_HT_700to1000_madgraphMLM_pythia8_v1'],
###    sample['QCD_Pt_1000to1400_pythia8_v1'],
###    sample['QCD_Pt_10to15_pythia8_v2'],
###    sample['QCD_Pt_120to170_pythia8_v1'],
###    sample['QCD_Pt_1400to1800_pythia8_v1'],
###    sample['QCD_Pt_15to30_pythia8_v2'],
###    sample['QCD_Pt_170to300_pythia8_v2'],
###    sample['QCD_Pt_1800to2400_pythia8_v1'],
###    sample['QCD_Pt_2400to3200_pythia8_v1'],
###    sample['QCD_Pt_300to470_pythia8_v1'],
###    sample['QCD_Pt_30to50_pythia8_v2'],
###    sample['QCD_Pt_3200toInf_pythia8_v1'],
###    sample['QCD_Pt_470to600_pythia8_v2'],
###    sample['QCD_Pt_50to80_pythia8_v2'],
###    sample['QCD_Pt_5to10_pythia8_v2'],
###    sample['QCD_Pt_600to800_pythia8_v3'],
###    sample['QCD_Pt_800to1000_pythia8_v2'],
###    sample['QCD_Pt_80to120_pythia8_v1'],
##    sample['ST_s_channel_4f_leptonDecays_amcatnlo_pythia8_v1'],
##    sample['ST_t_channel_antitop_4f_leptonDecays_amcatnlo_pythia8_v1'],
##    sample['ST_t_channel_top_4f_leptonDecays_amcatnlo_pythia8_v1'],
##    sample['ST_tW_antitop_5f_inclusiveDecays_powheg_pythia8_v1'],
##    sample['ST_tW_top_5f_inclusiveDecays_powheg_pythia8_v1'],
###    sample['TTJets_madgraphMLM_pythia8_v2'],
   #sample['TT_powheg_pythia8_v2'],
   #sample['WJetsToLNu_amcatnloFXFX_pythia8_v1'],
###    sample['WJetsToLNu_HT_100To200_madgraphMLM_pythia8_v1'],
###    sample['WJetsToLNu_HT_200To400_madgraphMLM_pythia8_v1'],
###    sample['WJetsToLNu_HT_400To600_madgraphMLM_pythia8_v3'],
###    sample['WJetsToLNu_HT_600ToInf_madgraphMLM_pythia8_v1'],
   #sample['WW_pythia8_v1'],
   #sample['WZ_pythia8_v1'],
   #sample['ZZ_pythia8_v3'],
###    sample['ZH_HToBB_ZToLL_M125_amcatnloFXFX_madspin_pythia8_v1'],
##    sample['ZH_HToBB_ZToLL_M125_powheg_pythia8_v1'],
###    sample['ZH_HToBB_ZToNuNu_M125_amcatnloFXFX_madspin_pythia8_v1'],
##    sample['ZprimeToZhToZlephbb_narrow_M600_madgraph_v1'],
##    sample['ZprimeToZhToZlephbb_narrow_M800_madgraph_v1'],
##    sample['ZprimeToZhToZlephbb_narrow_M1000_madgraph_v1'],
##    sample['ZprimeToZhToZlephbb_narrow_M1200_madgraph_v1'],
##    sample['ZprimeToZhToZlephbb_narrow_M1400_madgraph_v1'],
##    sample['ZprimeToZhToZlephbb_narrow_M1600_madgraph_v1'],
##    sample['ZprimeToZhToZlephbb_narrow_M1800_madgraph_v1'],
##    sample['ZprimeToZhToZlephbb_narrow_M2000_madgraph_v1'],
##    sample['ZprimeToZhToZlephbb_narrow_M2500_madgraph_v1'],
##    sample['ZprimeToZhToZlephbb_narrow_M3000_madgraph_v1'],
##    sample['ZprimeToZhToZlephbb_narrow_M3500_madgraph_v1'],
##    sample['ZprimeToZhToZlephbb_narrow_M4000_madgraph_v1'],
##    sample['ZprimeToZhToZlephbb_narrow_M4500_madgraph_v1'],
##    sample['ZprimeToZhToZinvhbb_narrow_M600_madgraph_v1'],
##    sample['ZprimeToZhToZinvhbb_narrow_M800_madgraph_v2'],
##    sample['ZprimeToZhToZinvhbb_narrow_M1000_madgraph_v1'],
##    sample['ZprimeToZhToZinvhbb_narrow_M1200_madgraph_v1'],
##    sample['ZprimeToZhToZinvhbb_narrow_M1400_madgraph_v1'],
##    sample['ZprimeToZhToZinvhbb_narrow_M1600_madgraph_v1'],
##    sample['ZprimeToZhToZinvhbb_narrow_M1800_madgraph_v1'],
##    sample['ZprimeToZhToZinvhbb_narrow_M2000_madgraph_v1'],
##    sample['ZprimeToZhToZinvhbb_narrow_M2500_madgraph_v1'],
##    sample['ZprimeToZhToZinvhbb_narrow_M3000_madgraph_v1'],
##    sample['ZprimeToZhToZinvhbb_narrow_M3500_madgraph_v1'],
##    sample['ZprimeToZhToZinvhbb_narrow_M4000_madgraph_v1'],
##    sample['ZprimeToZhToZinvhbb_narrow_M4500_madgraph_v1'],
#]



### DATA ###
selectedComponents = [
#### Run2015D
   sample['SingleMuon_Run2015D_PromptReco_v3'],
   #sample['SingleElectron_Run2015D_PromptReco_v3'],
   #sample['DoubleMuon_Run2015D_PromptReco_v3'],
   #sample['DoubleEG_Run2015D_PromptReco_v3'],
   #sample['MET_Run2015D_PromptReco_v3'],
#### Run2015C
   #sample['SingleMuon_Run2015C_PromptReco_v1'],
   #sample['SingleElectron_Run2015C_PromptReco_v1'],
   #sample['DoubleMuon_Run2015C_PromptReco_v1'],
   #sample['DoubleEG_Run2015C_PromptReco_v1'],
   #sample['MET_Run2015C_PromptReco_v1'],
#### Run2015B
   #sample['SingleMuon_Run2015B_PromptReco_v1'],
   #sample['SingleElectron_Run2015B_PromptReco_v1'],
   #sample['DoubleMuon_Run2015B_PromptReco_v1'],
   #sample['DoubleEG_Run2015B_PromptReco_v1'],
   #sample['MET_Run2015B_PromptReco_v1'],
   #sample['SingleMuon_Run2015B_17Jul2015_v1'],
   #sample['SingleElectron_Run2015B_17Jul2015_v1'],
   #sample['DoubleMuon_Run2015B_17Jul2015_v1'],
   #sample['DoubleEG_Run2015B_17Jul2015_v1'],
   #sample['MET_Run2015B_17Jul2015_v1'],
]
filterAnalyzer.processName = 'RECO'


#selectedComponents = [sample['ZprimeToZhToZlephbb_narrow_M2000_madgraph_v1'],]
#selectedComponents = [sample['BBbarDMJets_pseudoscalar_Mchi-10_Mphi-100_madgraphMLM_pythia8_v1'],]

#selectedComponents = [
####    sample['DYJetsToLL_M50_madgraphMLM_pythia8_v1'],
   ###sample['ZprimeToZhToZlephbb_narrow_M1000_madgraph_v1'],
   ###sample['ZprimeToZhToZlephbb_narrow_M1200_madgraph_v1'],
   ###sample['ZprimeToZhToZlephbb_narrow_M1400_madgraph_v1'],
   ###sample['ZprimeToZhToZlephbb_narrow_M1600_madgraph_v1'],
   ###sample['ZprimeToZhToZlephbb_narrow_M1800_madgraph_v1'],
   ###sample['ZprimeToZhToZlephbb_narrow_M2000_madgraph_v1'],
   ###sample['ZprimeToZhToZlephbb_narrow_M2500_madgraph_v1'],
   ###sample['ZprimeToZhToZlephbb_narrow_M3000_madgraph_v1'],
   ###sample['ZprimeToZhToZlephbb_narrow_M3500_madgraph_v1'],
   ###sample['ZprimeToZhToZlephbb_narrow_M4000_madgraph_v1'],
   ###sample['ZprimeToZhToZlephbb_narrow_M4500_madgraph_v1'],
   ###sample['ZprimeToZhToZlephbb_narrow_M600_madgraph_v1'],
   ###sample['ZprimeToZhToZlephbb_narrow_M800_madgraph_v1'],
   #sample['BBbarDMJets_pseudoscalar_Mchi-1000_Mphi-10000_madgraphMLM_pythia8_v1'],
   #sample['BBbarDMJets_pseudoscalar_Mchi-1000_Mphi-1000_madgraphMLM_pythia8_v1'],
   #sample['BBbarDMJets_pseudoscalar_Mchi-1000_Mphi-10_madgraphMLM_pythia8_v1'],
   #sample['BBbarDMJets_pseudoscalar_Mchi-10_Mphi-10000_madgraphMLM_pythia8_v1'],
   #sample['BBbarDMJets_pseudoscalar_Mchi-10_Mphi-100_madgraphMLM_pythia8_v1'],
   #sample['BBbarDMJets_pseudoscalar_Mchi-10_Mphi-10_madgraphMLM_pythia8_v1'],
   #sample['BBbarDMJets_pseudoscalar_Mchi-10_Mphi-15_madgraphMLM_pythia8_v1'],
   #sample['BBbarDMJets_pseudoscalar_Mchi-10_Mphi-50_madgraphMLM_pythia8_v1'],
   #sample['BBbarDMJets_pseudoscalar_Mchi-150_Mphi-10000_madgraphMLM_pythia8_v1'],
   #sample['BBbarDMJets_pseudoscalar_Mchi-150_Mphi-1000_madgraphMLM_pythia8_v1'],
   #sample['BBbarDMJets_pseudoscalar_Mchi-150_Mphi-200_madgraphMLM_pythia8_v1'],
   #sample['BBbarDMJets_pseudoscalar_Mchi-150_Mphi-295_madgraphMLM_pythia8_v1'],
   #sample['BBbarDMJets_pseudoscalar_Mchi-150_Mphi-500_madgraphMLM_pythia8_v1'],
   #sample['BBbarDMJets_pseudoscalar_Mchi-1_Mphi-10000_madgraphMLM_pythia8_v1'],
   #sample['BBbarDMJets_pseudoscalar_Mchi-1_Mphi-1000_madgraphMLM_pythia8_v1'],
   #sample['BBbarDMJets_pseudoscalar_Mchi-1_Mphi-100_madgraphMLM_pythia8_v1'],
   #sample['BBbarDMJets_pseudoscalar_Mchi-1_Mphi-10_madgraphMLM_pythia8_v1'],
   #sample['BBbarDMJets_pseudoscalar_Mchi-1_Mphi-200_madgraphMLM_pythia8_v1'],
   #sample['BBbarDMJets_pseudoscalar_Mchi-1_Mphi-20_madgraphMLM_pythia8_v1'],
   #sample['BBbarDMJets_pseudoscalar_Mchi-1_Mphi-300_madgraphMLM_pythia8_v1'],
   #sample['BBbarDMJets_pseudoscalar_Mchi-1_Mphi-500_madgraphMLM_pythia8_v1'],
   #sample['BBbarDMJets_pseudoscalar_Mchi-1_Mphi-50_madgraphMLM_pythia8_v1'],
   #sample['BBbarDMJets_pseudoscalar_Mchi-500_Mphi-10000_madgraphMLM_pythia8_v1'],
   #sample['BBbarDMJets_pseudoscalar_Mchi-500_Mphi-10_madgraphMLM_pythia8_v1'],
   #sample['BBbarDMJets_pseudoscalar_Mchi-500_Mphi-500_madgraphMLM_pythia8_v1'],
   #sample['BBbarDMJets_pseudoscalar_Mchi-500_Mphi-995_madgraphMLM_pythia8_v1'],
   #sample['BBbarDMJets_pseudoscalar_Mchi-50_Mphi-10_madgraphMLM_pythia8_v1'],
   #sample['BBbarDMJets_pseudoscalar_Mchi-50_Mphi-200_madgraphMLM_pythia8_v1'],
   #sample['BBbarDMJets_pseudoscalar_Mchi-50_Mphi-50_madgraphMLM_pythia8_v1'],
   #sample['BBbarDMJets_pseudoscalar_Mchi-50_Mphi-95_madgraphMLM_pythia8_v1'],
   #sample['BBbarDMJets_scalar_Mchi-1000_Mphi-10000_madgraphMLM_pythia8_v1'],
   #sample['BBbarDMJets_scalar_Mchi-1000_Mphi-1000_madgraphMLM_pythia8_v1'],
   #sample['BBbarDMJets_scalar_Mchi-1000_Mphi-10_madgraphMLM_pythia8_v1'],
   #sample['BBbarDMJets_scalar_Mchi-10_Mphi-10000_madgraphMLM_pythia8_v1'],
   #sample['BBbarDMJets_scalar_Mchi-10_Mphi-100_madgraphMLM_pythia8_v1'],
   #sample['BBbarDMJets_scalar_Mchi-10_Mphi-10_madgraphMLM_pythia8_v1'],
   #sample['BBbarDMJets_scalar_Mchi-10_Mphi-15_madgraphMLM_pythia8_v1'],
   #sample['BBbarDMJets_scalar_Mchi-10_Mphi-50_madgraphMLM_pythia8_v1'],
   #sample['BBbarDMJets_scalar_Mchi-150_Mphi-10000_madgraphMLM_pythia8_v1'],
   #sample['BBbarDMJets_scalar_Mchi-150_Mphi-1000_madgraphMLM_pythia8_v1'],
   #sample['BBbarDMJets_scalar_Mchi-150_Mphi-200_madgraphMLM_pythia8_v1'],
   #sample['BBbarDMJets_scalar_Mchi-150_Mphi-295_madgraphMLM_pythia8_v1'],
   #sample['BBbarDMJets_scalar_Mchi-150_Mphi-500_madgraphMLM_pythia8_v1'],
   #sample['BBbarDMJets_scalar_Mchi-1_Mphi-10000_madgraphMLM_pythia8_v1'],
   #sample['BBbarDMJets_scalar_Mchi-1_Mphi-1000_madgraphMLM_pythia8_v1'],
   #sample['BBbarDMJets_scalar_Mchi-1_Mphi-100_madgraphMLM_pythia8_v1'],
   #sample['BBbarDMJets_scalar_Mchi-1_Mphi-10_madgraphMLM_pythia8_v1'],
   #sample['BBbarDMJets_scalar_Mchi-1_Mphi-200_madgraphMLM_pythia8_v1'],
   #sample['BBbarDMJets_scalar_Mchi-1_Mphi-20_madgraphMLM_pythia8_v1'],
   #sample['BBbarDMJets_scalar_Mchi-1_Mphi-300_madgraphMLM_pythia8_v1'],
   #sample['BBbarDMJets_scalar_Mchi-1_Mphi-500_madgraphMLM_pythia8_v1'],
   #sample['BBbarDMJets_scalar_Mchi-1_Mphi-50_madgraphMLM_pythia8_v1'],
   #sample['BBbarDMJets_scalar_Mchi-500_Mphi-10000_madgraphMLM_pythia8_v1'],
   #sample['BBbarDMJets_scalar_Mchi-500_Mphi-10_madgraphMLM_pythia8_v1'],
   #sample['BBbarDMJets_scalar_Mchi-500_Mphi-500_madgraphMLM_pythia8_v1'],
   #sample['BBbarDMJets_scalar_Mchi-500_Mphi-995_madgraphMLM_pythia8_v1'],
   #sample['BBbarDMJets_scalar_Mchi-50_Mphi-10_madgraphMLM_pythia8_v1'],
   #sample['BBbarDMJets_scalar_Mchi-50_Mphi-50_madgraphMLM_pythia8_v1'],
   #sample['BBbarDMJets_scalar_Mchi-50_Mphi-95_madgraphMLM_pythia8_v1'],
   #sample['TTbarDMJets_pseudoscalar_Mchi-10_Mphi-100_madgraphMLM_pythia8_v1'],
   #sample['TTbarDMJets_pseudoscalar_Mchi-10_Mphi-10_madgraphMLM_pythia8_v1'],
   #sample['TTbarDMJets_pseudoscalar_Mchi-10_Mphi-50_madgraphMLM_pythia8_v1'],
   #sample['TTbarDMJets_pseudoscalar_Mchi-150_Mphi-1000_madgraphMLM_pythia8_v1'],
   #sample['TTbarDMJets_pseudoscalar_Mchi-150_Mphi-200_madgraphMLM_pythia8_v1'],
   #sample['TTbarDMJets_pseudoscalar_Mchi-150_Mphi-500_madgraphMLM_pythia8_v1'],
   #sample['TTbarDMJets_pseudoscalar_Mchi-1_Mphi-100_madgraphMLM_pythia8_v1'],
   #sample['TTbarDMJets_pseudoscalar_Mchi-1_Mphi-10_madgraphMLM_pythia8_v1'],
   #sample['TTbarDMJets_pseudoscalar_Mchi-1_Mphi-200_madgraphMLM_pythia8_v1'],
   #sample['TTbarDMJets_pseudoscalar_Mchi-1_Mphi-20_madgraphMLM_pythia8_v1'],
   #sample['TTbarDMJets_pseudoscalar_Mchi-1_Mphi-300_madgraphMLM_pythia8_v1'],
   #sample['TTbarDMJets_pseudoscalar_Mchi-1_Mphi-500_madgraphMLM_pythia8_v1'],
   #sample['TTbarDMJets_pseudoscalar_Mchi-1_Mphi-50_madgraphMLM_pythia8_v1'],
   #sample['TTbarDMJets_pseudoscalar_Mchi-500_Mphi-500_madgraphMLM_pythia8_v1'],
   #sample['TTbarDMJets_pseudoscalar_Mchi-50_Mphi-200_madgraphMLM_pythia8_v1'],
   #sample['TTbarDMJets_pseudoscalar_Mchi-50_Mphi-300_madgraphMLM_pythia8_v1'],
   #sample['TTbarDMJets_pseudoscalar_Mchi-50_Mphi-50_madgraphMLM_pythia8_v1'],
   #sample['TTbarDMJets_scalar_Mchi-10_Mphi-100_madgraphMLM_pythia8_v1'],
   #sample['TTbarDMJets_scalar_Mchi-10_Mphi-10_madgraphMLM_pythia8_v1'],
   #sample['TTbarDMJets_scalar_Mchi-10_Mphi-50_madgraphMLM_pythia8_v1'],
   #sample['TTbarDMJets_scalar_Mchi-150_Mphi-1000_madgraphMLM_pythia8_v3'],
   #sample['TTbarDMJets_scalar_Mchi-150_Mphi-200_madgraphMLM_pythia8_v1'],
   #sample['TTbarDMJets_scalar_Mchi-150_Mphi-500_madgraphMLM_pythia8_v3'],
   #sample['TTbarDMJets_scalar_Mchi-1_Mphi-1000_madgraphMLM_pythia8_v2'],
   #sample['TTbarDMJets_scalar_Mchi-1_Mphi-100_madgraphMLM_pythia8_v1'],
   #sample['TTbarDMJets_scalar_Mchi-1_Mphi-10_madgraphMLM_pythia8_v1'],
   #sample['TTbarDMJets_scalar_Mchi-1_Mphi-200_madgraphMLM_pythia8_v1'],
   #sample['TTbarDMJets_scalar_Mchi-1_Mphi-20_madgraphMLM_pythia8_v1'],
   #sample['TTbarDMJets_scalar_Mchi-1_Mphi-300_madgraphMLM_pythia8_v1'],
   #sample['TTbarDMJets_scalar_Mchi-1_Mphi-500_madgraphMLM_pythia8_v1'],
   #sample['TTbarDMJets_scalar_Mchi-1_Mphi-50_madgraphMLM_pythia8_v1'],
   #sample['TTbarDMJets_scalar_Mchi-500_Mphi-500_madgraphMLM_pythia8_v1'],
   #sample['TTbarDMJets_scalar_Mchi-50_Mphi-200_madgraphMLM_pythia8_v1'],
   #sample['TTbarDMJets_scalar_Mchi-50_Mphi-300_madgraphMLM_pythia8_v1'],
   #sample['TTbarDMJets_scalar_Mchi-50_Mphi-50_madgraphMLM_pythia8_v1'],
#]

### TEST (LOCAL)
#selectedComponents = [sample['Test'],]

#selectedComponents = [sample['SYNCH_ADDMonojet'],]
#selectedComponents = [sample['SYNCH_TTBar'],]
#selectedComponents = [sample['SYNCH_DYJetsToLL'],]
#selectedComponents = [sample['SYNCH_WJetsToLNu'],]
#selectedComponents = [sample['SYNCH_RSGravitonToGaGa'],]
#selectedComponents = [sample['SYNCH_ADDMonojet'],sample['SYNCH_TTBar'],sample['SYNCH_DYJetsToLL'],sample['SYNCH_WJetsToLNu'],sample['SYNCH_RSGravitonToGaGa'],]
#selectedComponents = [sample['SingleMuon_Run2015B_17Jul2015_v1'],]
#selectedComponents = [sample['DoubleMuon_Run2015B_PromptReco_v1'],]
#selectedComponents = [sample['SingleElectron_Run2015B_17Jul2015_v1'],]


from PhysicsTools.Heppy.utils.cmsswPreprocessor import CmsswPreprocessor
preprocessor = CmsswPreprocessor("corMETFromMiniAOD.py")

config = cfg.Config(
    components = selectedComponents,
    sequence = sequence,
    services = [output_service],
    #preprocessor = preprocessor,
    events_class = Events
    )

##############################
### LOOPER                 ###
##############################
# and the following runs the process directly if running as with python filename.py
if __name__ == '__main__':
    from PhysicsTools.HeppyCore.framework.looper import Looper
    looper = Looper(
        'DM',
        config,
        nPrint = 1,
        nEvents=1e99,
        )
    looper.loop()
    looper.write()
