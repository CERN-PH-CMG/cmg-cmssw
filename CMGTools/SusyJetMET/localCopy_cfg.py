import FWCore.ParameterSet.Config as cms

process = cms.Process("COPY")

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
    '/store/relval/CMSSW_3_8_4/RelValQCD_Pt_80_120/GEN-SIM-RECO/START38_V12-v1/0024/FC8E265A-96C2-DF11-916A-002618943972.root'
    )
)


process.load("CMGTools.SusyJetMET.Sources.HotSkims.sueann_highMHT_skim_cff")

process.options   = cms.untracked.PSet( wantSummary = cms.untracked.bool(False))
#WARNING!
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )


process.load("Configuration.EventContent.EventContent_cff")
process.out = cms.OutputModule("PoolOutputModule",
                               process.RECOSIMEventContent,
                               # outputCommands =  cms.untracked.vstring('keep *'),
                               fileName = cms.untracked.string('RECO.root'),
                               # fileName = cms.untracked.string('AODSIM.root'),
                               )

#from PFAnalyses.TTBar.topDecayFilters_cff import topFilterEventContent
#process.out.outputCommands.extend( topFilterEventContent)



process.endpath = cms.EndPath(
    process.out
    )


process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = 100
