import FWCore.ParameterSet.Config as cms

process = cms.Process("COPY")



from CMGTools.Production.datasetToSource import *
process.source = datasetToSource(
    'cbern',
    '/TauPlusX/Run2012A-PromptReco-v1/RECO/PAT_CMG_V5_4_0_runrange_190605-194076/TAUMU',
    'tauMu_fullsel.*root',
    )


process.options   = cms.untracked.PSet( wantSummary = cms.untracked.bool(False))
#WARNING!
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

process.out = cms.OutputModule(
    "PoolOutputModule",
    outputCommands = cms.untracked.vstring('keep *'),
    fileName = cms.untracked.string('tauMu_fullsel_tree_CMG.root'),
    )

#from PFAnalyses.TTBar.topDecayFilters_cff import topFilterEventContent
#process.out.outputCommands.extend( topFilterEventContent)


process.endpath = cms.EndPath(
    process.out
    )


process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = 1000
