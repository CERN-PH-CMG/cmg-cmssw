######################################################################
## File: testHemi_cfg.py
######################################################################

from PhysicsTools.PatAlgos.patTemplate_cfg import *
import FWCore.ParameterSet.Config as cms

# Unit test for the jet hemisphere analysis sequence.

process.setName_('ANA')

process.maxEvents = cms.untracked.PSet(
        input = cms.untracked.int32(2000)
        )

process.source.fileNames = cms.untracked.vstring(
    'file:/afs/cern.ch/user/c/cbern/public/patTuple_PATandPF2PAT.root'
    )

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

# Note: we're reading ttbar events.

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
