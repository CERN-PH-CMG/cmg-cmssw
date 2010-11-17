from PhysicsTools.PatAlgos.patTemplate_cfg import *
import FWCore.ParameterSet.Config as cms

import pprint


process.maxEvents = cms.untracked.PSet(
        input = cms.untracked.int32(-1)
        )

ext = 'PruneJet50'

# process.source.fileNames = cms.untracked.vstring('file:Samples/MultiJetRun2010BPromptReco3/NoMETSkim/METgt50/susyJetMET_tree_MultiJetRun2010BPromptReco3_HighMET.root')

process.load("CMGTools.SusyJetMET.Sources.MultiJetRun2010BPromptReco3.localTree_cff")

# output to be stored

print 'processing:'


# from CMGTools.Common.Tools.inputFiles import restrictInput as restrictInput
# process.source.fileNames = restrictInput( process.source.fileNames, 1)

process.setName_('PRUNE')

print process.source.fileNames


process.pfJetSelector = cms.EDFilter(
    "PFJetSelector",
    src = cms.InputTag("cmgPFJet"),
    cut = cms.string("pt()>50"),
    )

process.p = cms.Path(
    process.pfJetSelector
    )


process.out.fileName = cms.untracked.string('susyJetMET_tree_%s.root' %  ext )
process.out.outputCommands = cms.untracked.vstring('keep *')

print process.out.dumpPython()

# message logger setup 
process.MessageLogger.cerr.FwkReport.reportEvery = 1000
process.options = cms.untracked.PSet( wantSummary = cms.untracked.bool(True) )

