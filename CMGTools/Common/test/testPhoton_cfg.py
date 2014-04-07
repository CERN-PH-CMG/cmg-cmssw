######################################################################
## File: testPhoton_cfg.py
######################################################################

from PhysicsTools.PatAlgos.patTemplate_cfg import *
import FWCore.ParameterSet.Config as cms

# Unit test for the photon analysis sequence.

sep_line = "-" * 50
print
print sep_line
print "Photon building test"
print sep_line

process.setName_('ANA')

process.maxEvents = cms.untracked.PSet(
        input = cms.untracked.int32(-1)
        )
process.maxLuminosityBlocks = cms.untracked.PSet( 
        input = cms.untracked.int32(1)
        )
process.source.fileNames = ['/store/cmst3/user/cmgtools/CMG/RelValPhotonJets_Pt_10/CMSSW_4_2_3-START42_V12-v2/GEN-SIM-RECO/patTuple_PF2PAT_0.root']

extension = 'photon'

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

process.load("CMGTools.Common.photon_cff")

process.analysisSequence = cms.Sequence(
    process.photonSequence
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
