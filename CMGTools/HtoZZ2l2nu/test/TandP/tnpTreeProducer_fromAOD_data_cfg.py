import FWCore.ParameterSet.Config as cms

process = cms.Process("TagProbe")

process.load('FWCore.MessageService.MessageLogger_cfi')
process.options   = cms.untracked.PSet( wantSummary = cms.untracked.bool(True) )
process.MessageLogger.cerr.FwkReport.reportEvery = 100

process.source = cms.Source("PoolSource", 
    fileNames = cms.untracked.vstring(
	'/store/data/Run2011A/SingleMu/AOD/PromptReco-v6/000/173/240/F60FD515-1AC7-E011-9904-001D09F2915A.root',
#                                 '/store/data/Run2011A/SingleMu/AOD/PromptReco-v2/000/163/339/F4E034EB-2170-E011-B8A2-001617E30D52.root',
# 	'/store/data/Run2011A/SingleMu/AOD/PromptReco-v2/000/163/339/AE8F9B2C-7F70-E011-8A4A-0030487A1884.root',
#         '/store/data/Run2011A/SingleMu/AOD/PromptReco-v2/000/163/339/8693F25C-2670-E011-A5E7-003048F110BE.root',
    ),
)
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(2000) )    

process.load("Configuration.StandardSequences.MagneticField_cff")
process.load("Configuration.StandardSequences.Geometry_cff")
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
process.load("Configuration.StandardSequences.Reconstruction_cff")
process.GlobalTag.globaltag = cms.string('GR_R_42_V20::All')

## ==== Fast Filters ====
process.goodVertexFilter = cms.EDFilter("VertexSelector",
    src = cms.InputTag("offlinePrimaryVertices"),
    cut = cms.string("!isFake && ndof > 4 && abs(z) <= 25 && position.Rho <= 2"),
    filter = cms.bool(True),
)
process.noScraping = cms.EDFilter("FilterOutScraping",
    applyfilter = cms.untracked.bool(True),
    debugOn = cms.untracked.bool(False), ## Or 'True' to get some per-event info
    numtrack = cms.untracked.uint32(10),
    thresh = cms.untracked.double(0.25)
)
process.fastFilter = cms.Sequence(process.goodVertexFilter + process.noScraping)
##    __  __                       
##   |  \/  |_   _  ___  _ __  ___ 
##   | |\/| | | | |/ _ \| '_ \/ __|
##   | |  | | |_| | (_) | | | \__ \
##   |_|  |_|\__,_|\___/|_| |_|___/
##                                 
## ==== Merge CaloMuons and Tracks into the collection of reco::Muons  ====


# add PFTOPAT sequence for muons and PF isolation
# pat sequences
#runOnMC = False
process.load("CommonTools.ParticleFlow.PF2PAT_cff")


