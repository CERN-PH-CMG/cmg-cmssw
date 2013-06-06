import FWCore.ParameterSet.Config as cms

process = cms.Process("Sim")

process.load("FWCore.MessageService.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = cms.untracked.int32(10)


process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
)

process.load("SimGeneral.HepPDTESSource.pythiapdt_cfi")

process.RandomNumberGeneratorService = cms.Service("RandomNumberGeneratorService",
    generator = cms.PSet(
        initialSeed = cms.untracked.uint32(123456789),
        engineName = cms.untracked.string('HepJamesRandom')
    )
)



process.source = cms.Source(
    'PoolSource',
    fileNames = cms.untracked.vstring( 'file:Prod_TT_20k/Job_1/gen.root' )
    )

process.pfsim = cms.EDProducer(
    'PFSimParticleProducer',
    hepmcSrc = cms.InputTag('generator'),
    verbose = cms.untracked.bool( False )
    )

process.jets = cms.EDProducer(
    'PFSimFastJetProducer',
    particleSrc = cms.InputTag('pfsim'),
    jetPtThreshold = cms.double(5.),
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
    process.pfsim 
    + process.jets
    )


process.outpath = cms.EndPath(process.out)


test = False
if test:
    process.maxEvents.input = 10
    process.pfsim.verbose = True
