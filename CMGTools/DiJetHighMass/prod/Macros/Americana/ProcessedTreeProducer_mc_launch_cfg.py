import FWCore.ParameterSet.Config as cms

bQQ = True
bMix = False



process = cms.Process("Ana")
process.load('FWCore.MessageService.MessageLogger_cfi')
##-------------------- Communicate with the DB -----------------------
process.load('Configuration.StandardSequences.Services_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
process.GlobalTag.globaltag = 'GR_R_42_V12::All'
process.load('Configuration.StandardSequences.MagneticField_38T_cff')
process.load('Configuration.StandardSequences.Geometry_cff')
process.load('RecoJets.Configuration.RecoPFJets_cff')
process.load('RecoJets.Configuration.GenJetParticles_cff')
process.load('RecoJets.Configuration.RecoJets_cff')
##-------------------- Import the JEC services -----------------------
process.load('JetMETCorrections.Configuration.DefaultJEC_cff')
#############   Set the number of events #############
process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
)
#############   Format MessageLogger #################
process.MessageLogger.cerr.FwkReport.reportEvery = 1000
#############   Define the source file ###############
#process.source = cms.Source("PoolSource",
#    fileNames = cms.untracked.vstring('/store/mc/Summer11/QCD_Pt-15to3000_TuneZ2_Flat_7TeV_pythia6/AODSIM/PU_S3_START42_V11-v2/0004/FA6FEF7F-8E7E-E011-AC35-001A92971B78.root')
#)

process.load("CommonTools.ParticleFlow.Sources.Resonances.RSGravitonToJJ_M_4000_TuneZ2_7TeV_pythia6.REDIGI.AODSIM.source_cff")
#process.load("CommonTools.ParticleFlow.Sources.Resonances.AODSIM.QstarToJJ_M_3500_TuneD6T_7TeV_pythia6.source_cff")

############# processed tree producer ##################
process.TFileService = cms.Service("TFileService",fileName = cms.string('ProcessedTree_mc.root'))




process.ak7 = cms.EDAnalyzer('ProcessedTreeProducer',
    ## jet collections ###########################
    pfjets          = cms.InputTag('ak7PFJets'),
    calojets        = cms.InputTag('ak7CaloJets'),
    genjets         = cms.untracked.InputTag('ak7GenJets'),
    isMCarlo        = cms.untracked.bool(True),
    ## database entry for the uncertainties ######
    PFPayloadName   = cms.string(''),
    CaloPayloadName = cms.string(''),
    ## calojet ID and extender for the JTA #######
    calojetID       = cms.InputTag('ak7JetID'),
    calojetExtender = cms.InputTag('ak7JetExtender'),
    ## set the conditions for good Vtx counting ##
    offlineVertices = cms.InputTag('offlinePrimaryVertices'),
    goodVtxNdof     = cms.double(4), 
    goodVtxZ        = cms.double(24),
    ## rho #######################################
    srcCaloRho      = cms.InputTag('kt6CaloJets','rho'),
    srcPFRho        = cms.InputTag('kt6PFJets','rho'),
    ## simulated PU ##############################
    srcPU           = cms.untracked.InputTag('addPileupInfo'),
    ## number of jets to be stored ###############
    maxY            = cms.double(5.0), 
    minPFPt         = cms.double(20),
    minPFFatPt      = cms.double(10),
    maxPFFatEta     = cms.double(2.5),
    minCaloPt       = cms.double(20),
    minGenPt        = cms.untracked.double(20),
    minNPFJets      = cms.int32(1),
    minNCaloJets    = cms.int32(1), 
    minJJMass       = cms.double(-1),
    ## trigger ##############################
    printTriggerMenu = cms.untracked.bool(True),
    processName     = cms.string('HLT'),
    triggerName     = cms.vstring('HLT_Jet110'),
    triggerResults  = cms.InputTag("TriggerResults","","HLT"),
    triggerEvent    = cms.InputTag("hltTriggerSummaryAOD","","HLT"),
    ## jec services ##############################
    pfjecService    = cms.string('ak7PFL1FastL2L3'),
    calojecService  = cms.string('ak7CaloL1L2L3')
)

process.ak5 = process.ak7.clone(
    pfjets           = 'ak5PFJets',
    calojets         = 'ak5CaloJets',
    genjets          = 'ak7GenJets',
    PFPayloadName    = '',
    CaloPayloadName  = '',
    calojetID        = 'ak5JetID',
    calojetExtender  = 'ak5JetExtender',
    pfjecService     = 'ak5PFL1FastL2L3',
    calojecService   = 'ak5CaloL1L2L3',
    printTriggerMenu = False 
)

