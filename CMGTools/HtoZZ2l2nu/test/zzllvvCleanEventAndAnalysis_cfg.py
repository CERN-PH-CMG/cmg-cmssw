import FWCore.ParameterSet.Config as cms

process = cms.Process("HtoZZto2l2nu")

from CMGTools.HtoZZ2l2nu.localPatTuples_cff import configureFromCommandLine
process.source = cms.Source("PoolSource",
                            fileNames = cms.untracked.vstring()
                            )
castorDir, outputFile, process.source.fileNames = configureFromCommandLine()
from CMGTools.HtoZZ2l2nu.GeneratorLevelSequences_cff import addHiggsPtReweighting
needsPtReweight=addHiggsPtReweighting(process,castorDir)

process.load('CMGTools.HtoZZ2l2nu.CleanEventProducer_cfi')
process.load('CMGTools.HtoZZ2l2nu.CleanEventFilter_cfi')
process.load('CMGTools.HtoZZ2l2nu.PileupNormalizationProducer_cfi')
process.load('CMGTools.HtoZZ2l2nu.CleanEventAnalyzer_cfi')
process.TFileService = cms.Service("TFileService", fileName = cms.string(outputFile) )

if(needsPtReweight) :
    #process.p = cms.Path(process.hkfactorSequence*process.puWeights*process.cleanEvent*process.cleanEventFilter*process.evAnalyzer)
    process.p = cms.Path(process.hkfactorSequence*process.cleanEvent*process.cleanEventFilter*process.evAnalyzer)
else :
    #process.p = cms.Path(process.puWeights*process.cleanEvent*process.cleanEventFilter*process.evAnalyzer)
    process.p = cms.Path(process.cleanEvent*process.evAnalyzer)

# message logger
process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = 500
process.options = cms.untracked.PSet(
    wantSummary = cms.untracked.bool(True),
    SkipEvent = cms.untracked.vstring('ProductNotFound')
    )

