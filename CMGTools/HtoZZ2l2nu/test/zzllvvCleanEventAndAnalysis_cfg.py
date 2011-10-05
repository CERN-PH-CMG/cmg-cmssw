import FWCore.ParameterSet.Config as cms

process = cms.Process("HtoZZto2l2nu")

process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
process.GlobalTag.globaltag = 'START42_V13::All'
process.load("Configuration.StandardSequences.Geometry_cff")
process.load("Configuration.StandardSequences.MagneticField_cff")


from CMGTools.HtoZZ2l2nu.localPatTuples_cff import configureFromCommandLine
process.source = cms.Source("PoolSource",
                            fileNames = cms.untracked.vstring()
                            )
castorDir, outputFile, process.source.fileNames = configureFromCommandLine()
from CMGTools.HtoZZ2l2nu.GeneratorLevelSequences_cff import addHiggsPtReweighting
needsPtReweight=addHiggsPtReweighting(process,castorDir)

from CMGTools.HtoZZ2l2nu.ClusteredPFMetProducer_cfi import ClusteredPFMetProducer
process.ClusteredPFMetProducer = ClusteredPFMetProducer.clone()
process.ClusteredPFMetProducerPt5 = ClusteredPFMetProducer.clone( minJetPt = cms.double(5.0) )
process.ClusteredPFMetProducerPt10 = ClusteredPFMetProducer.clone( minJetPt = cms.double(10.0) )
process.ClusteredPFMetSequence = cms.Sequence(process.ClusteredPFMetProducer*process.ClusteredPFMetProducerPt5*process.ClusteredPFMetProducerPt10)

process.load('CMGTools.HtoZZ2l2nu.CleanEventProducer_cfi')
process.load('CMGTools.HtoZZ2l2nu.CleanEventFilter_cfi')
process.load('CMGTools.HtoZZ2l2nu.PileupNormalizationProducer_cfi')
process.load('CMGTools.HtoZZ2l2nu.CleanEventAnalyzer_cfi')
process.TFileService = cms.Service("TFileService", fileName = cms.string(outputFile) )

if(needsPtReweight) :
    #process.p = cms.Path(process.ClusteredPFMetSequence*process.hkfactorSequence*process.puWeights*process.cleanEvent*process.cleanEventFilter*process.evAnalyzer)
    process.p = cms.Path(process.ClusteredPFMetSequence*process.hkfactorSequence*process.puWeights*process.cleanEvent*process.evAnalyzer)
else :
    #process.p = cms.Path(process.ClusteredPFMetSequence*process.puWeights*process.cleanEvent*process.cleanEventFilter*process.evAnalyzer)
    process.p = cms.Path(process.ClusteredPFMetSequence*process.puWeights*process.cleanEvent*process.evAnalyzer)

# message logger
process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = 500
process.options = cms.untracked.PSet(
    wantSummary = cms.untracked.bool(True),
    SkipEvent = cms.untracked.vstring('ProductNotFound')
    )

