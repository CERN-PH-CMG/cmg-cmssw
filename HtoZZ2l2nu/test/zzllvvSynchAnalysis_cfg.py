import FWCore.ParameterSet.Config as cms

process = cms.Process("HtoZZto2l2nu")

from CMGTools.HtoZZ2l2nu.localPatTuples_cff import configureFromCommandLine
process.source = cms.Source("PoolSource",
                            fileNames = cms.untracked.vstring()
                            )
dtag, process.source.fileNames, outputFile = configureFromCommandLine(process)
print process.source.fileNames

process.load('CMGTools.HtoZZ2l2nu.CleanEventProducer_cfi')
from CMGTools.HtoZZ2l2nu.SynchSelections_cfi import *
process.cleanEvent.Generator = BaseGeneratorSelection.clone()
process.cleanEvent.Vertices = BaseVertexSelection.clone()
process.cleanEvent.Muons = BaseMuonsSelection.clone()
process.cleanEvent.Electrons = BaseElectronsSelection.clone()
process.cleanEvent.Dileptons = BaseDileptonSelection.clone()
process.cleanEvent.Jets = BaseJetSelection.clone()
process.cleanEvent.MET = BaseMetSelection.clone()
process.load('CMGTools.HtoZZ2l2nu.CleanEventFilter_cfi')
process.evAnalyzer = cms.EDAnalyzer("ZZ2l2nuCommonAnalyzer",
                                    Generator = BaseGeneratorSelection.clone(),
                                    Vertices = BaseVertexSelection.clone(),
                                    Muons = BaseMuonsSelection.clone(),
                                    Electrons = BaseElectronsSelection.clone(),
                                    Dileptons = BaseDileptonSelection.clone(),
                                    Jets = BaseJetSelection.clone(),
                                    MET = BaseMetSelection.clone()
                                    )
process.evAnalyzer.dtag=cms.string(dtag)
process.TFileService = cms.Service("TFileService", fileName = cms.string(outputFile) )
process.p = cms.Path(process.cleanEvent*process.cleanEventFilter*process.evAnalyzer)


# message logger
process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = 500
process.options = cms.untracked.PSet(
    wantSummary = cms.untracked.bool(True),
    SkipEvent = cms.untracked.vstring('ProductNotFound')
    )

