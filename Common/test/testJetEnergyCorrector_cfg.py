from PhysicsTools.PatAlgos.patTemplate_cfg import *
import FWCore.ParameterSet.Config as cms
# from CMGTools.Common.Tools.getGlobalTag import getGlobalTag

process.load('Configuration.StandardSequences.Services_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
process.GlobalTag.globaltag = 'GR_R_42_V19::All'

sep_line = "-" * 50
print
print sep_line
print "CMGTools JetEnergyCorrector test"
print sep_line



process.setName_('ANA')

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
    )

process.maxLuminosityBlocks = cms.untracked.PSet( 
    input = cms.untracked.int32(-1)
    )



from CMGTools.Production.datasetToSource import *
process.source = datasetToSource(
    'cmgtools',
    '/WJetsToLNu_TuneZ2_7TeV-madgraph-tauola/Summer11-PU_S4_START42_V11-v1/AODSIM/V2/PAT_CMG_V2_3_0',
    # '/TauPlusX/Run2011A-PromptReco-v6/AOD/V2/PAT_CMG_V2_3_0',
    'tree.*root') 


# reading the first 10 files:
nFiles = 5
print 'WARNING: RESTRICTING INPUT TO THE FIRST', nFiles, 'FILES'
process.source.fileNames = process.source.fileNames[:nFiles] 

print process.source.fileNames

# output module for EDM event (ntuple)
process.out.fileName = cms.untracked.string('tree_testJEC.root')
from CMGTools.Common.eventContent.everything_cff import everything

process.out.outputCommands = everything
process.out.outputCommands.append( 'keep cmgPFJets_cmgPFJetCorrector_*_*' )

process.load('CMGTools.Common.miscProducers.cmgPFJetCorrector_cfi')

process.cmgPFJetCorrector.verbose = True
process.cmgPFJetCorrector.levels = ['L1FastJet','L2Relative','L3Absolute']

process.p = cms.Path(
    process.cmgPFJetCorrector
)


process.schedule = cms.Schedule(
    process.p,
    process.outpath
    )

process.MessageLogger.cerr.FwkReport.reportEvery = 100
process.options = cms.untracked.PSet( wantSummary = cms.untracked.bool(False) ) 
