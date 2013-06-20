import FWCore.ParameterSet.Config as cms

process = cms.Process("Gen")

process.load("FWCore.MessageService.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = cms.untracked.int32(100)

process.load("SimGeneral.HepPDTESSource.pythiapdt_cfi")

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(100)
)

process.RandomNumberGeneratorService = cms.Service("RandomNumberGeneratorService",
    generator = cms.PSet(
        initialSeed = cms.untracked.uint32(123456789),
        engineName = cms.untracked.string('HepJamesRandom')
    )
)

# here, you can also read a LHE file as in Phil's example
# generate the LHE file yourself with POWHEG
#process.source = cms.Source("EmptySource")
process.source = cms.Source("LHESource",
                            fileNames = cms.untracked.vstring('/store/lhe/8245/pwgevents-0001.lhe'))
                                                                  

# if you read LHE, you need to hadronize the event with pythia,
# so use Phil's hadronizer instead of this pythia generator
#process.load('CMGTools.GenStudies.generators.pp_ttbar_cfi')
process.load('Configuration.GenProduction.EightTeV.POWHEG_PYTHIA6_Tauola_H_tautau_8TeV_cff')

#atlas = True
atlas = False
if atlas: 
    from CMGTools.GenStudies.generators.pythiaUESettingsBlock_ATLAS_cfi import pythiaUESettingsBlock
    process.generator.PythiaParameters.pythiaUESettings = pythiaUESettingsBlock.pythiaUESettings

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
      'drop *_ak5GenJetsNoNu_*_*',
      'drop edmTriggerResults_*_*_*',
      'drop ints_*_*_*'
      )
)

process.outpath = cms.EndPath(process.out)


# process.genJets.verbose = False
