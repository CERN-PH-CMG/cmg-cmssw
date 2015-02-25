import FWCore.ParameterSet.Config as cms

process = cms.Process("GENSIM")

process.load("FWCore.MessageService.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = cms.untracked.int32(10)

process.load("SimGeneral.HepPDTESSource.pythiapdt_cfi")

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(1000)
)

process.RandomNumberGeneratorService = cms.Service("RandomNumberGeneratorService",
    generator = cms.PSet(
        initialSeed = cms.untracked.uint32(123456789),
        engineName = cms.untracked.string('HepJamesRandom')
    )
)


process.source = cms.Source("EmptySource")
process.load('CMGTools.PFSim.generators.ee_hzha')
# process.load('CMGTools.PFSim.generators.gun_cfi')
process.load('CMGTools.PFSim.genJets_cff')
process.load('CMGTools.PFSim.pfsim_cff')

# process.generator.comEnergy = 91.
# process.generator.CardsPath = cms.string("CMGTools/PFSim/python/generators/hzha04_ZqqHbb.cards")

process.p = cms.Path(
    process.generator +
    process.genJetsSequence +
    process.pfsimSequence
    )


# output definition

process.out = cms.OutputModule(
    "PoolOutputModule",
    dataset = cms.untracked.PSet(
    dataTier = cms.untracked.string('GENSIM')
    ),
    fileName = cms.untracked.string('gensim.root'),
    outputCommands = cms.untracked.vstring('keep *')
)

process.outpath = cms.EndPath(process.out)


process.genJets.verbose = False
