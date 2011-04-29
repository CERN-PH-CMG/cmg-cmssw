from PhysicsTools.PatAlgos.patTemplate_cfg import *

# FIXME: check the GT for JECs
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
if ( not runOnMC ):
    process.GlobalTag.globaltag = 'GR_R_311_V2::All'
else:
    process.GlobalTag.globaltag = 'START311_V2::All'

process.options = cms.untracked.PSet(
    wantSummary = cms.untracked.bool(True),
    SkipEvent = cms.untracked.vstring('ProductNotFound')
    )

# =================================================================================
# local input
#
from CMGTools.HtoZZ2l2nu.localPatTuples_cff import fillFromCastor
#process.source.fileNames=fillFromCastor('/castor/cern.ch/cms/store/data/Run2011A/DoubleElectron/RECO/PromptReco-v1/000/161/311')
process.source.fileNames=cms.untracked.vstring('file:/tmp/psilva/Events_18_1_gZH.root')
#process.source.fileNames = cms.untracked.vstring('/store/data/Run2010A/MinimumBias/RAW-RECO/v6/000/144/114/CCDC14CB-61BB-DF11-A07F-0025B3E063E8.root')
if( not runOnMC and useLocalLumiSelection):
    import PhysicsTools.PythonAnalysis.LumiList as LumiList
    import FWCore.ParameterSet.Types as CfgTypes
    myLumis = LumiList.LumiList(filename = 'Data/Cert_161311_161312_7TeV_Collisions2011_JSON.txt').getCMSSWString().split(',')
    process.source.lumisToProcess = CfgTypes.untracked(CfgTypes.VLuminosityBlockRange())
    process.source.lumisToProcess.extend(myLumis)
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

# =================================================================================
# include counters
#
process.startCounter = cms.EDProducer("EventCountProducer")
process.triggerCounter = process.startCounter.clone()
process.preFilterCounter = process.startCounter.clone()
process.eeCounter = process.startCounter.clone()
process.mumuCounter = process.startCounter.clone()
process.emuCounter = process.startCounter.clone()
process.endCounter = process.startCounter.clone()

# ==================================================================================
# prepare the trigger filter
#
from HLTrigger.HLTfilters.hltHighLevel_cfi import *
process.eetrigFilter = hltHighLevel.clone(TriggerResultsTag = "TriggerResults::HLT")
process.eetrigFilter.throw = cms.bool(False)
process.eetrigFilter.HLTPaths = ['HLT_Ele17_CaloIdL_CaloIsoVL_Ele8_CaloIdL_CaloIsoVL_v1',
                                 'HLT_Ele17_CaloIdL_CaloIsoVL_Ele8_CaloIdL_CaloIsoVL_v2',
                                 'HLT_Ele17_CaloIdL_CaloIsoVL_Ele15_HFL_v1',
                                 'HLT_Ele17_CaloIdL_CaloIsoVL_Ele15_HFL_v2',
                                 'HLT_Ele32_CaloIdL_CaloIsoVL_SC17_v1',
                                 'HLT_Ele32_CaloIdL_CaloIsoVL_SC17_v2']
process.mumutrigFilter = process.eetrigFilter.clone()
process.mumutrigFilter.HLTPaths =['HLT_DoubleMu6_v1',
                                  'HLT_DoubleMu7_v1']
process.emutrigFilter = process.eetrigFilter.clone()
process.emutrigFilter.HLTPaths = ['HLT_Mu17_Ele8_CaloIdL_v1',
                                  'HLT_Mu17_Ele8_CaloIdL_v2',
                                  'HLT_Mu8_Ele17_CaloIdL_v1',
                                  'HLT_Mu8_Ele17_CaloIdL_v2',
                                  'HLT_Mu10_Ele10_CaloIdL_v2',
                                  'HLT_Mu10_Ele10_CaloIdL_v3']
process.singlemutrigFilter = process.eetrigFilter.clone()
process.singlemutrigFilter.HLTPaths = ['HLT_Mu9',
                                       'HLT_Mu15_v1']
process.singleetrigFilter = process.eetrigFilter.clone()
process.singleetrigFilter.HLTPaths = ['HLT_Ele10_LW_L1R',
                                      'HLT_Ele15_SW_L1R',
                                      'HLT_Ele15_SW_CaloEleId_L1R',
                                      'HLT_Ele17_SW_CaloEleId_L1R',
                                      'HLT_Ele17_SW_TightEleId_L1R',
                                      'HLT_Ele17_SW_TighterEleIdIsol_L1R_v2',
                                      'HLT_Ele17_SW_TighterEleIdIsol_L1R_v3']

