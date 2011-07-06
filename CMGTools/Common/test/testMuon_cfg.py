from PhysicsTools.PatAlgos.patTemplate_cfg import *
import FWCore.ParameterSet.Config as cms

from CMGTools.Common.testing_sources.sourceFiles import testing_sources
sourceFileList = testing_sources()

# Unit test for the muon analysis sequence.

sep_line = "-" * 50
print
print sep_line
print "Muon sequence test"
print sep_line

process.setName_('ANA')

process.maxEvents = cms.untracked.PSet(
        input = cms.untracked.int32(-1)
        )
process.maxLuminosityBlocks = cms.untracked.PSet( 
        input = cms.untracked.int32(10)
        )
process.source.fileNames = sourceFileList['ZMM']

extension = 'muons'

# output module for EDM event (ntuple)
process.out.fileName = cms.untracked.string('tree_test_%s.root' % extension)
from CMGTools.Common.eventContent.everything_cff import everything 
process.out.outputCommands = cms.untracked.vstring( 'drop *')
process.out.outputCommands.extend( everything ) 
    

#output file for histograms etc
process.TFileService = cms.Service("TFileService",
                                   fileName = cms.string("histograms_test_%s.root" % extension))


# default analysis sequence    
process.load('CMGTools.Common.analysis_cff')
process.load('CMGTools.Common.cutSummary_cff')

process.load("CMGTools.Common.met_cff")
process.load("CMGTools.Common.muon_cff")
process.load("CMGTools.Common.diMuon_cff")

process.zmumusummary = process.cutSummaryMuon.clone(inputCollection = cms.InputTag("cmgDiMuon"))

process.analysisSequence = cms.Sequence(
    process.pfSimpleMetSequence + 
    process.muonSequence +
    process.diMuonSequence +
    process.zmumusummary
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
