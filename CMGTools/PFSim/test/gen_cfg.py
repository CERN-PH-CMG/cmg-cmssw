import FWCore.ParameterSet.Config as cms

process = cms.Process("Gen")

process.load("FWCore.MessageService.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = cms.untracked.int32(10)

process.load("SimGeneral.HepPDTESSource.pythiapdt_cfi")

# Pythia settings for ttbar generation
#
process.load("Configuration.Generator.PythiaUESettings_cfi")


process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(5000)
)


process.RandomNumberGeneratorService = cms.Service("RandomNumberGeneratorService",
    generator = cms.PSet(
        initialSeed = cms.untracked.uint32(123456789),
        engineName = cms.untracked.string('HepJamesRandom')
    )
)


process.source = cms.Source("EmptySource")
process.load('CMGTools.PFSim.generators.pp_ttbar_cfi')

# I want to add:
# gen particle production
# gen particle for jet selection
# gen jet creation
# I could do that with my system.

process.load('CMGTools.PFSim.genJets_cff')

process.p = cms.Path(
    process.generator +
    process.genJetsSequence
    )


# output definition

process.out = cms.OutputModule(
    "PoolOutputModule",
    dataset = cms.untracked.PSet(
    dataTier = cms.untracked.string('GEN')
    ),
    fileName = cms.untracked.string('gen.root'),
    outputCommands = cms.untracked.vstring('keep *')
)

process.outpath = cms.EndPath(process.out)


process.genJets.verbose = False
