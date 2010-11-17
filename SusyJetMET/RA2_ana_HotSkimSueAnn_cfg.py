from PhysicsTools.PatAlgos.patTemplate_cfg import *
import FWCore.ParameterSet.Config as cms

import pprint


process.maxEvents = cms.untracked.PSet(
        input = cms.untracked.int32(-1)
        )

# process.source.fileNames = cms.untracked.vstring('rfio:/castor/cern.ch/user/l/lowette/grid/MultiJetRun2010BPromptReco3/susypat_74_1_yMC.root')

# sourceExt = 'StevenNov9'
# sourceExt = 'Test'
sourceExt = 'SueAnnHot'

if sourceExt == 'StevenNov9':
    process.load("CMGTools.SusyJetMET.Sources.Data.Steven.Nov9.susypat_cff")
if sourceExt == 'SueAnnHot':
    #    process.load("CMGTools.SusyJetMET.Sources.HotSkims.sueann_highMHT_skim_cff")
    # process.source.fileNames = cms.untracked.vstring('file:susypat_sueann_Hot_RA2.root')
    process.source.fileNames = cms.untracked.vstring('file:susypat_RA2.root')
if sourceExt == 'Test':
    process.source.fileNames = cms.untracked.vstring('file:susypat_RA2.root')

ext = 'RA2_CMG'

# processing steps
doSkimHighMET = False
cut_highMET = 'pt()>50'
selectEvents = False

# output to be stored

print 'processing:'


# from CMGTools.Common.Tools.inputFiles import restrictInput as restrictInput
# process.source.fileNames = restrictInput( process.source.fileNames, 1)

process.setName_('ANA')

print process.source.fileNames

outFileNameExt = ext

# reinitializing stuff defined in patTemplate_cfg
process.out.outputCommands = cms.untracked.vstring( 'drop *' )
process.out.SelectEvents.SelectEvents = cms.vstring()

process.load("CMGTools.Common.countingSequences_cff")


from CMGTools.Common.EventContent.everything_cff import everything
process.out.outputCommands += everything    
    
process.load('CMGTools.Common.jet_cff')
process.load('CMGTools.Common.met_cff')

from CMGTools.SusyJetMET.adaptCMGtoRA2_cff import adaptCMGtoRA2
# if sourceExt == 'StevenNov9':
adaptCMGtoRA2(process)
# else if sourceExt == 'SueAnnHot':
#    adaptCMGtoRECO(process)
    
process.load("RecoParticleFlow.PostProcessing.selectGoodPFEvents_cff")
process.load("RecoParticleFlow.PostProcessing.allPFMuons_cfi")
process.load("CMGTools.Common.runInfoAccounting_cfi")


process.load("SandBox.Skims.badPFMuonFilter_cfi")

process.cmgTuple = cms.Sequence(
    process.selectGoodPFEventsSequence +
    process.jetSequence +
    process.metSequence +
    process.allPFMuons 
    )
 
if doSkimHighMET:
    outFileNameExt += '_HighMET'
    process.load('CMGTools.Common.Skims.selEventsHighMET_cff')
    process.cmgTuple += process.selEventsHighMETSequence
    process.highMET.cut = cut_highMET


process.load("CMGTools.Common.runInfoAccounting_cfi")
process.p = cms.Path(
    process.runInfoAccounting + # must be the first in the path!
    process.cmgTuple 
    )


from CMGTools.SusyJetMET.EventContent.METTailStudies_cff import METTailStudies
process.out.outputCommands += METTailStudies
process.out.outputCommands.append('keep *_TriggerResults_*_*') 
process.out.outputCommands.append( 'keep recoCaloMETs_*_*_*' )
process.out.outputCommands.append( 'keep recoPFMETs_*_*_*' )

pprint.pprint(process.out.outputCommands)

#process.dump = cms.EDAnalyzer("EventContentAnalyzer")
# process.p += process.dump

if selectEvents:
    process.out.SelectEvents.SelectEvents.append('p')
process.out.fileName = cms.untracked.string('susyJetMET_tree_%s.root' %  outFileNameExt)

process.TFileService = cms.Service(
    "TFileService",
    fileName = cms.string("susyJetMET_histograms_%s.root" %  outFileNameExt )
    )

print process.out.dumpPython()

print 'output file: ', process.out.fileName
# print process.schedule

# message logger setup 
process.MessageLogger.cerr.FwkReport.reportEvery = 10
process.options = cms.untracked.PSet( wantSummary = cms.untracked.bool(True) )

