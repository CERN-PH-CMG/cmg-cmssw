######################################################################
## File: testMultiJet_cfg.py
######################################################################

from PhysicsTools.PatAlgos.patTemplate_cfg import *
import FWCore.ParameterSet.Config as cms

# Unit test for the multi-jet factory.

sep_line = "-" * 50
print
print sep_line
print "Multi-jet building test"
print sep_line

process.setName_('ANA')

process.maxEvents = cms.untracked.PSet(
        input = cms.untracked.int32(2000)
        )

process.source.fileNames = cms.untracked.vstring(
    'file:/afs/cern.ch/user/c/cbern/scratch0/CMG/RelVal/4_1_3/ttbar_PATandPF2PAT.root'
    )

extension = 'multijet'

# Output module for EDM event (ntuple).
process.out.fileName = cms.untracked.string('tree_test_%s.root' % extension)
from CMGTools.Common.eventContent.everything_cff import everything
process.out.outputCommands = cms.untracked.vstring('drop *')
process.out.outputCommands.extend(everything)
process.out.outputCommands.extend(["keep *_cmgMultiJet_*_*"])

# Output file for histograms etc.
process.TFileService = cms.Service("TFileService",
                                   fileName = cms.string("histograms_test_%s.root" % extension))


# Default analysis sequence.
process.load('CMGTools.Common.analysis_cff')

process.load("CMGTools.Common.jet_cff")
process.load("CMGTools.Common.multiJet_cff")

process.analysisSequence = cms.Sequence(
    process.jetSequence +
    process.multiJetSequence
    )

process.p = cms.Path(
    process.analysisSequence
)

process.schedule = cms.Schedule(
    process.p,
    process.outpath
    )

process.MessageLogger.cerr.FwkReport.reportEvery = 100
process.options = cms.untracked.PSet(wantSummary = cms.untracked.bool(True))

print 'input  :', process.source.fileNames
print 'output :'
print '  tree :', process.out.fileName
print '  hist :', process.TFileService.fileName

######################################################################
