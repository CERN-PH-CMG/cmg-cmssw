from PhysicsTools.PatAlgos.patTemplate_cfg import *

import sys
# import FWCore.ParameterSet.Config as cms

# changing process name 
# process.setName_("GEN")



process.options   = cms.untracked.PSet( wantSummary = cms.untracked.bool(False))

# source 

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(1000) )

process.load("CMGTools.Susy.GeneratorSources.pythia_LM0_cff")

# gen sequences: convert gen particles to PFCandidates 

process.load("CMGTools.Susy.GEN_cff")

process.dump = cms.EDAnalyzer("EventContentAnalyzer")

process.p = cms.Path(
    process.ProductionFilterSequence + 
    process.GENSequence
    )


process.load("CMGTools.Susy.outputGEN_cfi")

process.outpath = cms.EndPath(
    process.outputGEN
    )

# Add the top decay to the output file
# Schedule the paths
process.schedule = cms.Schedule(
    process.p,
    process.outpath
)


process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = 100
process.load("SimGeneral.HepPDTESSource.pythiapdt_cfi")
process.load("FastSimulation.Configuration.RandomServiceInitialization_cff")
