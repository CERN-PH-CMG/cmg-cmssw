import FWCore.ParameterSet.Config as cms

process = cms.Process("Gen")

process.load("FWCore.MessageService.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = cms.untracked.int32(100)

process.load("SimGeneral.HepPDTESSource.pythiapdt_cfi")

# Pythia settings for ttbar generation
#
process.load("Configuration.Generator.PythiaUESettings_cfi")


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
process.load('CMGTools.GenStudies.generators.pp_ttbar_cfi')

# I want to add:
# gen particle production
# gen particle for jet selection
# gen jet creation
# I could do that with my system.

process.load('PhysicsTools.HepMCCandAlgos.genParticles_cfi')

process.load('RecoJets.Configuration.GenJetParticles_cff')
process.genParticlesForJetsNoNu.excludeFromResonancePids = cms.vuint32(12,14,16)


process.load("RecoJets/Configuration/RecoGenJets_cff")

process.genJetPt10 = cms.EDFilter(
    "PtMinGenJetSelector",
    src = cms.InputTag('ak5GenJetsNoNu'),
    ptMin = cms.double(10)
    )

process.p = cms.Path(
    # make HepMC product from PYTHIA
    process.generator
    # make genParticles from HepMC product
    + process.genParticles
    # filter them for jets (remove invisible, and status!=1)
    + process.genParticlesForJetsNoNu
    # Make antikt 5 gen jets from genParticlesForJetsNoNu
    + process.ak5GenJetsNoNu
    # select high pt gen jets.
    + process.genJetPt10
    )


# output definition

process.out = cms.OutputModule(
    "PoolOutputModule",
    fileName = cms.untracked.string('gen.root'),
    outputCommands = cms.untracked.vstring(
      'keep *',
      'drop edmHepMCProduct_*_*_*',
      'drop recoGenParticles_*_*_*',
      'drop *_ak5GenJetsNoNu_*_*'
      )
)

process.outpath = cms.EndPath(process.out)


# process.genJets.verbose = False
