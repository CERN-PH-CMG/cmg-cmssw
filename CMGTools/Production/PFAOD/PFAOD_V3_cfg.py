import FWCore.ParameterSet.Config as cms
import sys 
import os 

process = cms.Process("PFAOD")


process.source = cms.Source(
    "PoolSource",
    fileNames = cms.untracked.vstring(
    '/store/data/Run2011B/HT/AOD/PromptReco-v1/000/175/834/E0B1CFCC-B8DB-E011-9062-BCAEC518FF7C.root'
    )
    )


process.options   = cms.untracked.PSet( wantSummary = cms.untracked.bool(False))
#WARNING!
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

process.load("Configuration.EventContent.EventContent_cff")
process.out = cms.OutputModule(
    "PoolOutputModule",
    process.AODSIMEventContent,
    fileName = cms.untracked.string( 'PFAOD.root' ),
    )

from CMGTools.Production.PFAOD.PFAOD_EventContent_cff import V3
process.out.outputCommands.extend( V3 )

process.endpath = cms.EndPath(
    process.out
    )


process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = 100