process.ak5QQtoQQ = process.ak7.clone(
    pfjets           = 'ak5PFJets',
    calojets         = 'ak5CaloJets',
    genjets          = 'ak7GenJets',
    PFPayloadName    = '',
    CaloPayloadName  = '',
    calojetID        = 'ak5JetID',
    calojetExtender  = 'ak5JetExtender',
    pfjecService     = 'ak5PFL1FastL2L3',
    calojecService   = 'ak5CaloL1L2L3',
    printTriggerMenu = False 
)


process.ak7QQtoQQ = process.ak7.clone()


process.ak5GGtoGG = process.ak7.clone(
    pfjets           = 'ak5PFJets',
    calojets         = 'ak5CaloJets',
    genjets          = 'ak7GenJets',
    PFPayloadName    = '',
    CaloPayloadName  = '',
    calojetID        = 'ak5JetID',
    calojetExtender  = 'ak5JetExtender',
    pfjecService     = 'ak5PFL1FastL2L3',
    calojecService   = 'ak5CaloL1L2L3',
    printTriggerMenu = False 
)


process.ak7GGtoGG = process.ak7.clone()

process.kt6PFJets.doRhoFastjet = True
process.kt6PFJets.Rho_EtaMax = cms.double(5.0)
process.kt6CaloJets.doRhoFastjet = True
process.kt6CaloJets.Rho_EtaMax = cms.double(5.0)
process.ak7PFJets.doAreaFastjet = True
process.ak7PFJets.Rho_EtaMax = cms.double(5.0)
process.ak5PFJets.doAreaFastjet = True
process.ak5PFJets.Rho_EtaMax = cms.double(5.0)

from KKousour.QCDAnalysis.myAdditionalJetSequence_cff import myAdditionalJetSequence
myAdditionalJetSequence( process )


process.ggFinalState = cms.EDFilter(
    "CandViewSelector",
    src = cms.InputTag("hardPartons"),
    cut = cms.string("pdgId() == 21")
    )
process.filterGGFinalState = cms.EDFilter(
    "CandCountFilter",
    src = cms.InputTag("ggFinalState"),
    minNumber = cms.uint32(2),
    )


process.qqFinalState = cms.EDFilter(
    "CandViewSelector",
    src = cms.InputTag("hardPartons"),
    cut = cms.string("pdgId() != 21")
    )
process.filterQQFinalState = cms.EDFilter(
    "CandCountFilter",
    src = cms.InputTag("qqFinalState"),
    minNumber = cms.uint32(2),
    )




process.ggInitialState = cms.EDFilter(
    "CandViewSelector",
    src = cms.InputTag("hardInitialState"),
    cut = cms.string("pdgId() == 21")
    )

process.filterGGInitialState = cms.EDFilter(
    "CandCountFilter",
    src = cms.InputTag("ggInitialState"),
    minNumber = cms.uint32(2),
    )


process.qqInitialState = cms.EDFilter(
    "CandViewSelector",
    src = cms.InputTag("hardInitialState"),
    cut = cms.string("pdgId() != 21")
    )

process.filterQQInitialState = cms.EDFilter(
    "CandCountFilter",
    src = cms.InputTag("qqInitialState"),
    minNumber = cms.uint32(2),
    )

process.GGtoRtoGG  = cms.Sequence(
    process.ggInitialState+
    process.filterGGInitialState+
    process.ggFinalState+
    process.filterGGFinalState
    )


process.QQtoRtoQQ  = cms.Sequence(
    process.qqInitialState+
    process.filterQQInitialState+
    process.qqFinalState+
    process.filterQQFinalState
    )

process.GGtoRtoQQ  = cms.Sequence(
    process.ggInitialState+
    process.filterGGInitialState+
    process.qqFinalState+
    process.filterQQFinalState
    )

process.QQtoRtoGG  = cms.Sequence(
    process.qqInitialState+
    process.filterQQInitialState+
    process.ggFinalState+
    process.filterGGFinalState
    )


process.p0 = cms.Path(process.partonic*process.kt6PFJets * process.kt6CaloJets * process.ak5PFJets * process.ak7PFJets)

process.pathQQtoQQ = cms.Path(process.QQtoRtoQQ*process.ak5QQtoQQ * process.ak7QQtoQQ)
process.pathGGtoGG = cms.Path(process.GGtoRtoGG*process.ak5GGtoGG * process.ak7GGtoGG)
process.path = cms.Path(process.ak5 * process.ak7)


process.schedule = cms.Schedule(
    process.p0,
    process.pathQQtoQQ,
    process.pathGGtoGG,
    process.path
    )