process.incEETrigSequence=cms.Sequence(process.eetrigFilter)
process.excMuMuTrigSequence=cms.Sequence(~process.eetrigFilter*process.mumutrigFilter)
process.excEmuTrigSequence=cms.Sequence(~process.eetrigFilter*~process.mumutrigFilter*process.emutrigFilter)
process.incSingleETrigSequence=cms.Sequence(process.singleetrigFilter)
process.excSingleMuTrigSequence=cms.Sequence(~process.singleetrigFilter*process.singlemutrigFilter)

if( not runOnMC ):
    if(applyTrigSequence=='ee') :  process.trigSequence = cms.Sequence(process.incEETrigSequence*process.triggerCounter)
    if(applyTrigSequence=='mumu'): process.trigSequence = cms.Sequence(process.excMuMuTrigSequence*process.triggerCounter)
    if(applyTrigSequence=='emu') : process.trigSequence = cms.Sequence(process.excEmuTrigSequence*process.triggerCounter)
    if(applyTrigSequence=='e') :   process.trigSequence = cms.Sequence(process.incSingleETrigSequence*process.triggerCounter)
    if(applyTrigSequence=='mu') :  process.trigSequence = cms.Sequence(process.excSingleMuTrigSequence*process.triggerCounter)

# ==================================================================================
# run the deterministic annealing vertex
# cf. https://twiki.cern.ch/twiki/bin/view/CMS/PrimaryVertex2011
process.offlinePrimaryVerticesDA = cms.EDProducer("PrimaryVertexProducer",
    verbose = cms.untracked.bool(False),
    algorithm = cms.string('AdaptiveVertexFitter'),
    TrackLabel = cms.InputTag("generalTracks"),
    useBeamConstraint = cms.bool(False),
    beamSpotLabel = cms.InputTag("offlineBeamSpot"),
    minNdof  = cms.double(0.0),
    PVSelParameters = cms.PSet(
        maxDistanceToBeam = cms.double(1.0)
    ),
    TkFilterParameters = cms.PSet(
        algorithm=cms.string('filter'),
        maxNormalizedChi2 = cms.double(20.0),
        minPixelLayersWithHits=cms.int32(2),
        minSiliconLayersWithHits = cms.int32(5),
        maxD0Significance = cms.double(5.0), 
        minPt = cms.double(0.0),
        trackQuality = cms.string("any")
    ),

    TkClusParameters = cms.PSet(
        algorithm   = cms.string("DA"),
        TkDAClusParameters = cms.PSet(
            coolingFactor = cms.double(0.6),  #  moderate annealing speed
            Tmin = cms.double(4.),            #  end of annealing
            vertexSize = cms.double(0.01),    #  ~ resolution / sqrt(Tmin)
            d0CutOff = cms.double(3.),        # downweight high IP tracks 
            dzCutOff = cms.double(4.)         # outlier rejection after freeze-out (T<Tmin)
        )
    )
)
process.runVertexing = cms.Sequence( process.offlinePrimaryVerticesDA )
# Tracks filtered
process.goodTracks = cms.EDFilter("TrackSelector",
                                  src = cms.InputTag("generalTracks"),
                                  cut = cms.string('pt > 1.0 && abs(eta)<2.4 && normalizedChi2<5 && abs(d0)<2 && abs(dz)<20')
                                  )


# ==================================================================================
# include pre-filter sequences
#
process.noscraping = cms.EDFilter("FilterOutScraping",
                                  applyfilter = cms.untracked.bool(True),
                                  debugOn = cms.untracked.bool(False),
                                  numtrack = cms.untracked.uint32(10),
                                  thresh = cms.untracked.double(0.25) )
process.primaryVertexFilter = cms.EDFilter("VertexSelector",
                                           src = cms.InputTag("offlinePrimaryVertices"),
                                           cut = cms.string("!isFake && ndof > 4 && abs(z) <= 24 && position.Rho <= 2"),
                                           filter = cms.bool(True)
                                           )

process.load('CommonTools/RecoAlgos/HBHENoiseFilterResultProducer_cfi')

if(runOnMC):
    #process.preFilter = cms.Sequence( process.noscraping*process.primaryVertexFilter*process.HBHENoiseFilterResultProducer*process.goodTracks)
    process.preFilter = cms.Sequence( process.noscraping*process.primaryVertexFilter*process.goodTracks)
