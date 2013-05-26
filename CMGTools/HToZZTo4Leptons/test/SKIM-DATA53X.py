
import FWCore.ParameterSet.Config as cms

from CMGTools.Common.Tools.cmsswRelease import isNewerThan


runOnMC      = False
runOld5XGT = False
runOnFastSim = False

process = cms.Process("CMG")


process.RandomNumberGeneratorService = cms.Service("RandomNumberGeneratorService",
  sourceSeed = cms.untracked.uint32(414000),
  moduleSeeds = cms.PSet(
    calibratedElectrons = cms.untracked.uint32(1041963),
  )
)


from CMGTools.Production.datasetToSource import *
process.source = datasetToSource(
    'cmgtools',
    '/DoubleElectron/Run2012C-24Aug2012-v1/AOD/V5_B/PAT_CMG_V5_10_0'
   )


## Maximal Number of Events
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(10) )

print 'loading the main CMG sequence'

process.load('CMGTools.Common.PAT.PATCMG_cff')

process.skim=cms.EDFilter('HZZCMGSkim',
                          nLeptons = cms.int32(3)
                          )



process.correctedMuons = cms.EDProducer('RochesterPATMuonCorrector',
             src = cms.InputTag("patMuonsWithTrigger"))

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
                               inputPatElectronsTag = cms.InputTag('patElectronsWithTrigger'),
                               regressionInputFile = cms.string("EGamma/EGammaAnalysisTools/data/eleEnergyRegWeights_V1.root"),
                               energyRegressionType = cms.uint32(1),
                               rhoCollection = cms.InputTag('kt6PFJets:rho:RECO'),
                               vertexCollection = cms.InputTag('goodPrimaryVertices')
)


process.calibratedElectrons = cms.EDProducer("CalibratedPatElectronProducer",
    inputPatElectronsTag = cms.InputTag("patElectronsWithRegression"),
    isMC = cms.bool(runOnMC),
    isAOD = cms.bool(False),
    debug = cms.bool(False),
    updateEnergyError = cms.bool(True),
    applyCorrections = cms.int32(1),
    inputDataset = cms.string("2012Jul13ReReco"),
    
)

                                                 
process.cmgElectron.cfg.inputCollection = 'calibratedElectrons'
process.genInfo = cms.EDProducer('HZZGenEmbedder')

process.p = cms.Path(process.genInfo+process.goodPrimaryVertices+process.tunepMuons+process.correctedMuons+process.cleanedMuons+process.cmgMuon+process.cmgMuonSel+\
                     process.patElectronsWithRegression+process.calibratedElectrons+\
                     process.cmgElectron+process.cmgElectronSel+process.skim)

########################################################
## CMG output definition
########################################################

process.outcmg = cms.OutputModule(
    "PoolOutputModule",
    fileName = cms.untracked.string('cmgTuple.root'),
    SelectEvents   = cms.untracked.PSet( SelectEvents = cms.vstring('p') ),
    outputCommands = cms.untracked.vstring(
    'keep *_*_*_*',
    'drop *_cmgPFBaseJetLeadCHS_*_*',
    'drop *_cmgDiElectronSel_*_*',
    'drop *_cmgDiMuonSel_*_*',
    'drop *_cmgPFJetLooseJetIdFailed_*_*',
    'drop *_cmgPFJetMediumJetIdFailed_*_*',
    'drop *_cmgPFJetSelCHS_*_*',
    "drop *_cmgPFJetTightJetIdFailed_*_*" , 
    "drop *_cmgPFJetVeryLooseJetId95Failed_*_*", 
    "drop *_cmgPFJetVeryLooseJetId95gammaFailed_*_*", 
    "drop *_cmgPFJetVeryLooseJetId95h0Failed_*_*",  
    "drop *_cmgPFJetVeryLooseJetId99Failed_*_*",  
    "drop *_cmgStructuredPFJetSel_*_*",         
    "drop *_cmgTauSel_*_*",                     
    "drop *_cmgMuonSel_*_PAT",                     
    "drop *_patMuonsWithTrigger_*_PAT",                     
    "drop *_tunepMuons_*_*",                     
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

from CMGTools.Common.Tools.getGlobalTag import getGlobalTag

process.GlobalTag.globaltag = getGlobalTag( runOnMC,runOld5XGT)
print 'Global tag       : ', process.GlobalTag.globaltag

########################################################
## Below, stuff that you probably don't want to modify
########################################################

## MessageLogger
process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = 10

## Options and Output Report
process.options   = cms.untracked.PSet( wantSummary = cms.untracked.bool(True) )

print 'starting CMSSW'

