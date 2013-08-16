from PhysicsTools.PatAlgos.patTemplate_cfg import *
import FWCore.ParameterSet.Config as cms

process.setName_('SKIM')

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
    )

process.maxLuminosityBlocks = cms.untracked.PSet( 
    input = cms.untracked.int32(-1)
    )


process.load("CMGTools.Common.sources.SingleMu.Run2011A_May10ReReco_v1.AOD.V2.PAT_CMG_V2_3_0.CollectionCounting.source_cff")


# reading the first 10 files:
nFiles = 10
# print 'WARNING: RESTRICTING INPUT TO THE FIRST', nFiles, 'FILES'
# process.source.fileNames = process.source.fileNames[:nFiles] 

print process.source.fileNames

# output module for EDM event (ntuple)
process.out.fileName = cms.untracked.string('tree_CMG.root')
from CMGTools.Common.eventContent.everything_cff import everything 
process.out.outputCommands = cms.untracked.vstring( 'keep *')


process.load('CMGTools.Common.analysis_cff')
process.cmgMuonSel.src = 'cmgMuonSel'
process.cmgMuonSel.cut = 'pt()>20'
process.cmgMuonCount.minNumber = 1

process.p = cms.Path(
    process.cmgMuonSel +
    process.cmgMuonCount
    )


process.schedule = cms.Schedule(
    process.p,
    process.outpath
    )

process.MessageLogger.cerr.FwkReport.reportEvery = 100
process.options = cms.untracked.PSet( wantSummary = cms.untracked.bool(True) ) 
