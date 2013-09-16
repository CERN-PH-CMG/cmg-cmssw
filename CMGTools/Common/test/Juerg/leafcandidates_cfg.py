# from PhysicsTools.PatAlgos.patTemplate_cfg import *
import FWCore.ParameterSet.Config as cms

sep_line = "-" * 50
print
print sep_line
print "CMGTools leaf candidates test"
print sep_line

process = cms.Process("ANA")

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(2000)
    )

process.maxLuminosityBlocks = cms.untracked.PSet( 
    input = cms.untracked.int32(-1)
    )


from CMGTools.Production.datasetToSource import *
process.source = datasetToSource(  
    'cmgtools_group',
    '/DoubleMu/Run2012A-13Jul2012-v1/AOD/V5_B',
    '.*root'
   )

# reading the first 10 files:
nFiles = 5
print 'WARNING: RESTRICTING INPUT TO THE FIRST', nFiles, 'FILES'
process.source.fileNames = process.source.fileNames[:nFiles] 

print process.source.fileNames

process.pfLeafCandidates = cms.EDProducer(
    "CandReducer",
    src = cms.InputTag("particleFlow")
    ) 

process.p = cms.Path(
    process.pfLeafCandidates
)

process.out = cms.OutputModule(
    "PoolOutputModule",
    fileName = cms.untracked.string('leafCandidates.root'),
    # save only events passing the full path
    SelectEvents   = cms.untracked.PSet( SelectEvents = cms.vstring('p') ),
    # save PAT Layer 1 output; you need a '*' to
    # unpack the list of commands 'patEventContent'
    outputCommands = cms.untracked.vstring( ['keep *_particleFlow_*_*',
                                             'keep *_*_*_ANA'] )
    
 
    )

process.outpath = cms.EndPath(process.out)

## MessageLogger
process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = 100
process.options = cms.untracked.PSet( wantSummary = cms.untracked.bool(False) ) 
