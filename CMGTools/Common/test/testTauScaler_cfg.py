from PhysicsTools.PatAlgos.patTemplate_cfg import *
import FWCore.ParameterSet.Config as cms
# from CMGTools.Common.Tools.getGlobalTag import getGlobalTag

sep_line = "-" * 50
print
print sep_line
print "CMGTools tau energy scaling test"
print sep_line



process.setName_('ANA')

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(5000)
    )

process.maxLuminosityBlocks = cms.untracked.PSet( 
    input = cms.untracked.int32(-1)
    )



from CMGTools.Production.datasetToSource import *
process.source = datasetToSource(
    'cmgtools',
    '/TauPlusX/Run2011A-PromptReco-v4/AOD/V2/PAT_CMG_V2_3_0',
    'tree.*root') 


# reading the first 10 files:
nFiles = 5
print 'WARNING: RESTRICTING INPUT TO THE FIRST', nFiles, 'FILES'
process.source.fileNames = process.source.fileNames[:nFiles] 

print process.source.fileNames

# output module for EDM event (ntuple)
process.out.fileName = cms.untracked.string('tree_testTauScaler.root')
from CMGTools.Common.eventContent.everything_cff import everything 
process.out.outputCommands = cms.untracked.vstring(
    'drop *',
    'keep cmgTau*_*_*_*'
    )


process.load('CMGTools.Common.factories.cmgTauScaler_cfi')

process.p = cms.Path(
    process.cmgTauUp +
    process.cmgTauDown
)


process.schedule = cms.Schedule(
    process.p,
    process.outpath
    )

process.MessageLogger.cerr.FwkReport.reportEvery = 100
process.options = cms.untracked.PSet( wantSummary = cms.untracked.bool(True) ) 
