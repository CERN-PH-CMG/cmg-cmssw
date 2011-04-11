from PhysicsTools.PatAlgos.patTemplate_cfg import *
import FWCore.ParameterSet.Config as cms

# Unit test for the jet analysis sequence.

sep_line = "-" * 50
print
print sep_line
print "Jet sequence test"
print sep_line

process.setName_('ANA')

process.maxEvents = cms.untracked.PSet(
        input = cms.untracked.int32(-1)
        )


process.source = cms.Source(
    "PoolSource",
    
    noEventSort = cms.untracked.bool(True),
    duplicateCheckMode = cms.untracked.string("noDuplicateCheck"),
    fileNames = cms.untracked.vstring()
    )
process.source.fileNames.extend([
    '/store/cmst3/user/cbern/CMG/RelVal/4_1_2/RelValQCD_FlatPt_15_3000/patTuple_PATandPF2PAT_RelValQCD_FlatPt_15_3000_0.root',
    '/store/cmst3/user/cbern/CMG/RelVal/4_1_2/RelValQCD_FlatPt_15_3000/patTuple_PATandPF2PAT_RelValQCD_FlatPt_15_3000_1.root',
    '/store/cmst3/user/cbern/CMG/RelVal/4_1_2/RelValQCD_FlatPt_15_3000/patTuple_PATandPF2PAT_RelValQCD_FlatPt_15_3000_2.root',
    '/store/cmst3/user/cbern/CMG/RelVal/4_1_2/RelValQCD_FlatPt_15_3000/patTuple_PATandPF2PAT_RelValQCD_FlatPt_15_3000_3.root',
    ])


extension = 'jet'

# output module for EDM event (ntuple)
process.out.fileName = cms.untracked.string('tree_test_%s.root' % extension)
from CMGTools.Common.eventContent.everything_cff import everything 
process.out.outputCommands = cms.untracked.vstring( 'drop *')
process.out.outputCommands.extend( everything ) 
process.out.outputCommands.append( 'keep patJets_*_*_*' )    
# process.out.outputCommands.append( 'keep recoPFJets_*_*_*' )    
process.out.outputCommands.append( 'keep recoGenJets_*_*_*' )    

#output file for histograms etc
process.TFileService = cms.Service("TFileService",
                                   fileName = cms.string("histograms_test_%s.root" % extension))


# default analysis sequence    
process.load('CMGTools.Common.analysis_cff')

# note: we're reading ttbar events
process.load("CMGTools.Common.jet_cff")

process.p = cms.Path(
    process.jetSequence
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
