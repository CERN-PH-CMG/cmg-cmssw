import FWCore.ParameterSet.Config as cms


process = cms.Process("TEST")
process.maxEvents = cms.untracked.PSet(input = cms.untracked.int32(1000))
process.source = cms.Source("EmptySource")

process.load("IOMC.RandomEngine.IOMC_cff")
#process.RandomNumberGeneratorService.generator.initialSeed = 8899191
#HZHA generator 
process.generator = cms.EDFilter("ZNNBGeneratorFilter",
  CardsPath = cms.string("GeneratorInterface/ZNNBInterface/data/znnb01.cards")
)

#make gen particles and all that stuff
process.load("SimGeneral.HepPDTESSource.pythiapdt_cfi")
process.load("PhysicsTools.HepMCCandAlgos.genParticles_cfi")
#process.load("RecoJets.Configuration.RecoGenJets_cff")
#process.load("RecoMET.Configuration.RecoGenMET_cff")
#process.load("RecoJets.Configuration.GenJetParticles_cff")
#process.load("RecoMET.Configuration.GenMETParticles_cff")
#process.load("RecoJets.JetProducers.ExclusiveNJadeGenJets_cfi") 
#process.load("RecoJets.JetProducers.ExclusiveYcutJadeGenJets_cfi") 
#process.genJetMET = cms.Sequence(process.genJetParticles*process.recoGenJets+process.genMETParticles*process.recoGenMET)
#process.genJetMET = cms.Sequence(process.genJetParticles*process.ExclusiveNJadeGenJets+process.ExclusiveYcutJadeGenJets)

#ourtput module
process.GEN = cms.OutputModule("PoolOutputModule",
	fileName = cms.untracked.string('znnb.root')
)

#paths
process.p = cms.Path(process.generator+
                    process.genParticles#+
                    #process.genJetMET
                     )
process.outpath = cms.EndPath(process.GEN)