process.patMuonsPFlow = cms.EDProducer("PATMuonProducer",
    embedTpfmsMuon = cms.bool(True),
    embedHighLevelSelection = cms.bool(True),
    embedCaloMETMuonCorrs = cms.bool(True),
    caloMETMuonCorrs = cms.InputTag("muonMETValueMapProducer","muCorrData"),
    addResolutions = cms.bool(False),
    resolutions = cms.PSet(

    ),
    userIsolation = cms.PSet(

    ),
    embedPFCandidate = cms.bool(True),
    pfMuonSource = cms.InputTag("pfIsolatedMuons"),
    efficiencies = cms.PSet(

    ),
    embedStandAloneMuon = cms.bool(True),
    useParticleFlow = cms.bool(True),
    userData = cms.PSet(
        userCands = cms.PSet(
            src = cms.VInputTag("")
        ),
        userInts = cms.PSet(
            src = cms.VInputTag("")
        ),
        userFloats = cms.PSet(
            src = cms.VInputTag("")
        ),
        userClasses = cms.PSet(
            src = cms.VInputTag("")
        ),
        userFunctionLabels = cms.vstring(),
        userFunctions = cms.vstring()
    ),
    embedTrack = cms.bool(True),
    addEfficiencies = cms.bool(False),
    usePV = cms.bool(True),
    embedTcMETMuonCorrs = cms.bool(True),
    pvSrc = cms.InputTag("offlinePrimaryVertices"),
    addTeVRefits = cms.bool(True),
    embedCombinedMuon = cms.bool(True),
    genParticleMatch = cms.InputTag(""),
    beamLineSrc = cms.InputTag("offlineBeamSpot"),
    addGenMatch = cms.bool(False),
    muonSource = cms.InputTag("muons"),
    tpfmsSrc = cms.InputTag("tevMuons","firstHit"),
    pickySrc = cms.InputTag("tevMuons","picky"),
    isoDeposits = cms.PSet(
        pfNeutralHadrons = cms.InputTag("isoDepMuonWithNeutral"),
        pfChargedHadrons = cms.InputTag("isoDepMuonWithCharged"),
        pfPhotons = cms.InputTag("isoDepMuonWithPhotons")
    ),
    embedGenMatch = cms.bool(False),
    tcMETMuonCorrs = cms.InputTag("muonTCMETValueMapProducer","muCorrData"),
    embedPickyMuon = cms.bool(True),
    isolationValues = cms.PSet(
        pfNeutralHadrons = cms.InputTag("isoValMuonWithNeutral"),
        pfChargedHadrons = cms.InputTag("isoValMuonWithCharged"),
        pfPhotons = cms.InputTag("isoValMuonWithPhotons")
    )
)

postfix = ""

#configure top projections
getattr(process,"pfNoPileUp"+postfix).enable = True
getattr(process,"pfNoMuon"+postfix).enable = True
getattr(process,"pfNoMuon"+postfix).verbose = False
getattr(process,"pfNoElectron"+postfix).enable = True
getattr(process,"pfNoTau"+postfix).enable = False
getattr(process,"pfNoJet"+postfix).enable = False

#fix isolation to use a cone of 0.3 for both electrons and muons
process.isoValMuonWithNeutral.deposits[0].deltaR = cms.double(0.3)
process.isoValMuonWithCharged.deposits[0].deltaR = cms.double(0.3)
process.isoValMuonWithPhotons.deposits[0].deltaR = cms.double(0.3)
process.pfIsolatedMuons.combinedIsolationCut = cms.double(9999.)
process.isoValElectronWithNeutral.deposits[0].deltaR = cms.double(0.3)
process.isoValElectronWithCharged.deposits[0].deltaR = cms.double(0.3)
process.isoValElectronWithPhotons.deltaR = cms.double(0.3)
process.pfIsolatedElectrons.combinedIsolationCut = cms.double(9999.)
    
process.pf2PatMuons = cms.Sequence(
    process.pfParticleSelectionSequence + 
    process.pfMuonSequence +
    process.patMuonsPFlow)




from RecoMuon.MuonIdentification.calomuons_cfi import calomuons;
process.mergedMuons = cms.EDProducer("CaloMuonMerger",
    mergeTracks = cms.bool(True),
    mergeCaloMuons = cms.bool(False), # AOD
    muons     = cms.InputTag("muons"), 
    caloMuons = cms.InputTag("calomuons"),
    tracks    = cms.InputTag("generalTracks"),
    minCaloCompatibility = calomuons.minCaloCompatibility,
    ## Apply some minimal pt cut
    muonsCut     = cms.string("pt > 5 && track.isNonnull"),
    caloMuonsCut = cms.string("pt > 5"),
    tracksCut    = cms.string("pt > 5"),
)

## ==== Trigger matching
process.load("MuonAnalysis.MuonAssociators.patMuonsWithTrigger_cff")
## with some customization
process.muonMatchHLTL2.maxDeltaR = 0.3 # Zoltan tuning - it was 0.5
process.muonMatchHLTL3.maxDeltaR = 0.1
from MuonAnalysis.MuonAssociators.patMuonsWithTrigger_cff import *
changeRecoMuonInput(process, "mergedMuons")



