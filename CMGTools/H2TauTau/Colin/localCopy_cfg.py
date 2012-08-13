import FWCore.ParameterSet.Config as cms

process = cms.Process("COPY")


from CMGTools.Production.datasetToSource import *
process.source = datasetToSource(
    'cbern',
    '/VBF_HToTauTau_M-125_8TeV-powheg-pythia6/Summer12-PU_S7_START52_V9-v1/AODSIM/V5/PAT_CMG_V5_5_1/TAUMU_ColinAug13_B',
    '.*root',
    )

# josh - me
# process.source.eventsToProcess = cms.untracked.VEventRange("1:2167:649810", "1:2520:755879", "1:1629:488453", "1:2373:711618", "1:2226:667654", "1:3093:927510", "1:907:272025", "1:2890:866757")
# me - josh
process.source.eventsToProcess = cms.untracked.VEventRange("1:1974:591991", "1:3076:922447", "1:246:73768", "1:351:105104")


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
