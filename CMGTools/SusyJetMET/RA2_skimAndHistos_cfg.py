from PhysicsTools.PatAlgos.patTemplate_cfg import *
import FWCore.ParameterSet.Config as cms

import pprint


process.maxEvents = cms.untracked.PSet(
        input = cms.untracked.int32(-1)
        )

# process.source.fileNames = cms.untracked.vstring('rfio:/castor/cern.ch/user/l/lowette/grid/MultiJetRun2010BPromptReco3/susypat_74_1_yMC.root')

sourceExt = 'MultiJetRun2010BPromptReco3'
if sourceExt == 'MultiJetRun2010BPromptReco3':
    process.load("CMGTools.SusyJetMET.Sources.MultiJetRun2010BPromptReco3.localTree_cff")


doSkimHighMET = True
cut_highMET = 'pt()>50'
saveTree = True

# output to be stored

print 'processing:'


# from CMGTools.Common.Tools.inputFiles import restrictInput as restrictInput
# process.source.fileNames = restrictInput( process.source.fileNames, 1)

process.setName_('HIST')

print process.source.fileNames

outFileNameExt = sourceExt

process.load("CMGTools.Common.countingSequences_cff")

process.load('CMGTools.Common.jet_cff')
process.load('CMGTools.Common.met_cff')

process.cmgTuple = cms.Sequence()

if doSkimHighMET:
    outFileNameExt += '_HighMET'
    process.load('CMGTools.Common.Skims.selEventsHighMET_cff')
    process.cmgTuple += process.selEventsHighMETSequence
    process.highMET.cut = cut_highMET

process.cmgTuple += cms.Sequence(
    process.jetHistograms +
    process.metHistograms
    )

process.p = cms.Path(
    process.cmgTuple 
    )


process.out.SelectEvents.SelectEvents.append('p')
process.out.fileName = cms.untracked.string('susyJetMET_tree_%s.root' %  outFileNameExt)
if saveTree:
    process.out.outputCommands = cms.untracked.vstring('keep *')

process.TFileService = cms.Service(
    "TFileService",
    fileName = cms.string("susyJetMET_histograms_%s.root" %  outFileNameExt )
    )

print process.out.dumpPython()

print 'output file: ', process.out.fileName
# print process.schedule

# message logger setup 
process.MessageLogger.cerr.FwkReport.reportEvery = 1000
process.options = cms.untracked.PSet( wantSummary = cms.untracked.bool(True) )