from MuonAnalysis.TagAndProbe.common_variables_cff import *
process.load("MuonAnalysis.TagAndProbe.common_modules_cff")

muontype = "isGlobalMuon && isTrackerMuon"
kinacc = muontype + " && pt > 20 && abs(eta) < 2.4"
goodtrk = kinacc + " && globalTrack.normalizedChi2<10 && globalTrack.hitPattern.numberOfValidTrackerHits>11 && globalTrack.hitPattern.numberOfValidMuonHits>1"  
iso = goodtrk + " && (neutralHadronIso+chargedHadronIso+photonIso)/pt < 0.15"


from CMGTools.HtoZZ2l2nu.leptonsPassingPFSelection_cfi import leptonsPassingPF
process.muPassingId = leptonsPassingPF.clone()
process.muPassingId.pfCut = muontype

process.muPassingKin = leptonsPassingPF.clone()
process.muPassingKin.pfCut = kinacc

process.muPassingGoodTrk = leptonsPassingPF.clone()
process.muPassingGoodTrk.pfCut = goodtrk

process.muPassingIso = leptonsPassingPF.clone()
process.muPassingIso.pfCut = iso


process.tagMuons = cms.EDFilter("PATMuonSelector",
    src = cms.InputTag("patMuonsWithTrigger"),
    cut = cms.string("pt > 15 && "+MuonIDFlags.VBTF.value()+" && !triggerObjectMatchesByCollection('hltL3MuonCandidates').empty()"),
)




process.oneTag  = cms.EDFilter("CandViewCountFilter", src = cms.InputTag("tagMuons"), minNumber = cms.uint32(1))

process.probeMuons = cms.EDFilter("PATMuonSelector",
    src = cms.InputTag("patMuonsWithTrigger"),
    cut = cms.string("track.isNonnull"),  # no real cut now
)

process.tpPairs = cms.EDProducer("CandViewShallowCloneCombiner",
    cut = cms.string('60 < mass < 140'),
    decay = cms.string('tagMuons@+ probeMuons@-')
)
process.onePair = cms.EDFilter("CandViewCountFilter", src = cms.InputTag("tpPairs"), minNumber = cms.uint32(1))

from MuonAnalysis.TagAndProbe.muon.tag_probe_muon_extraIso_cff import ExtraIsolationVariables

#import the module to get he lumi as variable associated to the probe
process.load("Tools.MyAnalysisTools.lumiDetailsForTP_cfi")




