import FWCore.ParameterSet.Config as cms

process = cms.Process("analysis")
process.load("FWCore.MessageService.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = cms.untracked.int32(1)
process.load('Configuration.StandardSequences.Services_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
process.load('JetMETCorrections.Configuration.JetCorrectionProducers_cff')
process.load('CMG.JetIDAnalysis.jetanalyzer_cfi')
process.load('CMG.MetAnalysis.metProducerSequence_cff')
 
#process.GlobalTag.globaltag = 'GR_R_42_V23::All'
process.GlobalTag.globaltag = 'MC_44_V12::All'
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(10) )

process.source = cms.Source("PoolSource",
                            fileNames = cms.untracked.vstring(
                            'file:/tmp/pharris/test.root'
                            ),
    skipEvents = cms.untracked.uint32(0)                        
)

process.output = cms.OutputModule("PoolOutputModule",
                                  outputCommands = cms.untracked.vstring('keep *'),
                                  fileName = cms.untracked.string("test.root")
)       

#process.ana = cms.Sequence(.metSquence)
process.ana      = cms.Sequence(process.ak5PFJetsL2L3*process.metSequence)
process.p        = cms.Path(process.ana)
process.outpath  = cms.EndPath(process.output)