else :
    process.preFilter = cms.Sequence( process.trigSequence*process.noscraping*process.primaryVertexFilter*process.HBHENoiseFilterResultProducer*process.goodTracks)

#process.load('CommonTools/RecoAlgos/HBHENoiseFilter_cfi')
#process.preFilter = cms.Sequence( process.noscraping*process.primaryVertexFilter*process.HBHENoiseFilter)


# ============================================================================
# include gen level utils
# FIXME: add acceptance filter?
process.load('SimGeneral.HepPDTESSource.pythiapdt_cfi')
process.prunedGen = cms.EDProducer( "GenParticlePruner",
    src = cms.InputTag("genParticles"),
    select = cms.vstring(

        "drop  *  ",
        "keep++ pdgId =   {Z0}",
        "++keep pdgId =   {Z0}",
        "keep++ pdgId =   {W+}",
        "++keep pdgId =   {W+}",
        "keep++ pdgId =   {W-}",
        "++keep pdgId =   {W-}",
        "keep++ pdgId =   {h0}",
        "++keep pdgId =   {h0}",
        "keep++ pdgId =   {e+}",
        "++keep pdgId =   {e+}",
        "keep++ pdgId =   {e-}",
        "++keep pdgId =   {e-}",
        "keep++ pdgId =  {mu+}",
        "++keep pdgId =  {mu+}",
        "keep++ pdgId =  {mu-}",
        "++keep pdgId =  {mu-}",
        "++keep pdgId =      6",
        "++keep pdgId =     -6",
        "++keep pdgId =      5",
        "++keep pdgId =     -5",
        "++keep pdgId =      4",
        "++keep pdgId =     -4",
        "++keep pdgId =     12",
        "++keep pdgId =     14",
        "++keep pdgId =     16",
        "++keep pdgId =    -12",
        "++keep pdgId =    -14",
        "++keep pdgId =    -16",
        "++keep pdgId = {tau+}",
        "++keep pdgId = {tau-}",
    )
)


# =============================================================================================
# PAT configuration
#
process.load("PhysicsTools.PatAlgos.patSequences_cff")
from PhysicsTools.PatAlgos.tools.coreTools import *

# match the trigger information
process.load("PhysicsTools.PatAlgos.triggerLayer1.triggerProducer_cff")
process.patTrigger.onlyStandAlone = True
process.patTrigger.processName  = '*'
process.patTriggerEvent.processName = '*'

#
# muons
#
process.muonTriggerMatchHLT = cms.EDProducer( 'PATTriggerMatcherDRDPtLessByR',
                                              src     = cms.InputTag( 'patMuons' ),
                                              matched = cms.InputTag( 'patTrigger' ),
                                              matchedCuts = cms.string( 'path( "HLT_*" )' ),
                                              maxDPtRel = cms.double( 0.5 ),
                                              maxDeltaR = cms.double( 0.5 ),
                                              resolveAmbiguities    = cms.bool( True ),        # only one match per trigger object
                                              resolveByMatchQuality = cms.bool( True )
                                              )
process.patMuonsWithTrigger = cms.EDProducer( 'PATTriggerMatchMuonEmbedder',
    src     = cms.InputTag(  'patMuons' ),
    matches = cms.VInputTag('muonTriggerMatchHLT')
)

#pat muon filling
process.patMuons.embedPFCandidate = False
process.patMuons.embedTrack = True
process.muonMatch.matched = "prunedGen"
classifyMuonsByHits=False ## Turn this on to get extra info on muon MC origin, on GEN-SIM-RECO

if runOnMC: 
    if classifyMuonsByHits: 
        process.load("MuonAnalysis.MuonAssociators.muonClassificationByHits_cfi")
        from MuonAnalysis.MuonAssociators.muonClassificationByHits_cfi import addUserData as addClassByHits
        addClassByHits(process.patMuons, labels=['classByHitsGlb'], extraInfo = True)
        process.muonClassificationByHits = cms.Sequence(process.mix * process.trackingParticlesNoSimHits * process.classByHitsGlb )
        process.preMuonSequence = cms.Sequence(process.prunedGen * process.muonMatch + process.patTrigger + process.muonClassificationByHits)
        process.MessageLogger.suppressWarning += ['classByHitsGlb'] # kill stupid RPC hit associator warning
    else:
        process.preMuonSequence = cms.Sequence(process.prunedGen * process.muonMatch + process.patTrigger)
