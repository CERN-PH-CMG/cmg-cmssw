from PhysicsTools.PatAlgos.patTemplate_cfg import *
import FWCore.ParameterSet.Config as cms
from CMGTools.Common.Tools.getGlobalTag import getGlobalTag

sep_line = "-" * 50
print
print sep_line
print "CMGTools event cleaning test"
print sep_line

process.setName_('ANA')

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
    )

process.maxLuminosityBlocks = cms.untracked.PSet( 
    input = cms.untracked.int32(-1)
    )

# process.source.fileNames = cms.untracked.vstring(
#    # 'file:patTuple_HT.root'
#    'file:/afs/cern.ch/user/c/cbern/scratch0/patTuple_PF2PAT.root'
#    )

#process.load("CMGTools.Common.sources.HT.Run2011A_May10ReReco_v1.AOD.source_maxime_cff")

# a ttbar relval
# process.source.fileNames = ['/store/relval/CMSSW_4_2_3/RelValTTbar/GEN-SIM-RECO/START42_V12-v2/0063/6431F1F0-7F7B-E011-A8B7-003048678D9A.root']
# Loukas's skims
# process.source.fileNames = ['file:/afs/cern.ch/user/g/gouskos/scratch0/DATA/HT-May10ReReco-550toInf.root']
process.source.fileNames = ['file:/afs/cern.ch/user/g/gouskos/scratch0/DATA/HT-May10ReReco-450to550.root']

# reading the first 10 files:
nFiles = 10
print 'WARNING: RESTRICTING INPUT TO THE FIRST', nFiles, 'FILES'
process.source.fileNames = process.source.fileNames[:nFiles] 

print process.source.fileNames

# output module for EDM event (ntuple)
process.out.fileName = cms.untracked.string('tree_testCMGTools.root')
from CMGTools.Common.eventContent.everything_cff import everything 
process.out.outputCommands = cms.untracked.vstring( 'drop *')
process.out.outputCommands.extend( everything ) 
process.out.dropMetaData = cms.untracked.string('PRIOR')

process.load('CMGTools.Common.eventCleaning.eventCleaning_cff')

process.p = cms.Path(
    process.eventCleaningSequence
)

runOnMC = False
process.GlobalTag.globaltag = cms.string(getGlobalTag(runOnMC))

process.schedule = cms.Schedule(
    process.p,
    process.outpath
    )

process.MessageLogger.cerr.FwkReport.reportEvery = 100
process.options = cms.untracked.PSet( wantSummary = cms.untracked.bool(True) ) 
