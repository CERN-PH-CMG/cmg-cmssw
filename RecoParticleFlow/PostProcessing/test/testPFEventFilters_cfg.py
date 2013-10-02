import FWCore.ParameterSet.Config as cms

process = cms.Process("PFFILTER")


## Source
process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
    # 4_2 HT data
    # '/store/cmst3/user/cbern/CMG/HT/Run2011A-May10ReReco-v1/AOD/PFAOD_101_2_Onu.root',
    # below, the files from AN-10-383 (2010 data, adapted to 4_1_X or below)
#    'rfio:///castor/cern.ch/user/c/cbern/AN-10-383/MuonAndElectron/selected_Inconsistent.root'
#    'rfio:///castor/cern.ch/user/c/cbern/AN-10-383/MuonAndElectron/selected_Greedy.root'
    'rfio:///castor/cern.ch/user/c/cbern/AN-10-383/MuonAndElectron/selected_METMinimizing.root'
    )
)
## Maximal Number of Events
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

process.load("RecoParticleFlow.PostProcessing.selectGoodPFEvents_cff")

process.p = cms.Path(
    process.selectGoodPFEventsSequence
    )

process.pTag = cms.Path(
    process.selectGoodPFEventsTaggingSequence
    )


## Output Module Configuration (expects a path 'p')

from RecoParticleFlow.PostProcessing.pfPostProcessingEventContent_cff import pfPostProcessingEventContent

process.out = cms.OutputModule(
    "PoolOutputModule",
    fileName = cms.untracked.string('output.root'),
    # save only events passing the full path
    SelectEvents   = cms.untracked.PSet( SelectEvents = cms.vstring('p') ),
    # CHANGE THE FOLLOWING IF YOU WANT TO WRITE SOMETHING OUT
    outputCommands = pfPostProcessingEventContent
    )

process.outTag =  process.out.clone()
process.outTag.fileName = 'outputTag.root'
process.outTag.SelectEvents = cms.untracked.PSet( SelectEvents = cms.vstring('pTag') )


process.outpath = cms.EndPath(process.out+process.outTag)

## MessageLogger
process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = 100
process.options = cms.untracked.PSet( wantSummary = cms.untracked.bool(True) )
