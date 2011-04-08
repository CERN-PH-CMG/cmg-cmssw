from PhysicsTools.PatAlgos.patTemplate_cfg import *
import FWCore.ParameterSet.Config as cms

sep_line = "-" * 50
print
print sep_line
print "CMGTools main test"
print sep_line

process.setName_('ANA')

process.maxEvents = cms.untracked.PSet(
        input = cms.untracked.int32(-1)
        )

process.maxLuminosityBlocks = cms.untracked.PSet( 
    input = cms.untracked.int32(20)
    )

process.source.fileNames = cms.untracked.vstring(
#    'file:/afs/cern.ch/user/c/cbern/scratch0/CMG/RelVal/ttbar_PATandPF2PAT.root',
    'file:/afs/cern.ch/user/c/cbern/scratch0/CMG/RelVal/4_1_3/ttbar_PATandPF2PAT.root',
    )


# output module for EDM event (ntuple)
process.out.fileName = cms.untracked.string('tree_testCMGTools.root')
from CMGTools.Common.eventContent.everything_cff import everything 
process.out.outputCommands = cms.untracked.vstring( 'drop *')
process.out.outputCommands.extend( everything ) 
    

#output file for histograms etc
process.TFileService = cms.Service("TFileService",
                                   fileName = cms.string("histograms_testCMGTools.root"))


# default analysis sequence    
process.load('CMGTools.Common.analysis_cff')

# now, we're going to tune the default analysis sequence to our needs
# by modifying the parameters of the modules present in this sequence. 

# Select events with 2 jet ...  
# process.cmgPFJetCount.minNumber = 2
# with pT > 50.
# process.cmgPFJetSel.cut = "pt()>50"
# and MET larger than 50
# process.cmgPFMETSel.cut = "pt()>50"

# note: we're reading ttbar events


process.p = cms.Path(
    process.analysisSequence
)

process.load("CMGTools.Common.runInfoAccounting_cfi")
process.outpath += process.runInfoAccounting


process.schedule = cms.Schedule(
    process.p,
    process.outpath
    )

process.MessageLogger.cerr.FwkReport.reportEvery = 100
process.options = cms.untracked.PSet( wantSummary = cms.untracked.bool(True) ) 
