from PhysicsTools.PatAlgos.patTemplate_cfg import *
import FWCore.ParameterSet.Config as cms

# unit test for the electron analysis sequence 

process.setName_('ANA')

process.maxEvents = cms.untracked.PSet(
        input = cms.untracked.int32(2000)
        )

process.source.fileNames = cms.untracked.vstring(
    'file:/afs/cern.ch/user/c/cbern/public/patTuple_PATandPF2PAT.root'
    )

extension = 'electrons'

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

# now, we're going to tune the default analysis sequence to our needs
# by modifying the parameters of the modules present in this sequence. 

# Select events with 2 jets ...  
process.cmgPFJetCount.minNumber = 3
# with pT > 70.
process.cmgPFJetSel.cut = "pt()>50"
# and MET larger than 50
process.cmgPFMETSel.cut = "pt()>50"

# note: we're reading ttbar events

process.load("CMGTools.Common.skims.cmgElectronSel_cfi")
process.load("CMGTools.Common.skims.cmgElectronCount_cfi")
process.load("CMGTools.Common.histograms.cmgElectronHistograms_cfi")

process.electronSequence = cms.Sequence(
    process.cmgElectron +
    process.cmgElectronSel +
    process.cmgElectronCount +
    process.cmgElectronHistograms
    )

process.analysisSequence = cms.Sequence(
    process.electronSequence
    )

process.p = cms.Path(
    process.analysisSequence
)

process.MessageLogger.cerr.FwkReport.reportEvery = 100
process.options = cms.untracked.PSet( wantSummary = cms.untracked.bool(True) ) 

print 'input  : ', process.source.fileNames
print 'output :'
print '  tree :', process.out.fileName
print '  hist :', process.TFileService.fileName
