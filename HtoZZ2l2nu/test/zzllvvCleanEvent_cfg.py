import FWCore.ParameterSet.Config as cms

process = cms.Process("HtoZZto2l2nu")

from CMGTools.HtoZZ2l2nu.localPatTuples_cff import configureFromCommandLine
process.source = cms.Source("PoolSource",
                            fileNames = cms.untracked.vstring()
                            )
dtag, process.source.fileNames, outputFile = configureFromCommandLine(process)
print process.source.fileNames

process.load('CMGTools.HtoZZ2l2nu.CleanEventProducer_cfi')
process.load('CMGTools.HtoZZ2l2nu.CleanEventFilter_cfi')
process.out = cms.OutputModule("PoolOutputModule",
                               fileName = cms.untracked.string(outputFile),
                               outputCommands = cms.untracked.vstring('drop *',
                                                                      'keep *_prunedGen_*_*',
                                                                      'keep *_genEventScale_*_*',
                                                                      'keep *_addPileupInfo_*_*',
                                                                      'keep GenRunInfoProduct_*_*_*',
                                                                      'keep *_genMetTrue_*_*',
                                                                      'keep *_selectedPat*_*_*',
                                                                      'keep patMETs_*_*_*',
                                                                      'keep double*_*_rho_*',
                                                                      'keep *_generalTracks_*_*',
                                                                      'keep *_offlinePrimaryVertices_*_*',
                                                                      'keep *_offlinePrimaryVerticesDA_*_*',
                                                                      'keep recoPFCandidates_particleFlow__*',
                                                                      'keep edmMergeableCounter_*_*_*',
                                                                      'keep *_pfMet_*_*',
                                                                      'keep *_cleanEvent_*_*'),
                               SelectEvents   = cms.untracked.PSet( SelectEvents = cms.vstring('p') )
                               )
process.p = cms.Path(process.cleanEvent)
process.e = cms.EndPath(process.out)



# message logger
process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = 500
process.options = cms.untracked.PSet(
    wantSummary = cms.untracked.bool(True),
    SkipEvent = cms.untracked.vstring('ProductNotFound')
    )

