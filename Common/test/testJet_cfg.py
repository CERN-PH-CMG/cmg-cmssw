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


# process.load("CMGTools.Common.sources.relval.RelValQCD_FlatPt_15_3000.CMSSW_3_11_2.MC_311_V2.source_cff")
process.load("CMGTools.Common.sources.relval.RelValQCD_FlatPt_15_3000.CMSSW_3_11_2.MC_311_V2.NoPileUpRemoval.source_cff")
# process.load("CMGTools.Common.sources.relval.RelValQCD_FlatPt_15_3000.CMSSW_4_1_2.MC_311_V2.source_cff")

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
process.load('CMGTools.Common.skims.patJetLead_cfi')

process.p = cms.Path(
    process.jetSequence +
    process.patJetLead
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
