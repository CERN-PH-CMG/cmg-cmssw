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

from CMGTools.HtoZZ2l2nu.PileupNormalizationProducer_cfi import puWeights
process.puWeights      = puWeights.clone( data = cms.string('/afs/cern.ch/user/p/psilva/public/Pileup/PileupTruth2011AplusB.root') )
process.puWeights2011A = puWeights.clone( data = cms.string('/afs/cern.ch/user/p/psilva/public/Pileup/PileupTruth2011A.root') )
process.puWeights2011B = puWeights.clone( data = cms.string('/afs/cern.ch/user/p/psilva/public/Pileup/PileupTruth2011B.root') )
process.puWeightSequence = cms.Sequence(process.puWeights+process.puWeights2011A+process.puWeights2011B)

process.load('CMGTools.HtoZZ2l2nu.CleanEventAnalyzer_cfi')
process.TFileService = cms.Service("TFileService", fileName = cms.string(outputFile) )

if(needsPtReweight) :
    process.p = cms.Path(process.ClusteredPFMetSequence*process.hkfactorSequence*process.puWeightSequencet*process.evAnalyzer)
else :
    process.p = cms.Path(process.ClusteredPFMetSequence*process.puWeightSequence*process.evAnalyzer)

# message logger
process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = 500
process.options = cms.untracked.PSet(
    wantSummary = cms.untracked.bool(True),
    SkipEvent = cms.untracked.vstring('ProductNotFound')
    )