else:
    removeMCMatching(process, ['Muons'])
    process.preMuonSequence = cms.Sequence(process.patTrigger)

# the Muon Selector
process.selMuons = cms.EDFilter("PATMuonRefSelector",
    src = cms.InputTag("patMuonsWithTrigger"),
    cut = cms.string("isGlobalMuon && pt >= 15 && abs(eta) < 2.4")
)

# the muon sequence
process.muSeq = cms.Sequence( 
    process.preMuonSequence *
    process.patMuons *
    process.muonTriggerMatchHLT *
    process.patMuonsWithTrigger *
    process.selMuons )


# 
# electron selection
#
process.eleTriggerMatchHLT = cms.EDProducer( "PATTriggerMatcherDRDPtLessByR",
                                             src     = cms.InputTag( "patElectrons" ),
                                             matched = cms.InputTag( "patTrigger" ),
                                             matchedCuts = cms.string( 'path( "HLT_*" )' ),
                                             maxDPtRel = cms.double( 0.5 ),
                                             maxDeltaR = cms.double( 0.5 ),
                                             resolveAmbiguities    = cms.bool( True ),        # only one match per trigger object
                                             resolveByMatchQuality = cms.bool( True )
                                             )
process.eleIdTriggerMatchHLT = process.eleTriggerMatchHLT.clone(collectionTags = cms.vstring('hltPixelMatchElectronsL1Iso', 'hltPixelMatchElectronsL1NonIso') )
process.patElectronsWithTrigger = cms.EDProducer( "PATTriggerMatchElectronEmbedder",
    src     = cms.InputTag(  "patElectrons" ),
    matches = cms.VInputTag(cms.InputTag('eleTriggerMatchHLT'), cms.InputTag('eleIdTriggerMatchHLT'))
) 

#run electron ids
process.load("ElectroWeakAnalysis.WENu.simpleEleIdSequence_cff")
import RecoEgamma.ElectronIdentification.cutsInCategoriesElectronIdentificationV06_DataTuning_cfi as cic
process.eidCiCVeryLoose = cic.eidVeryLoose.clone()
process.eidCiCLoose = cic.eidLoose.clone()
process.eidCiCMedium = cic.eidMedium.clone()
process.eidCiCTight = cic.eidTight.clone()
process.patElectrons.electronIDSources.simpleEleId95relIso=cms.InputTag("simpleEleId95relIso")
process.patElectrons.electronIDSources.simpleEleId90relIso=cms.InputTag("simpleEleId90relIso")
process.patElectrons.electronIDSources.simpleEleId85relIso=cms.InputTag("simpleEleId85relIso")
process.patElectrons.electronIDSources.simpleEleId80relIso=cms.InputTag("simpleEleId80relIso")
process.patElectrons.electronIDSources.simpleEleId70relIso=cms.InputTag("simpleEleId70relIso")
process.patElectrons.electronIDSources.simpleEleId60relIso=cms.InputTag("simpleEleId60relIso")
process.patElectrons.electronIDSources.eidCiCVeryLoose = cms.InputTag("eidCiCVeryLoose")
process.patElectrons.electronIDSources.eidCiCLoose = cms.InputTag("eidCiCLoose")
process.patElectrons.electronIDSources.eidCiCMedium = cms.InputTag("eidCiCMedium")
process.patElectrons.electronIDSources.eidCiCTight = cms.InputTag("eidCiCTight")
process.patElectronId = cms.Sequence(
    process.simpleEleIdSequence +
    process.eidCiCVeryLoose +
    process.eidCiCLoose +
    process.eidCiCMedium +
    process.eidCiCTight 
)

#pat electron filling
process.patElectrons.embedPFCandidate = False
process.patElectrons.embedSuperCluster = True
process.patElectrons.embedTrack = True
process.patElectrons.addElectronID = True
process.electronMatch.matched = "prunedGen"

# electron selector
process.selElectrons = cms.EDFilter("PATElectronRefSelector",
    src = cms.InputTag("patElectronsWithTrigger"),
    cut = cms.string(" ecalDrivenSeed && pt >= 15 && abs(eta) < 2.5"),
)

if runOnMC: 
    process.preElectronSequence = cms.Sequence(
        process.patElectronId +
        process.patTrigger + 
        process.prunedGen * 
        process.electronMatch 
        )
else:
    removeMCMatching(process, ['Electrons'])
    process.preElectronSequence = cms.Sequence(
        process.patElectronId +
        process.patTrigger 
        )

