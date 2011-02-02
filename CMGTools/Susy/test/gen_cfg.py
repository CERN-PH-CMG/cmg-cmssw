import FWCore.ParameterSet.Config as cms

process = cms.Process("GEN")

# overriding source and various other things


process.options   = cms.untracked.PSet( wantSummary = cms.untracked.bool(False))
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(1000) )

process.out = cms.OutputModule(
    "PoolOutputModule",
    outputCommands =  cms.untracked.vstring('keep *'),
#    outputCommands =  cms.untracked.vstring(
#      'keep recoPFCandidates_*_*_*',
#      'keep *_genParticles_*_*',
#      'keep *_genMetTrue_*_*'),
    fileName = cms.untracked.string('gen.root'),
    SelectEvents = cms.untracked.PSet( SelectEvents = cms.vstring('p') )
    )


#process.load("PhysicsTools.HepMCCandAlgos.genParticles_cfi")
#process.load("RecoJets.Configuration.GenJetParticles_cff")
#process.load("RecoJets.Configuration.RecoGenJets_cff")

#process.load("RecoMET.Configuration.GenMETParticles_cff")
#process.load("RecoMET.Configuration.RecoGenMET_cff")

process.load("CMGTools.Susy.GeneratorSources.pythia_LM0_cff")

process.load("CMGTools.Susy.gen_cff")
process.load("CMGTools.Susy.genToPFConverter_cfi")

process.p = cms.Path(
    process.ProductionFilterSequence + 
    process.gen + 
    process.particleFlow
    )

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
