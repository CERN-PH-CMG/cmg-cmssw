# cfg to merge root files together

import FWCore.ParameterSet.Config as cms

process = cms.Process("COPY")


from CMGTools.Production.datasetToSource import *
process.source = datasetToSource(
    'cmgtools',
    '/W2Jets_TuneZ2_7TeV-madgraph-tauola/Fall11-PU_S6_START42_V14B-v1/AODSIM/V5_B/PAT_CMG_V5_6_0_B/TAUELE_2011_OK_ColinOct31',
    'tauEle_fullsel.*root') 
# process.source = cms.Source("PoolSource",
#                            fileNames = cms.untracked.vstring() )


process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )
process.source.fileNames = process.source.fileNames[:12]
print process.source.fileNames

process.options   = cms.untracked.PSet( wantSummary = cms.untracked.bool(False))


process.out = cms.OutputModule(
    "PoolOutputModule",
    outputCommands =  cms.untracked.vstring(
    'keep *',
    ),
    fileName = cms.untracked.string('tauEle_fullsel_tree.root'),
    )

process.endpath = cms.EndPath(
    process.out
    )


process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = 1000
