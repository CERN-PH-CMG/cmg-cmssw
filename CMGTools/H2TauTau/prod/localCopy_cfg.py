import FWCore.ParameterSet.Config as cms

process = cms.Process("COPY")



from CMGTools.Production.datasetToSource import *
process.source = datasetToSource(
    'cmgtools',
    '/TTJets_TuneZ2_7TeV-madgraph-tauola/Fall11-PU_S6_START42_V14B-v2/AODSIM/V5/HTTSKIM1/PAT_CMG_V5_2_0',
    'cmgTuple.*root',
    )


process.options   = cms.untracked.PSet( wantSummary = cms.untracked.bool(False))
#WARNING!
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

process.out = cms.OutputModule(
    "PoolOutputModule",
    outputCommands = cms.untracked.vstring('keep *'),
    fileName = cms.untracked.string('cmgTuple.root'),
    )

#from PFAnalyses.TTBar.topDecayFilters_cff import topFilterEventContent
#process.out.outputCommands.extend( topFilterEventContent)


process.endpath = cms.EndPath(
    process.out
    )


process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = 1000
