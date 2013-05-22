import FWCore.ParameterSet.Config as cms

process = cms.Process("Gen")

process.load("FWCore.MessageService.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = cms.untracked.int32(10)

process.load("SimGeneral.HepPDTESSource.pythiapdt_cfi")

# Pythia settings for ttbar generation
#
process.load("Configuration.Generator.PythiaUESettings_cfi")

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(5)
)


process.RandomNumberGeneratorService = cms.Service("RandomNumberGeneratorService",
    generator = cms.PSet(
        initialSeed = cms.untracked.uint32(123456789),
        engineName = cms.untracked.string('HepJamesRandom')
    )
)


process.source = cms.Source("EmptySource")

process.load('CMGTools.PFSim.generators.pp_ttbar_cfi')

process.pfsim = cms.EDProducer(
    'PFSimParticleProducer',
    hepmcSrc = cms.InputTag('generator')
    )

process.out = cms.OutputModule(
    "PoolOutputModule",
    dataset = cms.untracked.PSet(
    dataTier = cms.untracked.string('GEN')
    ),
    fileName = cms.untracked.string('MC.root'),
    outputCommands = cms.untracked.vstring('keep *')
)

process.p = cms.Path(
    process.generator
    + process.pfsim
    )


process.outpath = cms.EndPath(process.out)


