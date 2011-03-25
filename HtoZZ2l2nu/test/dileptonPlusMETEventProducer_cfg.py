import FWCore.ParameterSet.Config as cms

process = cms.Process("HtoZZto2l2nu")

from CMGTools.HtoZZ2l2nu.localPatTuples_cff import *
process.source = cms.Source("PoolSource",
                            fileNames = getLocalSourceFor('GluGluToHToZZTo2L2NuM400')
#                            fileNames = cms.untracked.vstring('file:patTuple.root')
                            )
import sys
if(len(sys.argv)>2 ):
    process.source.fileNames = getLocalSourceFor( sys.argv[2] )

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

process.load('CMGTools.HtoZZ2l2nu.NormalizationCounter_cfi')
process.load('CMGTools.HtoZZ2l2nu.CleanEventProducer_cfi')
process.load('CMGTools.HtoZZ2l2nu.CleanEventFilter_cfi')
process.load('CMGTools.HtoZZ2l2nu.CleanEventAnalyzer_cfi')

process.out = cms.OutputModule("PoolOutputModule",
                               fileName = cms.untracked.string('/tmp/evHyp.root')
                               )
process.p = cms.Path(process.loadNormalizationCounters*process.cleanEvent*process.cleanEventFilter*process.analysis)
process.e = cms.EndPath(process.saveNormalizationCounters*process.out)


# message logger
process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = 500
process.options = cms.untracked.PSet(
    wantSummary = cms.untracked.bool(True),
    SkipEvent = cms.untracked.vstring('ProductNotFound')
    )

