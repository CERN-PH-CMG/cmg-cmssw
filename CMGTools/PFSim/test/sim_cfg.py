import FWCore.ParameterSet.Config as cms

process = cms.Process("Sim")

process.load("FWCore.MessageService.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = cms.untracked.int32(100)


process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
)


process.RandomNumberGeneratorService = cms.Service("RandomNumberGeneratorService",
    generator = cms.PSet(
        initialSeed = cms.untracked.uint32(123456789),
        engineName = cms.untracked.string('HepJamesRandom')
    )
)


# add poolsource

process.source = cms.Source(
    'PoolSource',
    fileNames = cms.untracked.vstring( 'file:gen.root' )
    )

process.load('PhysicsTools.HepMCCandAlgos.genParticles_cfi')

process.pfsim = cms.EDProducer(
    'PFSimParticleProducer',
    hepmcSrc = cms.InputTag('generator')
    )

process.fastjet = cms.EDProducer(
    'PFSimFastJetProducer',
    particleSrc = cms.InputTag('genParticles')
    )


process.out = cms.OutputModule(
    "PoolOutputModule",
    dataset = cms.untracked.PSet(
    dataTier = cms.untracked.string('SIM')
    ),
    fileName = cms.untracked.string('sim.root'),
    outputCommands = cms.untracked.vstring('keep *')
)

process.p = cms.Path(
    # process.pfsim +
    process.genParticles+
    process.fastjet 
    )


process.outpath = cms.EndPath(process.out)


