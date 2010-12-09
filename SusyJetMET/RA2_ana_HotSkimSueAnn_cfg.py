from PhysicsTools.PatAlgos.patTemplate_cfg import *
import FWCore.ParameterSet.Config as cms

import pprint


process.maxEvents = cms.untracked.PSet(
        input = cms.untracked.int32(-1)
        )

# sourceExt = 'SueAnnHot'
sourceExt = 'LM1'

if sourceExt == 'SueAnnHot':
    process.load('CMGTools.SusyJetMET.Sources.QCD_SueAnn_HotSkim.SusyPatPFPath.allHotSkims_cff')
    from CMGTools.SusyJetMET.Sources.QCD_SueAnn_HotSkim.hotSkim_cff import hotSkim
    hotSkim( process.source, sourceExt ) 
if sourceExt == 'StevenHot':
    sourceExt = 'StevenHot'
    process.load("CMGTools.SusyJetMET.Sources.Data.HotSkim.source_cff")
if sourceExt == 'LM1':
    process.load("CMGTools.SusyJetMET.Sources.LM1_SUSY_sftsht_7TeV_pythia6.Fall10_START38_V12_v1.GEN_SIM_RECO.RECO.SusyPat.susypat_cff")
    

ext = 'RA2_CMG_InconsMuon'

# processing steps
doSkimHighMET = False
cut_highMET = 'pt()>50'
selectEvents = False

# output to be stored

print 'processing:'

process.setName_('ANA')

print process.source.fileNames

outFileNameExt = ext + '_' + sourceExt 

# reinitializing stuff defined in patTemplate_cfg
process.out.outputCommands = cms.untracked.vstring( 'drop *' )
process.out.SelectEvents.SelectEvents = cms.vstring()

process.load("CMGTools.Common.countingSequences_cff")


from CMGTools.SusyJetMET.EventContent.susyJetMET_cff import susyJetMET
process.out.outputCommands += susyJetMET    
 
process.load('CMGTools.Common.jet_cff')
process.load('CMGTools.Common.met_cff')

from CMGTools.SusyJetMET.adaptCMGtoRA2_cff import adaptCMGtoRA2
# if sourceExt == 'StevenNov9':
adaptCMGtoRA2(process)
# else if sourceExt == 'SueAnnHot':
#    adaptCMGtoRECO(process)

# delta pt filter
process.load("RecoParticleFlow.PostProcessing.selectGoodPFEvents_cff")
process.load("RecoParticleFlow.PostProcessing.selectEventsWithSmallDeltaPtMuons_cff")
process.load("RecoParticleFlow.PostProcessing.allPFMuons_cfi")

process.load("CMGTools.Common.runInfoAccounting_cfi")



process.cmgTuple = cms.Sequence(
    process.jetSequence +
    process.metSequence +
    process.allPFMuons +
    # the filtering should be done at the end so that we have the objects to study it!
    # not necessary
    # process.selectEventsWithSmallDeltaPtMuons + 
    process.largeDeltaPtMuons +
    cms.ignore( process.filterLargeDeltaPtMuons ) + 
    process.selectGoodPFEventsSequence 
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

