import FWCore.ParameterSet.Config as cms

sep_line = "-" * 50
print
print sep_line
print "CMGTools slim CMGCandidates test"
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
    #'/DoubleMu/Run2012A-13Jul2012-v1/AOD/V5_B',
    #'/MuHad/Run2012A-13Jul2012-v1/AOD/V5_B',
    '/TTJets_MassiveBinDECAY_TuneZ2star_8TeV-madgraph-tauola/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM/V5_B',
   )
 
nFiles = 1
print 'WARNING: RESTRICTING INPUT TO THE FIRST', nFiles, 'FILES'
process.source.fileNames = process.source.fileNames[:nFiles] 

print process.source.fileNames


process.load("CMGTools.Common.cmgCandidates_cfi")

from CommonTools.ParticleFlow.goodOfflinePrimaryVertices_cfi import *
process.goodOfflinePrimaryVertices=goodOfflinePrimaryVertices.clone()

from RecoJets.JetProducers.ak5PFJets_cfi import *
process.ak5PFJets = ak5PFJets.clone()
process.ak5PFJets.src = 'particleFlow'
process.ak5PFJets.doAreaFastjet = True
process.ak5PFJets.doRhoFastjet = False
process.ak5PFJets2 = process.ak5PFJets.clone()
process.ak5PFJets2.src = 'cmgCandidates'

process.p = cms.Path(
    process.cmgCandidates +
    process.goodOfflinePrimaryVertices +
    process.ak5PFJets +
    process.ak5PFJets2
)

process.out = cms.OutputModule(
    "PoolOutputModule",
    fileName = cms.untracked.string('Candidates.root'),
    SelectEvents   = cms.untracked.PSet( SelectEvents = cms.vstring('p') ),
    outputCommands = cms.untracked.vstring( ['keep recoPFCandidates_particleFlow_*_*',
                                             'keep *_*_*_ANA'] )
    
 
    )

process.outpath = cms.EndPath(process.out)

## MessageLogger
process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = 100
process.options = cms.untracked.PSet( wantSummary = cms.untracked.bool(False) ) 
