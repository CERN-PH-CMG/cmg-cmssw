import FWCore.ParameterSet.Config as cms

process = cms.Process("COPY")

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
    'file:h2TauTau_tree_CMG.root'
    )
)

process.options   = cms.untracked.PSet( wantSummary = cms.untracked.bool(False))
#WARNING!
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )


from CMGTools.H2TauTau.eventContent.tauMu_cff import tauMu as tauMuEventContent
process.out = cms.OutputModule(
    "PoolOutputModule",
    outputCommands = tauMuEventContent,
    fileName = cms.untracked.string('testEventContent.root'),
    # fileName = cms.untracked.string('AODSIM.root'),
    )

#from PFAnalyses.TTBar.topDecayFilters_cff import topFilterEventContent
#process.out.outputCommands.extend( topFilterEventContent)



process.endpath = cms.EndPath(
    process.out
    )


process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = 1000
