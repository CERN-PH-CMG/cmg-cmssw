import FWCore.ParameterSet.Config as cms
import os, sys

process = cms.Process("TOP2")

runOnData = False

#sample = 'ttbar'
sample = 'data'

# Message logger setup.
process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = 500
process.options = cms.untracked.PSet(wantSummary = cms.untracked.bool(True))

# source
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(10000) )

process.maxLuminosityBlocks = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
    )

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring('/store/cmst3/user/jueugste/nTuples/TTJets_MassiveBinDECAY_treeCMG_leptonJetsSkim_9.root'),
    #fileNames = cms.untracked.vstring('file:/afs/cern.ch/user/p/palencia/work/cmg/V5_10_0/CMGTools/CMSSW_5_3_3_patch3/src/UserCode/TopMass/test/ttbar_treeCMG_lJetsSel.root'),
    #fileNames = cms.untracked.vstring('file:/afs/cern.ch/user/p/palencia/work/cmg/V5_10_0/CMGTools/CMSSW_5_3_3_patch3/src/UserCode/TopMass/test/muDataRun2012B_treeCMG_lJetsSel.root'),
)
 

print "\n Processing files:"
print process.source.fileNames

print "\n Processing events:"
print process.maxEvents



process.ana = cms.EDAnalyzer('TtbarLeptonJets')

process.TFileService = cms.Service("TFileService",
    fileName = cms.string(sample+'Histos.root')
    )

process.p = cms.Path( process.ana )
   

######################################################################