process.tpTree = cms.EDAnalyzer("TagProbeFitTreeProducer",
    # choice of tag and probe pairs, and arbitration
    tagProbePairs = cms.InputTag("tpPairs"),
    arbitration   = cms.string("OneProbe"),
    # probe variables: all useful ones
    variables = cms.PSet(
        AllVariables,
        ExtraIsolationVariables,
        isoTrk03Abs = cms.InputTag("probeMuonsIsoValueMaps","probeMuonsIsoFromDepsTk"),
        isoTrk03Rel = cms.InputTag("probeMuonsIsoValueMaps","probeMuonsRelIsoFromDepsTk"),
        dxyPVdzmin = cms.InputTag("muonDxyPVdzmin","dxyPVdzmin"),
        dzPV = cms.InputTag("muonDxyPVdzmin","dzPV"),
        avgInsDelLumi =           cms.InputTag("lumiDetailsForTP","avgInsDelLumi"),
        intgDelLumi =           cms.InputTag("lumiDetailsForTP","intgDelLumi"),
        avgInsRecLumi =           cms.InputTag("lumiDetailsForTP","avgInsRecLumi"),
        intgRecLumi =           cms.InputTag("lumiDetailsForTP","intgRecLumi"),
        bx =           cms.InputTag("lumiDetailsForTP","bx"),

    ),
    flags = cms.PSet(
       TrackQualityFlags,
       MuonIDFlags,
       HighPtTriggerFlags,
       # Extra triggers
       Mu8_forEMu  = cms.string("!triggerObjectMatchesByFilter('hltL1MuOpenEG5L3Filtered8').empty"),
       Mu17_forEMu = cms.string("!triggerObjectMatchesByFilter('hltL1MuOpenEG5L3Filtered17').empty"),
       ## Isolation
       Isol    = cms.string("(isolationR03.emEt + isolationR03.hadEt + isolationR03.sumPt)/pt < 0.15"), 
       IsolTk3 = cms.string("isolationR03.sumPt < 3"), 
       ## ParticleFlow
       PF = cms.InputTag("muonsPassingPF"),
       Track_VBTF  = cms.string("track.numberOfValidHits > 10 && track.hitPattern.pixelLayersWithMeasurement > 0 && abs(dB) < 0.2"),
       passingMuon = cms.InputTag("muPassingId"),
       passingKin = cms.InputTag("muPassingKin"),
       passingTrk = cms.InputTag("muPassingGoodTrk"),
       passingIso = cms.InputTag("muPassingIso")
    ),
    tagVariables = cms.PSet(
        nVertices   = cms.InputTag("nverticesModule"),
        nVerticesDA = cms.InputTag("nverticesDAModule"),
        combRelIso = cms.string("(isolationR03.emEt + isolationR03.hadEt + isolationR03.sumPt)/pt"),
    ),
    tagFlags = cms.PSet(HighPtTriggerFlags),
    pairVariables = cms.PSet(
        nJets15 = cms.InputTag("njets15Module"),
        nJets30 = cms.InputTag("njets30Module"),
        dz      = cms.string("daughter(0).vz - daughter(1).vz"),
        nL1EG5  = cms.InputTag("nL1EG5Module"), # to unlock EMu triggers
        pt      = cms.string("pt"), # let's do some bump hunt in the T&P too
    ),
    pairFlags = cms.PSet(),
    isMC           = cms.bool(False),
    addRunLumiInfo = cms.bool(True),
)



process.load('RecoJets.Configuration.RecoPFJets_cff')
##-------------------- Turn-on the FastJet density calculation -----------------------
process.kt6PFJets.doRhoFastjet = True
##-------------------- FastJet density calculation for Iso -----------------------
process.kt6PFJetsForIso = process.kt6PFJets.clone( Rho_EtaMax = cms.double(2.5), Ghost_EtaMax = cms.double(2.5) )


process.load("MuonAnalysis.TagAndProbe.muon.tag_probe_muon_extraIso_cfi")

process.tnpSimpleSequence = cms.Sequence(
    process.tagMuons +
    process.oneTag     +
    process.probeMuons +
    process.tpPairs    +
    process.onePair    +
    process.nverticesModule +
    process.muonDxyPVdzmin +
    process.lumiDetailsForTP +
    process.offlinePrimaryVerticesDA100um * process.nverticesDAModule +
    process.njets15Module +
    process.njets30Module +
    process.mergedL1EG + process.nL1EG5Module +
    process.muonsPassingPF +
    process.muPassingId +
    process.muPassingKin +
    process.muPassingGoodTrk +
    process.muPassingIso +
    process.probeMuonsIsoSequence +
    process.kt6PFJetsForIso * process.computeCorrectedIso + 
    process.tpTree
)

process.tagAndProbe = cms.Path( 
    process.fastFilter +
    process.mergedMuons                 *
    process.pf2PatMuons                 +
    process.patMuonsWithTriggerSequence +
    process.tnpSimpleSequence
)

##    _____               _    _             
##   |_   _| __ __ _  ___| | _(_)_ __   __ _ 
##     | || '__/ _` |/ __| |/ / | '_ \ / _` |
##     | || | | (_| | (__|   <| | | | | (_| |
##     |_||_|  \__,_|\___|_|\_\_|_| |_|\__, |
##                                     |___/ 

