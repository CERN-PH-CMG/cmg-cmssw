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

process.load("CMGTools.Susy.gen_cff")
process.load("CMGTools.Susy.genToPFConverter_cfi")


process.dump = cms.EDAnalyzer("EventContentAnalyzer")

process.genMetTrue.src = 'particleFlow:GEN'

process.genMetTrueEt = process.genMetTrue.clone()

# this one fits with the "reconstructed" MET: 
process.genMetTrueEt.usePt = False

process.p = cms.Path(
    process.ProductionFilterSequence + 
    process.gen + 
    process.particleFlow +
    process.genMetTrue +
    process.genMetTrueEt
    )



process.outGen = cms.OutputModule(
    "PoolOutputModule",
    outputCommands =  cms.untracked.vstring('keep *'),
#    outputCommands =  cms.untracked.vstring(
#      'keep recoPFCandidates_*_*_*',
#      'keep *_genParticles_*_*',
#      'keep *_genMetTrue_*_*'),
    fileName = cms.untracked.string('gen.root'),
    SelectEvents = cms.untracked.PSet( SelectEvents = cms.vstring('p') )
    )

process.outpath = cms.EndPath(
    process.outGen
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
