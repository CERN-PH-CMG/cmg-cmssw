from PhysicsTools.PatAlgos.patTemplate_cfg import *
import FWCore.ParameterSet.Config as cms

# Unit test for the MET analysis sequence.

sep_line = "-" * 50
print
print sep_line
print "MET sequence test"
print sep_line

process.setName_('ANA')

process.maxEvents = cms.untracked.PSet(
        input = cms.untracked.int32(2000)
        )

process.source.fileNames = cms.untracked.vstring(
    'file:/afs/cern.ch/user/c/cbern/scratch0/CMG/RelVal/4_1_3/ttbar_PATandPF2PAT.root'
    )

extension = 'met'

# output module for EDM event (ntuple)
process.out.fileName = cms.untracked.string('tree_test_%s.root' % extension)
from CMGTools.Common.eventContent.everything_cff import everything 
process.out.outputCommands = cms.untracked.vstring( 'drop *')
process.out.outputCommands.append('keep  patMETs_patMETsPFlow_*_*')
process.out.outputCommands.extend( everything ) 
    

#output file for histograms etc
process.TFileService = cms.Service("TFileService",
                                   fileName = cms.string("histograms_test_%s.root" % extension))


# default analysis sequence    
process.load('CMGTools.Common.analysis_cff')

# note: we're reading ttbar events

process.load("CMGTools.Common.jet_cff")
process.load("CMGTools.Common.met_cff")

# make test with the pf candidates
process.load('CMGTools.Common.Tools.cmgBaseMETModifier_cfi')
# MET from PFCandidates
process.testCmgBaseMETModifierSrc = process.cmgMETPFCandidates.clone()
process.testCmgBaseMETModifierSrc.cfg.inputCollection = "particleFlow"
# now subtract the pf candidate
process.testCmgBaseMETModifier = process.cmgBaseMETModifier.clone()
process.testCmgBaseMETModifier.cfg.inputCollection = "particleFlow"
process.testCmgBaseMETModifier.cfg.metCollection = 'testCmgBaseMETModifierSrc'
process.testCmgBaseMETModifier.cfg.operator = '-' #subtract from the MET
process.testCmgBaseMETSequence  = cms.Sequence(process.testCmgBaseMETModifierSrc*process.testCmgBaseMETModifier)
process.out.outputCommands.append('keep *_testCmgBaseMETModifier*_*_*')


process.analysisSequence = cms.Sequence(
    process.jetSequence+                                    
    process.metSequence+
    process.testCmgBaseMETSequence
    )

process.p = cms.Path(
    process.analysisSequence
)

process.schedule = cms.Schedule(
    process.p,
    process.outpath

    )

process.MessageLogger.cerr.FwkReport.reportEvery = 100
process.options = cms.untracked.PSet( wantSummary = cms.untracked.bool(True) ) 

print 'input  : ', process.source.fileNames
print 'output :'
print '  tree :', process.out.fileName
print '  hist :', process.TFileService.fileName
