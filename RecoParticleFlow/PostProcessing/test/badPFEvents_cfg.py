import FWCore.ParameterSet.Config as cms

process = cms.Process("PFFILTER")


## Source
process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
    # 4_2 HT data
    '/store/cmst3/user/cbern/CMG/HT/Run2011A-May10ReReco-v1/AOD/PFAOD_101_2_Onu.root',
    # below, the files from AN-10-383 (2010 data, adapted to 4_1_X or below)
#    'rfio:///castor/cern.ch/user/c/cbern/AN-10-383/MuonAndElectron/selected_Inconsistent.root'
#    'rfio:///castor/cern.ch/user/c/cbern/AN-10-383/MuonAndElectron/selected_Greedy.root'
#    'rfio:///castor/cern.ch/user/c/cbern/AN-10-383/MuonAndElectron/selected_METMinimizing.root'
    )
)

# process.load("CMGTools.Common.sources.HT.Run2011A_May10ReReco_v1.AOD.source_cff")
process.load("CMGTools.Common.sources.HT.Run2011A_May10ReReco_v1.AOD.source_maxime_cff")

# process.source.fileNames = [ process.source.fileNames[0] ]
print process.source.fileNames

## Maximal Number of Events
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

process.load("RecoParticleFlow.PostProcessing.badPFEvents_cff")

process.outpath = cms.EndPath(
    process.outInconsistentMuons +
    process.outGreedyMuons
    # + process.outPFEventFilter
    )

## MessageLogger
process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = 100
process.options = cms.untracked.PSet( wantSummary = cms.untracked.bool(True) )
