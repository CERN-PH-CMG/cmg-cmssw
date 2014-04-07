import FWCore.ParameterSet.Config as cms

process = cms.Process("COPY")


from CMGTools.Production.datasetToSource import *
process.source = datasetToSource(
    'cmgtools',
    '/VBF_HToTauTau_M-125_8TeV-powheg-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/PAT_CMG_V5_14_0',
    'cmgTuple.*root',
    )

process.source.eventsToProcess = cms.untracked.VEventRange("1:351:105104")

process.options   = cms.untracked.PSet( wantSummary = cms.untracked.bool(False))
#WARNING!
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

process.out = cms.OutputModule(
    "PoolOutputModule",
    outputCommands = cms.untracked.vstring('keep *'),
    fileName = cms.untracked.string('selected.root'),
    )

#from PFAnalyses.TTBar.topDecayFilters_cff import topFilterEventContent
#process.out.outputCommands.extend( topFilterEventContent)


process.endpath = cms.EndPath(
    process.out
    )



process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = 1000
