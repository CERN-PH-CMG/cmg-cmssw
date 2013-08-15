import FWCore.ParameterSet.Config as cms

from CMGTools.Common.Tools.cmsswRelease import isNewerThan


runOnMC      = False
process = cms.Process("CMG")


process.RandomNumberGeneratorService = cms.Service("RandomNumberGeneratorService",
  sourceSeed = cms.untracked.uint32(414000),
  moduleSeeds = cms.PSet(
    calibratedElectrons = cms.untracked.uint32(1041963),
  )
)


from CMGTools.Production.datasetToSource import *

datasetInfo = (
    'cmgtools',
    '/DoubleElectron/Run2012A-22Jan2013-v1/AOD/PAT_CMG_V5_15_0',
    '.*root')
process.source = datasetToSource(
    *datasetInfo
    )

## Maximal Number of Events
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

print 'loading the main CMG sequence'

process.load('CMGTools.Common.PAT.PATCMG_cff')

process.skim=cms.EDFilter('HZZCMGSkim',
                          nLeptons = cms.int32(3)
                          )

process.correctedMuons = cms.EDProducer("MuScleFitPATMuonCorrector", 
                             src = cms.InputTag("patMuonsWithTrigger"), 
                             debug = cms.bool(False), 
                             identifier = cms.string("Data2012_53X_ReReco"), 
                             applySmearing = cms.bool(runOnMC), 
                             fakeSmearing = cms.bool(False)
)


process.cleanedMuons = cms.EDProducer('PATMuonCleanerBySegments',
             src = cms.InputTag("correctedMuons"),
             preselection = cms.string("track.isNonnull"),
             passthrough = cms.string("isGlobalMuon && numberOfMatches >= 2"),
             fractionOfSharedSegments = cms.double(0.499))


process.cmgMuon.cfg.inputCollection = 'cleanedMuons'

process.goodPrimaryVertices = cms.EDFilter("VertexSelector",
                                             src = cms.InputTag("offlinePrimaryVertices"),
                                             cut = cms.string("!isFake && ndof > 4 && abs(z) <= 24 && position.Rho <= 2"),
                                             filter = cms.bool(True),
                                           )

process.patElectronsWithRegression = cms.EDProducer("RegressionEnergyPatElectronProducer",
       debug = cms.untracked.bool(False),
       inputElectronsTag = cms.InputTag('patElectronsWithTrigger'),
       inputCollectionType = cms.uint32(1),
       useRecHitCollections = cms.bool(False),
       produceValueMaps = cms.bool(False),
       regressionInputFile = cms.string("EgammaAnalysis/ElectronTools/data/eleEnergyRegWeights_WithSubClusters_VApr15.root"),
       energyRegressionType = cms.uint32(2),
       rhoCollection = cms.InputTag('kt6PFJets:rho:RECO'),
       vertexCollection = cms.InputTag('goodPrimaryVertices'),
       nameEnergyReg = cms.string("eneRegForGsfEle"),
       nameEnergyErrorReg = cms.string("eneErrorRegForGsfEle"),
       recHitCollectionEB = cms.InputTag('reducedEcalRecHitsEB'),
       recHitCollectionEE = cms.InputTag('reducedEcalRecHitsEE')
)


process.calibratedElectrons = cms.EDProducer("CalibratedPatElectronProducer",
    inputPatElectronsTag = cms.InputTag("patElectronsWithRegression"),
    isMC = cms.bool(runOnMC),
    verbose = cms.bool(False),
    synchronization = cms.bool(False),
    updateEnergyError = cms.bool(True),
    applyLinearityCorrection = cms.bool(True),
    correctionsType = cms.int32(2),
    combinationType = cms.int32(3),
    lumiRatio = cms.double(1.0),
    inputDataset = cms.string("22Jan2013ReReco"),
    combinationRegressionInputPath = cms.string("EgammaAnalysis/ElectronTools/data/eleEnergyRegWeights_WithSubClusters_VApr15.root"),
    scaleCorrectionsInputPath = cms.string("EgammaAnalysis/ElectronTools/data/scalesNewReg-May2013.csv"),
    linearityCorrectionsInputPath = cms.string("EgammaAnalysis/ElectronTools/data/linearityNewReg-May2013.csv")
    
)

