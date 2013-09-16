from PhysicsTools.PatAlgos.patTemplate_cfg import *
import FWCore.ParameterSet.Config as cms

process.setName_('COUNT')

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
    )

process.maxLuminosityBlocks = cms.untracked.PSet( 
    input = cms.untracked.int32(-1)
    )


process.load("CMGTools.Common.sources.DoubleMu.Run2011A_ZMu_PromptSkim_v6.RAW_RECO.PAT_CMG_V2_3_0.source_cff")


# reading the first 10 files:
nFiles = 10
# print 'WARNING: RESTRICTING INPUT TO THE FIRST', nFiles, 'FILES'
# process.source.fileNames = process.source.fileNames[:nFiles] 

print process.source.fileNames

# output module for EDM event (ntuple)
process.out.fileName = cms.untracked.string('tree_CMG.root')
from CMGTools.Common.eventContent.everything_cff import everything 
process.out.outputCommands = cms.untracked.vstring( 'keep *')

process.load('CMGTools.Common.miscProducers.misc_cff')
process.p = cms.Path( process.miscSequence )

process.schedule = cms.Schedule(
    process.p,
    process.outpath
    )

process.MessageLogger.cerr.FwkReport.reportEvery = 100
process.options = cms.untracked.PSet( wantSummary = cms.untracked.bool(True) ) 
