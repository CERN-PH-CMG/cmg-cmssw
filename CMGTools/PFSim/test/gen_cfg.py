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

process.generator = cms.EDFilter("Pythia6GeneratorFilter",
    pythiaHepMCVerbosity = cms.untracked.bool(False),
    maxEventsToPrint = cms.untracked.int32(0),
    pythiaPylistVerbosity = cms.untracked.int32(0),
    filterEfficiency = cms.untracked.double(1.0),
    comEnergy = cms.double(10000.0),
    PythiaParameters = cms.PSet(
        process.pythiaUESettingsBlock,
        processParameters = cms.vstring('MSEL      = 0     ! User defined processes', 
            'MSUB(81)  = 1     ! qqbar to QQbar', 
            'MSUB(82)  = 1     ! gg to QQbar', 
            'MSTP(7)   = 6     ! flavour = top', 
            'PMAS(6,1) = 175.  ! top quark mass'),
        # This is a vector of ParameterSet names to be read, in this order
        parameterSets = cms.vstring('pythiaUESettings', 
            'processParameters')
    )
)

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


