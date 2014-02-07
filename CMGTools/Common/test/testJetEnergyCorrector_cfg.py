# from PhysicsTools.PatAlgos.patTemplate_cfg import *
import FWCore.ParameterSet.Config as cms

sep_line = "-" * 50
print
print sep_line
print "CMGTools JetEnergyCorrector test"
print sep_line

process = cms.Process("ANA")

runOnMC = False
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
    input = cms.untracked.int32(1000)
    )

process.maxLuminosityBlocks = cms.untracked.PSet( 
    input = cms.untracked.int32(-1)
    )


from CMGTools.Production.datasetToSource import *
process.source = datasetToSource(  
    'cmgtools',
    # '/VBF_HToTauTau_M-125_8TeV-powheg-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/PAT_CMG_V5_8_0',
    # '/DYJetsToLL_M-50_TuneZ2Star_8TeV-madgraph-tarball/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B/PAT_CMG_V5_10_0',
    '/DoubleMu/Run2012A-13Jul2012-v1/AOD/V5_B/PAT_CMG_V5_10_0',
    'cmgTuple.*root'
   )

# reading the first 10 files:
nFiles = 5
print 'WARNING: RESTRICTING INPUT TO THE FIRST', nFiles, 'FILES'
process.source.fileNames = process.source.fileNames[:nFiles] 

print process.source.fileNames

from CMGTools.Common.miscProducers.cmgPFJetCorrector_cfi import cmgPFJetCorrector
process.cmgPFJetSel = cmgPFJetCorrector.clone()

process.cmgPFJetSel.verbose = False
process.cmgPFJetSel.levels = ['L1FastJet','L2Relative','L3Absolute']
if not runOnMC:
    process.cmgPFJetSel.levels = ['L1FastJet','L2Relative','L3Absolute','L2L3Residual']

print 'Correction levels', process.cmgPFJetSel.levels

process.p = cms.Path(
    process.cmgPFJetSel
)

from CMGTools.Common.eventContent.everything_cff import everything
process.out = cms.OutputModule(
    "PoolOutputModule",
    fileName = cms.untracked.string('cmgTuple.root'),
    # save only events passing the full path
    SelectEvents   = cms.untracked.PSet( SelectEvents = cms.vstring('p') ),
    # save PAT Layer 1 output; you need a '*' to
    # unpack the list of commands 'patEventContent'
    outputCommands = everything
    )
process.out.outputCommands.append( 'keep cmgPFJets_cmgPFJetSel_*_*' )
process.outpath = cms.EndPath(process.out)

## MessageLogger
process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = 100
process.options = cms.untracked.PSet( wantSummary = cms.untracked.bool(False) ) 