#the electron sequence
process.elSeq = cms.Sequence( 
    process.preElectronSequence * 
    process.patElectrons *
    (process.eleTriggerMatchHLT + process.eleIdTriggerMatchHLT) *
    process.patElectronsWithTrigger *
    process.selElectrons	
    )



# 
# di-lepton objects
#FIXME: remove the cut on the sign
# FIXME: define a mass window
process.zMMCand = cms.EDProducer("CandViewShallowCloneCombiner",
                                 decay = cms.string('selMuons selMuons'),
                                 cut = cms.string('mass > 30 && daughter(0).pt > 0 && daughter(1).pt > 0'),
                                 checkCharge = cms.bool(False)
)
process.zEECand = cms.EDProducer("CandViewShallowCloneCombiner",
                                 decay = cms.string('selElectrons selElectrons'),
                                 cut = cms.string('mass > 30 && daughter(0).pt > 0 && daughter(1).pt > 0'),
                                 checkCharge = cms.bool(False)
                                 )

process.emuCand = cms.EDProducer("CandViewShallowCloneCombiner",
                                 decay = cms.string('selElectrons selMuons'),
                                 cut = cms.string('mass > 0 && daughter(0).pt > 0 && daughter(1).pt > 0'),
                                 checkCharge = cms.bool(False)
                                 )

#FIXME: cut on the # of dimuon candidates
process.zMMCandCountFilter = cms.EDFilter("CandViewCountFilter", src = cms.InputTag("zMMCand"), minNumber = cms.uint32(1))
process.zEECandCountFilter = cms.EDFilter("CandViewCountFilter", src = cms.InputTag("zEECand"), minNumber = cms.uint32(1))
process.emuCandCountFilter = cms.EDFilter("CandViewCountFilter", src = cms.InputTag("emuCand"), minNumber = cms.uint32(1))

# FIXME: check this
process.muonCandSequence = cms.Sequence(
    process.zMMCand + 
    process.zMMCandCountFilter
    )
process.electronCandSequence = cms.Sequence(
    process.zEECand + 
    process.zEECandCountFilter
    )
process.emuCandSequence = cms.Sequence(
    process.emuCand + 
    process.emuCandCountFilter
    )


# 
# Jets: prepare FastJet sequence for pileup corrections
#
process.load('JetMETCorrections.Configuration.DefaultJEC_cff')
process.load('RecoJets.Configuration.RecoPFJets_cff')
process.kt6PFJets.doRhoFastjet = True
process.kt6PFJets.Ghost_EtaMax = cms.double(5.0) 
process.kt6PFJets.Rho_EtaMax = cms.double(5.0)
process.ak5PFJets.doAreaFastjet = True
process.ak5PFJets.Ghost_EtaMax = cms.double(5.0)  
process.ak5PFJets.Rho_EtaMax = cms.double(5.0)
process.fjSequence = cms.Sequence(process.kt6PFJets+process.ak5PFJets)

from PhysicsTools.PatAlgos.tools.jetTools import *
fjJECLevels=['L1FastJet', 'L2Relative', 'L3Absolute']
osJECLevels=['L1Offset', 'L2Relative', 'L3Absolute']
if( not runOnMC ):
    fjJECLevels.extend(['L2L3Residual'])
    osJECLevels.extend(['L2L3Residual'])
    
process.patJetCorrFactors.levels = fjJECLevels
process.patJetCorrFactors.rho = cms.InputTag('kt6PFJets','rho')

from PhysicsTools.PatAlgos.tools.jetTools import *
if(not runOnMC):  removeMCMatching(process,['All'])
switchJetCollection(process,cms.InputTag('ak5PFJets'),
                    doJTA        = True,
                    doBTagging   = True,
                    jetCorrLabel = ('AK5PF', cms.vstring(fjJECLevels)),
                    doType1MET   = False,
                    genJetCollection=cms.InputTag("ak5GenJets"),
                    doJetID      = True
                    )
process.patJets.embedCaloTowers = cms.bool(False)
process.patJets.embedPFCandidates = cms.bool(True)
addJetCollection(process,cms.InputTag('ak5PFJets'),
                 'AK5','Offset',
                 doJTA = True,
                 doBTagging = True,
                 jetCorrLabel = ('AK5PF', cms.vstring(osJECLevels)),
                 doType1MET = False,
                 doL1Cleaning = False,
                 doL1Counters = True,
                 genJetCollection=cms.InputTag("ak5GenJets"),
                 doJetID = True,
                 jetIdLabel = "ak5"
                 )

