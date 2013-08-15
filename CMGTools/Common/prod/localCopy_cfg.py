import FWCore.ParameterSet.Config as cms

process = cms.Process("COPY")

pickRelVal = False

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
    'file:AODNoSim.root'
    )
)

if pickRelVal:
    from PhysicsTools.PatAlgos.tools.cmsswVersionTools import pickRelValInputFiles

    process.source = cms.Source(
        "PoolSource",
        fileNames = cms.untracked.vstring(
        pickRelValInputFiles( cmsswVersion  = 'CMSSW_4_1_2'
                              , relVal = 'RelValTTbar'
                              # , globalTag     = 'START311_V2'
                              , globalTag = 'MC_311_V2'
                              , numberOfFiles = 999
                              )
        )
        )


process.options   = cms.untracked.PSet( wantSummary = cms.untracked.bool(False))

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )


process.load("Configuration.EventContent.EventContent_cff")
process.out = cms.OutputModule(
    "PoolOutputModule",
    process.AODSIMEventContent,
#    outputCommands =  cms.untracked.vstring(
#    'keep *'
#    ),
    fileName = cms.untracked.string('copy.root'),
    )

process.endpath = cms.EndPath(
    process.out
    )


process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = 100
