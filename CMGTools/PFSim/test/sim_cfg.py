import FWCore.ParameterSet.Config as cms

process = cms.Process("SIM")

process.load("FWCore.MessageService.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = cms.untracked.int32(100)


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
#    fileNames = cms.untracked.vstring( 'file:hzha_ZqqHbb_fastsim.root' )
    fileNames = cms.untracked.vstring( 'file:gun_gamma_fastsim.root' )
    )


process.load('CMGTools.PFSim.pfsim_cff')

process.p = cms.Path(
    process.pfsimSequence
    )


from CMGTools.PFSim.eventContent import simple as gensimreco

process.out = cms.OutputModule(
    "PoolOutputModule",
    dataset = cms.untracked.PSet(
    dataTier = cms.untracked.string('SIM')
    ),
    fileName = cms.untracked.string('sim.root'),
    outputCommands = cms.untracked.vstring( *gensimreco )
)

process.outpath = cms.EndPath(process.out)


test = False
if test:
    process.maxEvents.input = 10
    process.pfsim.verbose = True
