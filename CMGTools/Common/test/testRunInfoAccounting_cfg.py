from PhysicsTools.PatAlgos.patTemplate_cfg import *
import FWCore.ParameterSet.Config as cms

sep_line = "-" * 50
print
print sep_line
print "CMGTools run info accounting test"
print sep_line

process.setName_('ANA')

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
    )

process.maxLuminosityBlocks = cms.untracked.PSet( 
    input = cms.untracked.int32(-1)
    )

# process.source.fileNames = cms.untracked.vstring(
#     '/store/cmst3/user/cbern/CMG/LM4_SUSY_sftsht_7TeV-pythia6/Summer11-PU_S4_START42_V11-v1/AODSIM/V2/PFAOD_1_1_m8a.root'
#     )

runOnMC = False
if runOnMC:
    process.load("CMGTools.Common.sources.LM4_SUSY_sftsht_7TeV_pythia6.Summer11_PU_S4_START42_V11_v1.AODSIM.V2.source_cff")
else:
    process.load("CMGTools.Common.sources.HT.Run2011A_May10ReReco_v1.AOD.V2.source_cff")

# reading the first 10 files:
nFiles = 10
print 'WARNING: RESTRICTING INPUT TO THE FIRST', nFiles, 'FILES'
process.source.fileNames = process.source.fileNames[:nFiles] 

print process.source.fileNames

#output file for histograms etc
process.TFileService = cms.Service("TFileService",
                                   fileName = cms.string("histograms_testRunInfoAccounting.root"))


process.load("CMGTools.Common.runInfoAccounting_cff")

# this module is doing nothing.
# change the prescale factor to see the effect on the run info accounting. 
process.pre1 = cms.EDFilter(
    "Prescaler",
    prescaleFactor = cms.int32(2),
    prescaleOffset = cms.int32(0)
)

process.p = cms.Path(
    process.pre1
    )

process.ria = cms.Sequence( process.runInfoAccountingDataSequence )
if runOnMC:
    process.ria = cms.Sequence( process.runInfoAccountingSequence )


process.outpath = cms.EndPath(
    process.ria
    )

process.schedule = cms.Schedule(
    process.p,
    process.outpath
    )

process.MessageLogger.cerr.FwkReport.reportEvery = 100
process.options = cms.untracked.PSet( wantSummary = cms.untracked.bool(True) ) 
