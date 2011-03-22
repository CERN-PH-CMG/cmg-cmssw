import FWCore.ParameterSet.Config as cms

process = cms.Process("HtoZZto2l2nu")

from CMGTools.HtoZZ2l2nu.localPatTuples_cff import *
process.source = cms.Source("PoolSource",
                            fileNames = GluGluToHToZZTo2L2NuM400
                            )
print process.source.fileNames
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

from CMGTools.Common.runInfoCounter_cfi import runInfoCounter
process.cmgRunInfoCounter=runInfoCounter.clone()

process.load('CMGTools.HtoZZ2l2nu.NormalizationCounter_cfi')

from CMGTools.HtoZZ2l2nu.StandardSelections_cfi import *
process.cleanEvent = cms.EDProducer("DileptonPlusMETEventProducer",
                                    Vertices = BaseVertexSelection.clone(),
                                    Muons = BaseMuonsSelection.clone(),
                                    Electrons = BaseElectronsSelection.clone(),
                                    Dileptons = BaseDileptonSelection.clone(),
                                    Jets = BaseJetSelection.clone(),
                                    MET = BaseMetSelection.clone()
                                    )

process.out = cms.OutputModule("PoolOutputModule",
                               outputCommands = cms.untracked.vstring('drop *', 
                                                                      'keep *_*_*_HtoZZto2l2nu'),
                               fileName = cms.untracked.string('eh.root')
                               )
process.p = cms.Path(process.loadHistosFromRunInfo*process.cmgRunInfoCounter*process.cleanEvent)
process.e = cms.EndPath(process.saveHistosInRunInfo*process.out)


# message logger
process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = 500
