import FWCore.ParameterSet.Config as cms

process = cms.Process("COPY")

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
    'file:gen.root'
    )
)

process.options   = cms.untracked.PSet( wantSummary = cms.untracked.bool(False))
#WARNING!
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )


process.load("Configuration.EventContent.EventContent_cff")
process.out = cms.OutputModule(
    "PoolOutputModule",
    # process.RECOSIMEventContent,
    outputCommands =  cms.untracked.vstring(
      'drop recoPFCandidates_particleFlow__GEN'
      ),
    fileName = cms.untracked.string('genDrop.root'),
    # fileName = cms.untracked.string('AODSIM.root'),
    )

#from PFAnalyses.TTBar.topDecayFilters_cff import topFilterEventContent
#process.out.outputCommands.extend( topFilterEventContent)



process.endpath = cms.EndPath(
    process.out
    )


process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = 100
