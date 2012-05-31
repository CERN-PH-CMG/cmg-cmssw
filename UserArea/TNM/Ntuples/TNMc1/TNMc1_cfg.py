#$Revision: 1.2 $
import FWCore.ParameterSet.Config as cms

process = cms.Process("TheNtupleMaker")

process.load("FWCore.MessageService.MessageLogger_cfi")
# See TheNtupleMaker twiki for a brief explanation
#process.MessageLogger.destinations = cms.untracked.vstring("cerr")
#process.MessageLogger.cerr.FwkReport.reportEvery = 10
process.MessageLogger.cerr.default.limit = 5

# This is required in order to configure HLTConfigProducer
process.load("L1TriggerConfig.L1GtConfigProducers.L1GtConfig_cff")

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

process.source = cms.Source("PoolSource",
	fileNames =
	cms.untracked.vstring("file:patTuple.root"
	)
)
process.load("Ntuples.TNMc1.ntuple_cfi")

process.p = cms.Path(process.demo)
