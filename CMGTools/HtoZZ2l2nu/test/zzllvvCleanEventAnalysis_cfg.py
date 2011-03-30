import FWCore.ParameterSet.Config as cms

process = cms.Process("HtoZZto2l2nuAN")

from CMGTools.HtoZZ2l2nu.localPatTuples_cff import fillFromCastor
process.source = cms.Source("PoolSource",
                            fileNames = fillFromCastor('/castor/cern.ch/user/p/psilva/Dileptons/TTJetsmadgraph/')
                            )
process.load('CMGTools.HtoZZ2l2nu.CleanEventAnalyzer_cfi')
process.TFileService = cms.Service("TFileService", fileName = cms.string('Histograms.root') )
process.p = cms.Path(process.evAnalyzer)

# message logger
process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = 500
process.options = cms.untracked.PSet(
    wantSummary = cms.untracked.bool(True),
    SkipEvent = cms.untracked.vstring('ProductNotFound')
    )

