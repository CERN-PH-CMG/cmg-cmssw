import FWCore.ParameterSet.Config as cms


from PhysicsTools.PatAlgos.selectionLayer1.muonCountFilter_cfi import *


process = cms.Process("analysis")

process.load("FWCore.MessageService.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = cms.untracked.int32(1000)


process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )


process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(LISTOFFILES),
    skipEvents = cms.untracked.uint32(0)                        
)


# muon fileter
process.load('PhysicsTools.PatAlgos.selectionLayer1.muonCountFilter_cfi')

process.MuonsFilter = countPatMuons.clone(
    src       = cms.InputTag("selectedPatMuonsPFlow"),
    minNumber = cms.uint32(2)
)


process.load("CMG.JetIDAnalysis.jetanalyzer_cfi")

process.TFileService = cms.Service("TFileService", 
    fileName = cms.string("OUTPUTFILENAME"),
    closeFileFast = cms.untracked.bool(True)
)

process.p = cms.Path(process.MuonsFilter*process.jetanalyzer)
