import FWCore.ParameterSet.Config as cms

process = cms.Process("COPY2")

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
    'file:AOD.root'
    )
)



process.options   = cms.untracked.PSet( wantSummary = cms.untracked.bool(False))
#WARNING!
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )


process.load("Configuration.EventContent.EventContent_cff")
process.out = cms.OutputModule(
    "PoolOutputModule",
    process.AODEventContent,
#    outputCommands =  cms.untracked.vstring(
#    'keep *'
#    ),
    fileName = cms.untracked.string('AODNoSim.root'),
    )




process.endpath = cms.EndPath(
    process.out
    )


process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = 100
