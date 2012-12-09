# from PhysicsTools.PatAlgos.patTemplate_cfg import *
import FWCore.ParameterSet.Config as cms

sep_line = "-" * 50
print
print sep_line
print "CMGTools JetEnergyCorrector test"
print sep_line

process = cms.Process("ANA")

runOnMC = True
runOld5XGT = False

process.load("Configuration.StandardSequences.GeometryDB_cff")
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
process.load("Configuration.StandardSequences.MagneticField_38T_cff")

# process.load('Configuration.StandardSequences.Services_cff')
# process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
#process.GlobalTag.globaltag = 'GR_R_42_V23::All'  # for data
# process.GlobalTag.globaltag = 'START42_V17::All'  # for MC

from CMGTools.Common.Tools.getGlobalTag import getGlobalTag

process.GlobalTag.globaltag = getGlobalTag( runOnMC, runOld5XGT )
print 'Global tag       : ', process.GlobalTag.globaltag



process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
    )

process.maxLuminosityBlocks = cms.untracked.PSet( 
    input = cms.untracked.int32(-1)
    )


from CMGTools.Production.datasetToSource import *
process.source = datasetToSource(
    # 'CMS',
    # '/DoubleMu/Run2012C-PromptReco-v2/AOD'
    'cmgtools',
    '/VBF_HToTauTau_M-125_8TeV-powheg-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/PAT_CMG_V5_8_0',
    'cmgTuple.*root'
   )

# reading the first 10 files:
nFiles = 5
print 'WARNING: RESTRICTING INPUT TO THE FIRST', nFiles, 'FILES'
process.source.fileNames = process.source.fileNames[:nFiles] 

print process.source.fileNames

process.load('CMGTools.Common.miscProducers.cmgPFJetCorrector_cfi')

process.cmgPFJetCorrector.verbose = True
process.cmgPFJetCorrector.levels = ['L1FastJet','L2Relative','L3Absolute']
# process.cmgPFJetCorrector.levels = ['L1Offset','L2Relative','L3Absolute']

process.p = cms.Path(
    process.cmgPFJetCorrector
)

from CMGTools.Common.eventContent.everything_cff import everything
process.out = cms.OutputModule(
    "PoolOutputModule",
    fileName = cms.untracked.string('tree_testJEC.root'),
    # save only events passing the full path
    SelectEvents   = cms.untracked.PSet( SelectEvents = cms.vstring('p') ),
    # save PAT Layer 1 output; you need a '*' to
    # unpack the list of commands 'patEventContent'
    outputCommands = everything
    )
process.out.outputCommands.append( 'keep cmgPFJets_cmgPFJetCorrector_*_*' )

## MessageLogger
process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = 100
process.options = cms.untracked.PSet( wantSummary = cms.untracked.bool(False) ) 
