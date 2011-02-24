import FWCore.ParameterSet.Config as cms


outputGEN = cms.OutputModule(
    "PoolOutputModule",
    outputCommands =  cms.untracked.vstring('keep *'),
#     outputCommands =  cms.untracked.vstring(
#       'keep *_particleFlow_*_*',
#       'keep *_genMetTrue_*_*'),
    fileName = cms.untracked.string('GEN.root'),
    SelectEvents = cms.untracked.PSet( SelectEvents = cms.vstring('p') )
    )
