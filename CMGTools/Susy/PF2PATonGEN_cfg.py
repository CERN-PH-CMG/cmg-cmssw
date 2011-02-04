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
# process.load("CMGTools.Susy.genToPFConverter_cfi")

# PF2PAT + PAT, working at gen level

process.load("PhysicsTools.PatAlgos.patSequences_cff")

from PhysicsTools.PatAlgos.tools.pfTools import *
postfix = "PFlow"
usePF2PAT(process,runPF2PAT=True, jetAlgo='AK5', runOnMC=True, postfix=postfix) 
from CMGTools.Susy.PF2PATonGEN_cff import * 
PF2PATonGEN(process)

# CMG RA2 analysis

process.dump = cms.EDAnalyzer("EventContentAnalyzer")

process.p = cms.Path(
    process.ProductionFilterSequence + 
    process.gen + 
    process.PF2PATonGENSequence
    )

process.outGen = cms.OutputModule(
    "PoolOutputModule",
#    outputCommands =  cms.untracked.vstring('keep *'),
    outputCommands =  cms.untracked.vstring(
      'keep recoPFCandidates_*_*_*',
      'keep *_genParticles_*_*',
      'keep *_genMetTrue_*_*'),
    fileName = cms.untracked.string('gen.root'),
    SelectEvents = cms.untracked.PSet( SelectEvents = cms.vstring('p') )
    )

from PhysicsTools.PatAlgos.patEventContent_cff import patEventContentNoCleaning
#process.load("PhysicsTools.PFCandProducer.PF2PAT_EventContent_cff")
#process.out.outputCommands =  cms.untracked.vstring('drop *')
process.out.fileName = 'PF2PATonGEN.root'
process.out.outputCommands = cms.untracked.vstring( *patEventContentNoCleaning) 
process.out.outputCommands.append( 'keep *_genMetTrue*_*_*' )
process.out.outputCommands.append( 'keep recoPFCandidates_particleFlow_*_*' )

process.outpath = cms.EndPath(
    process.out
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
