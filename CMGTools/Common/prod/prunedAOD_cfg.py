import FWCore.ParameterSet.Config as cms

process = cms.Process("COPY2")

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
    'file:AODNoSim.root'
    )
)

# process.load("CMGTools.Common.sources.DoubleElectron.Run2011A_PromptReco_v1.AOD.source_cff")
process.load("CMGTools.Common.sources.HT.Run2011A_PromptReco_v1.AOD.source_cff")


process.options   = cms.untracked.PSet( wantSummary = cms.untracked.bool(False))
#WARNING!
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )



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
process.out.outputCommands.append('keep *_HBHENoiseFilterResultProducer_*_*')


process.load("CMGTools.Common.eventCleaning.eventCleaning_cff")

process.p = cms.Path(
    process.eventCleaningSequence
    )

process.endpath = cms.EndPath(
    process.out
    )


process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = 10
