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
                               fileName = cms.untracked.string('/tmp/evHyp.root'),
                               outputCommands = cms.untracked.vstring('drop *',
                                                                      'keep *_MEtoEDMConverter_*_*',
                                                                      'keep *_prunedGen_*_*',
                                                                      'keep *_genEventScale_*_*',
                                                                      'keep GenRunInfoProduct_*_*_*',
                                                                      'keep *_genMetTrue_*_*',
                                                                      'keep *_selectedPat*_*_*',
                                                                      'keep patMETs_*_*_*',
                                                                      'keep double*_*_rho_*',
                                                                      'keep *_tcMet_*_*',
                                                                      'keep *_pfMet_*_*',
                                                                      'keep *_cleanEvent_*_*')
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

