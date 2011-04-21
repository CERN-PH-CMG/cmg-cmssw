import FWCore.ParameterSet.Config as cms

process = cms.Process("COPY2")

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
    'file:AODNoSim.root'
    )
)



process.options   = cms.untracked.PSet( wantSummary = cms.untracked.bool(False))
#WARNING!
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(1000) )


process.load("Configuration.EventContent.EventContent_cff")
process.out = cms.OutputModule(
    "PoolOutputModule",
    process.AODSIMEventContent,
    #    outputCommands =  cms.untracked.vstring(
    #    'keep *'
    #    ),
    fileName = cms.untracked.string('prunedAOD.root'),
    )

process.load("PhysicsTools.PFCandProducer.PF2PAT_EventContent_cff")
process.out.outputCommands.extend( process.prunedAODForPF2PATEventContent.outputCommands )




process.endpath = cms.EndPath(
    process.out
    )


process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = 100
