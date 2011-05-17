######################################################################
## File: testHemi_cfg.py
######################################################################

from PhysicsTools.PatAlgos.patTemplate_cfg import *
import FWCore.ParameterSet.Config as cms

# Unit test for the jet hemisphere analysis sequence.

sep_line = "-" * 50
print
print sep_line
print "Hemisphere and di-hemisphere building test"
print sep_line

process.setName_('ANA')

process.maxEvents = cms.untracked.PSet(
        input = cms.untracked.int32(-1)
        )
process.maxLuminosityBlocks = cms.untracked.PSet(
        input = cms.untracked.int32(1)
        )
process.load("CMGTools.Common.sources.relval.RelValQCD_FlatPt_15_3000.CMSSW_3_11_2.MC_311_V2.NoPileUpRemoval.source_cff")

extension = 'hemi'

# Output module for EDM event (ntuple).
process.out.fileName = cms.untracked.string('tree_test_%s.root' % extension)
from CMGTools.Common.eventContent.everything_cff import everything
process.out.outputCommands = cms.untracked.vstring('drop *')
process.out.outputCommands.extend(everything)

# Output file for histograms etc.
process.TFileService = cms.Service("TFileService",
                                   fileName = cms.string("histograms_test_%s.root" % extension))

# Default analysis sequence.
process.load('CMGTools.Common.analysis_cff')

process.load("CMGTools.Common.jet_cff")
process.load("CMGTools.Common.hemi_cff")
process.load("CMGTools.Common.diHemi_cff")

# This just uses two small collections to test that everything works
# without taking too much time.
cfg_tmp = cms.PSet(process.cmgHemi.cfg.clone(inputCollection=
                                             cms.VInputTag("cmgPFJetLead",
                                                           "cmgPFJetLead")))
process.cmgHemi = process.cmgHemi.clone(cfg=cfg_tmp)

process.analysisSequence = cms.Sequence(
    process.jetSequence +
    process.hemiSequence +
    process.diHemiSequence
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
