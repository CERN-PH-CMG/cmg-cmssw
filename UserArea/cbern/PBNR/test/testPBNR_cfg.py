import FWCore.ParameterSet.Config as cms

process = cms.Process("PBNR")

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
    '/store/data/Run2011A/HT/AOD/PromptReco-v4/000/166/921/F277100B-BA97-E011-998D-001D09F24D4E.root'
    )
)

process.load('cbern.PBNR.PBNR_cff')

process.p = cms.Path(
    process.PBNRSequence
    )

process.options   = cms.untracked.PSet( wantSummary = cms.untracked.bool(True))
#WARNING!
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(5000) )

process.load("Configuration.EventContent.EventContent_cff")
process.out = cms.OutputModule(
    "PoolOutputModule",
    outputCommands = cms.untracked.vstring('keep recoPFJets_*_*_*'),
    fileName = cms.untracked.string('PBNR.root'),
    SelectEvents   = cms.untracked.PSet( SelectEvents = cms.vstring('p') )
    )


process.endpath = cms.EndPath(
    process.out
    )


process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = 100


