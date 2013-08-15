import FWCore.ParameterSet.Config as cms

process = cms.Process("HtoZZto2l2nu")

process.load("Configuration.StandardSequences.Geometry_cff")
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
from Configuration.AlCa.autoCond import autoCond
process.GlobalTag.globaltag=cms.string(autoCond.get('startup',autoCond['mc']))
#else process.GlobalTag=autoCond['com10']
process.load("Configuration.StandardSequences.MagneticField_cff")
process.load("TrackingTools/TransientTrack/TransientTrackBuilder_cfi")

#the source is configured from the command line
process.source = cms.Source("PoolSource",
                            fileNames = cms.untracked.vstring()
                            )
from CMGTools.HtoZZ2l2nu.localPatTuples_cff import configureSourceFromCommandLine
castorDir, outputFile, process.source.fileNames = configureSourceFromCommandLine()

# histogram service
process.TFileService = cms.Service("TFileService", fileName = cms.string(outputFile) )

######################################
# ANALYSIS                           #
######################################
from CMGTools.HtoZZ2l2nu.Analysis_cff import defineAnalysis
defineAnalysis(process,castorDir,True)

#######################################
# SCHEDULE THE EXECUTION OF THE PATHS #
####################################### 
process.schedule = cms.Schedule( process.analysis )

# message logger
process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = 500
process.options = cms.untracked.PSet(
    wantSummary = cms.untracked.bool(True),
    SkipEvent = cms.untracked.vstring('ProductNotFound')
    )

