import FWCore.ParameterSet.Config as cms

from CMGTools.Common.Tools.cmsswRelease import isNewerThan


runOnMC      = False
runOld5XGT = False
runOnFastSim = False

process = cms.Process("CMG")

from CMGTools.Production.datasetToSource import *
process.source = datasetToSource(
    'cmgtools',
    '/DoubleMu/Run2011A-16Jan2012-v1/AOD/V5/PAT_CMG_V5_6_0_B'
   )

process.source.fileNames = process.source.fileNames[:10]

## Maximal Number of Events
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(10000) )

print 'loading the main CMG sequence'

process.load('CMGTools.Common.PAT.PATCMG_cff')

process.skim=cms.EDFilter('HZZCMGSkim',
                          nLeptons = cms.int32(3)
                          )

process.cleanedMuons = cms.EDProducer('PATMuonCleanerBySegments',
             src = cms.InputTag("patMuonsWithTrigger"),
             preselection = cms.string("track.isNonnull"),
             passthrough = cms.string("isGlobalMuon && numberOfMatches >= 2"),
             fractionOfSharedSegments = cms.double(0.499))



process.cmgMuon.cfg.inputCollection = 'cleanedMuons'

process.p = cms.Path(process.cleanedMuons+process.cmgMuon+process.cmgMuonSel+process.skim)

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
    "drop *_cmgMuon_*_CMG",                     
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

process.GlobalTag.globaltag = getGlobalTag( runOnMC)
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