#Jet recalibrator

process.cmgPFJetSel = cms.EDProducer(
    "PFJetCorrector",
    # make sure your jet and rho collections are compatible
    src = cms.InputTag( 'cmgPFJetSel' ),
    vertices = cms.InputTag( 'offlinePrimaryVertices' ),
    rho = cms.InputTag( 'kt6PFJets:rho:RECO' ),
    payload = cms.string('AK5PF'),
    levels = cms.vstring('L1FastJet','L2Relative','L3Absolute'),
    sort = cms.bool(True),
    verbose = cms.untracked.bool( False )
)


if not runOnMC:
    process.cmgPFJetSel.levels = ['L1FastJet','L2Relative','L3Absolute','L2L3Residual']

                                                 
process.cmgElectron.cfg.inputCollection = 'calibratedElectrons'

process.genInfo = cms.EDProducer('HZZGenEmbedder')

process.p = cms.Path(process.genInfo+\
                     process.goodPrimaryVertices+\
                     process.correctedMuons+\
                     process.cleanedMuons+\
                     process.cmgMuon+\
                     process.cmgMuonSel+\
                     process.patElectronsWithRegression+process.calibratedElectrons+\
                     process.cmgElectron+process.cmgElectronSel+process.skim+process.cmgPFJetSel)

########################################################
## CMG output definition
########################################################

process.outcmg = cms.OutputModule(
    "PoolOutputModule",
    fileName = cms.untracked.string('skim.root'),
    SelectEvents   = cms.untracked.PSet( SelectEvents = cms.vstring('p') ),
    outputCommands = cms.untracked.vstring(
    'keep *_*_*_*',
    'drop *_cmgPFBaseJetLeadCHS_*_*',
    'drop *_cmgDiElectronSel_*_*',
    'drop *_cmgDiMuonSel_*_*',
    'drop *_cmgPFJetLooseJetIdFailed_*_*',
    'drop *_cmgPFJetMediumJetIdFailed_*_*',
    'drop *_cmgPFJetSelCHS_*_*',
    'drop *_cmgPFJetSel_*_*',
    'keep *_cmgPFJetSel_*_CMG',
    "drop *_cmgPFJetTightJetIdFailed_*_*" , 
    "drop *_cmgPFJetVeryLooseJetId95Failed_*_*", 
    "drop *_cmgPFJetVeryLooseJetId95gammaFailed_*_*", 
    "drop *_cmgPFJetVeryLooseJetId95h0Failed_*_*",  
    "drop *_cmgPFJetVeryLooseJetId99Failed_*_*",  
    "drop *_cmgStructuredPFJetSel_*_*",         
    "drop *_cmgTauSel_*_*",                     
    "drop *_cmgMuonSel_*_PAT",                     
    "drop *_patMuonsWithTrigger_*_PAT",                     
    "drop *_patElectronsWithTrigger_*_PAT",                     
    "drop *_patElectronsWithRegression_*_PAT",                     
    "drop *_cmgMuon_*_CMG",                     
    "drop *_cmgElectron_*_CMG",                     
    "drop *_correctedMuons_*_CMG",                     
    "drop *_tauGenJetsSelectorAllHadrons_*_*"
    ),
    dropMetaData = cms.untracked.string('PRIOR')
    )

process.outpath = cms.EndPath(process.outcmg)




########################################################
## Conditions 
########################################################

process.load("Configuration.StandardSequences.GeometryDB_cff")
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
process.load("Configuration.StandardSequences.MagneticField_38T_cff")

from CMGTools.Common.Tools.getGlobalTag import getGlobalTagByDataset
process.GlobalTag.globaltag = getGlobalTagByDataset( runOnMC, datasetInfo[1])
print 'Global tag       : ', process.GlobalTag.globaltag


## MessageLogger
process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = 10

## Options and Output Report
process.options   = cms.untracked.PSet( wantSummary = cms.untracked.bool(True) )

print 'starting CMSSW'

