import FWCore.ParameterSet.Config as cms

process = cms.Process("COPY")

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
    '/store/cmst3/user/cmgtools/CMG/VBF_HToTauTau_M-115_7TeV-powheg-pythia6-tauola/Summer11-PU_S4_START42_V11-v1/AODSIM/V2/PAT_CMG_V2_3_0/tree_CMG_0.root'
    )
)

process.options   = cms.untracked.PSet( wantSummary = cms.untracked.bool(False))
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )


process.out = cms.OutputModule(
    "PoolOutputModule",
    outputCommands =  cms.untracked.vstring(
    'keep *',
    ),
    fileName = cms.untracked.string('copy.root'),
    )

process.endpath = cms.EndPath(
    process.out
    )


process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = 100