process.selectedPatJets.cut = cms.string("pt >= 15 && abs(eta) < 2.5")

from PhysicsTools.PatAlgos.tools.metTools import *
addTcMET(process, 'TC')
addPfMET(process, 'PF')
if(not runOnMC ):
    process.patJetsAK5Offset.addGenPartonMatch = False
    process.patJetsAK5Offset.addGenJetMatch = False
    process.patJetsAK5Offset.addPartonJetMatch = False
    process.patJetsAK5Offset.getJetMCFlavour = False


# =================================================================
# define the paths
#
process.startSeq = cms.Sequence(
    process.startCounter*
    process.runVertexing*
    process.preFilter*
    process.preFilterCounter*
    process.fjSequence
    )
process.eePath = cms.Path(process.startSeq * process.elSeq + process.electronCandSequence + process.patDefaultSequence + process.eeCounter)
process.mumuPath = cms.Path(process.startSeq * process.muSeq + process.muonCandSequence + process.patDefaultSequence + process.mumuCounter)
process.emuPath = cms.Path(process.startSeq * process.emuCandSequence + process.patDefaultSequence + process.emuCounter)
process.e = cms.EndPath( process.endCounter*process.out )
process.schedule = cms.Schedule( process.eePath, process.mumuPath, process.emuPath,process.e )

# ======================================================================================
# configure the output
#
process.load('Configuration.EventContent.EventContent_cff')
from PhysicsTools.PatAlgos.patEventContent_cff import patEventContentNoCleaning, patExtraAodEventContent
process.out.SelectEvents=cms.untracked.PSet( SelectEvents = cms.vstring('eePath', 'mumuPath', 'emuPath') )
process.out.outputCommands = cms.untracked.vstring('drop *',
                                                   'keep *_MEtoEDMConverter_*_*',
                                                   'keep *_*_HBHENoise*_*',
                                                   ##### MC
                                                   'keep *_prunedGen_*_*',
                                                   'keep *_genMetTrue_*_*',
                                                   #### this contain the counter after each filter you want to consider
                                                   #### the genFilter, the goodCollision cleaning, the Skimming
                                                   'keep edmMergeableCounter_*_*_*',
                                                   ##### all the muons and all the electrons
                                                   'keep *_patMuonsWithTrigger_*_*',
                                                   'keep *_patElectronsWithTrigger_*_*',
                                                   ##### the 2 leptons candidate I build
                                                   'keep *_zEECand_*_*',
                                                   'keep *_zMMCand_*_*',
                                                   'keep *_emuCand_*_*',
                                                   ##### MET
                                                   'keep *_tcMet_*_*',
                                                   'keep *_pfMet_*_*',
                                                   ##### Tracking
                                                   'keep *_offlinePrimaryVertices_*_*',
                                                   'keep *_offlinePrimaryVerticesDA_*_*',
                                                   'keep *_offlinePrimaryVerticesWithBS_*_*',
                                                   'keep *_offlineBeamSpot_*_*',
                                                   #'keep *_goodTracks_*_*',
                                                   'keep *_generalTracks_*_*',
                                                   ###### Trigger
                                                   #'keep *_hltTriggerSummaryAOD_*_*',
                                                   'keep *_TriggerResults_*_*',
                                                   ###### PileUp
                                                   'keep *_addPileupInfo_*_*',
                                                   ######Taus
                                                   'keep *_selectedPatTaus_*_*',
                                                   'keep *_genEventScale_*_*',
                                                   'keep GenRunInfoProduct_*_*_*',
                                                   'keep *_flavorHistoryFilter_*_*',
                                                   'keep recoGenJets_ak5GenJets_*_*',
                                                   'keep double*_*_rho_'+process.name_(),
                                                   'keep double*_*_sigma_'+process.name_(),
                                                   'keep recoPFCandidates_particleFlow_*_*',
                                                   'keep *_selectedPat*_*_*',
                                                   'keep patMETs_*_*_*'
                                                   )
#process.out.outputCommands.extend( patEventContentNoCleaning )
#process.out.outputCommands.extend( patExtraAodEventContent )
process.out.fileName = cms.untracked.string('patTuple.root')
process.outpath = cms.EndPath(process.out)

#
# message logger
#
process.MessageLogger.cerr.FwkReport.reportEvery = 5000

print "[zzllvvPattuple] scheduling the following processes"
print process.schedule
print "[zzllvvPattuple] events will be selected for the following paths:"
print process.out.SelectEvents.SelectEvents

#
# that's all
#