## Then make another collection for standalone muons, using standalone track to define the 4-momentum
process.muonsSta = cms.EDProducer("RedefineMuonP4FromTrack",
    src   = cms.InputTag("muons"),
    track = cms.string("outer"),
)
## Match to trigger, to measure the efficiency of HLT tracking
from PhysicsTools.PatAlgos.tools.helpers import *
process.patMuonsWithTriggerSequenceSta = cloneProcessingSnippet(process, process.patMuonsWithTriggerSequence, "Sta")
process.muonMatchHLTL2Sta.maxDeltaR = 0.5
process.muonMatchHLTL3Sta.maxDeltaR = 0.5
massSearchReplaceAnyInputTag(process.patMuonsWithTriggerSequenceSta, "mergedMuons", "muonsSta")

## Define probes and T&P pairs
process.probeMuonsSta = cms.EDFilter("PATMuonSelector",
    src = cms.InputTag("patMuonsWithTriggerSta"),
    cut = cms.string("outerTrack.isNonnull"), # no real cut now
)

process.tpPairsSta = process.tpPairs.clone(decay = "tagMuons@+ probeMuonsSta@-", cut = '40 < mass < 150')

process.onePairSta = cms.EDFilter("CandViewCountFilter", src = cms.InputTag("tpPairsSta"), minNumber = cms.uint32(1))

process.staToTkMatch.maxDeltaR     = 0.3
process.staToTkMatch.maxDeltaPtRel = 2.
process.staToTkMatchNoZ.maxDeltaR     = 0.3
process.staToTkMatchNoZ.maxDeltaPtRel = 2.

process.tpTreeSta = process.tpTree.clone(
    tagProbePairs = "tpPairsSta",
    variables = cms.PSet(
        KinematicVariables, 
        ## track matching variables
        tk_deltaR     = cms.InputTag("staToTkMatch","deltaR"),
        tk_deltaEta   = cms.InputTag("staToTkMatch","deltaEta"),
        tk_deltaR_NoZ   = cms.InputTag("staToTkMatchNoZ","deltaR"),
        tk_deltaEta_NoZ = cms.InputTag("staToTkMatchNoZ","deltaEta"),
    ),
    flags = cms.PSet(
        outerValidHits = cms.string("outerTrack.numberOfValidHits > 0"),
        TM  = cms.string("isTrackerMuon"),
        Glb = cms.string("isGlobalMuon"),
    ),
)
process.tpTreeSta.pairVariables.nJets15 = "njets15ModuleSta"
process.tpTreeSta.pairVariables.nJets30 = "njets30ModuleSta"
del process.tpTreeSta.pairVariables.nL1EG5
process.njets15ModuleSta = process.njets15Module.clone(pairs = "tpPairsSta")
process.njets30ModuleSta = process.njets30Module.clone(pairs = "tpPairsSta")

process.tnpSimpleSequenceSta = cms.Sequence(
    process.tagMuons +
    process.oneTag     +
    process.probeMuonsSta   +
    process.tpPairsSta      +
    process.onePairSta      +
    process.nverticesModule +
    process.offlinePrimaryVerticesDA100um * process.nverticesDAModule +
    process.staToTkMatchSequenceZ +
    process.njets15ModuleSta +
    process.njets30ModuleSta +
    process.tpTreeSta
)

process.tagAndProbeSta = cms.Path( 
    process.fastFilter +
    process.muonsSta                       +
    process.patMuonsWithTriggerSequenceSta +
    process.tnpSimpleSequenceSta
)

process.TFileService = cms.Service("TFileService", fileName = cms.string("/tmp/cerminar/tnpZ_Data.root"))



temp = process.dumpPython()
outputfile = file("/tmp/cerminar/tnpTreeProducer_dump.py",'w')
outputfile.write(temp)
outputfile.close()
