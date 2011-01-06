from PhysicsTools.PatAlgos.patTemplate_cfg import *
import FWCore.ParameterSet.Config as cms

import pprint,sys

process.out.SelectEvents.SelectEvents = cms.vstring()


process.maxEvents = cms.untracked.PSet(
        input = cms.untracked.int32(-1)
        )


ext = 'RA2'

doPATTuple = True
selectEvents = True
selectForDisplay = None
Spring10 = False

process.setName_('PAT2')

print 'processing:'

sourceExt = 'hotskim_QCD'
# sourceExt = 'LM1'
# sourceExt = 'hotskim_Data'

if sourceExt.find('hotskim_QCD')!=-1:
    process.load("CMGTools.SusyJetMET.Sources.QCD_SueAnn_HotSkim.sueann_highMHT_skim_cff")
    from CMGTools.SusyJetMET.Sources.QCD_SueAnn_HotSkim.hotSkim_cff import hotSkim
    hotSkim( process.source, sourceExt ) 
elif sourceExt == 'LM1':
    process.load("CMGTools.SusyJetMET.Sources.LM1_SUSY_sftsht_7TeV_pythia6.Fall10_START38_V12_v1.GEN_SIM_RECO.RECO.source_cff")
# elif sourceExt == 'hotskim_Data':
#    process.load("CMGTools.SusyJetMET.Sources.Data.HotSkim.source_cff")    
else:
    print 'no such source definition: ', sourceExt
    sys.exit(1)

# process.source.eventsToProcess = cms.untracked.VEventRange( '1:243087' )

print process.source.fileNames


if selectForDisplay!=None:
    from CMGTools.SusyJetMET.selectEvents_cff import selectEvents as addVEventRange
    addVEventRange( selectForDisplay, process.source )



outFileNameExt = ext + '_' + sourceExt

# reinitializing stuff defined in patTemplate_cfg

process.load("CMGTools.Common.countingSequences_cff")

# depending on what we want to do, the various sequences will be added to this path.

    
from CMGTools.SusyJetMET.RA2OfficialSequence_cff import addSUSYPATRA2
# for Fall10 samples 
# addSUSYPATRA2( process, 'REDIGI38X')

# WARNING

if Spring10:
    addSUSYPATRA2( process, 'REDIGI')
    process.patJets.addTagInfos     = False
    process.patJets.addBTagInfo = False
else: #Fall 10
    addSUSYPATRA2( process, 'REDIGI38X')

process.pfseq.remove( process.badPFMuonFilter ) 

 
process.pcalo = cms.Path(
    process.caloseq
    )

# loading filters
process.load("RecoParticleFlow.PostProcessing.METCorrelationFilter_cfi")
process.load("CMGTools.SusyJetMET.eventCleaningFilters_cff")

# adding counters and filters to the particle flow path

process.p = cms.Path(
    process.startupSequence +
    #WARNING
    # process.METCorrelationFilter +
    # process.eventCleaningFiltersIgnore +
    process.pfseq +
    process.eventCleaningFilters +
    process.finalSequence  
    )
process.outpath += process.saveHistosInRunInfo

process.schedule = cms.Schedule(
    process.pcalo,
    process.p,
    process.outpath
    )

#process.dump = cms.EDAnalyzer("EventContentAnalyzer")
# process.p += process.dump


if selectEvents:
    # IMPORTANT:
    # pcalo lets a new tail event enter. It is probably filtered in the pf path, but I don't know where....
    
    # process.out.SelectEvents.SelectEvents.append('pcalo')
    process.out.SelectEvents.SelectEvents.append('p')

process.out.fileName = cms.untracked.string('susypat_%s.root' %  outFileNameExt)
process.out.outputCommands = cms.untracked.vstring( 'drop *' )

from CMGTools.SusyJetMET.EventContent.RA2Official_cff import RA2Official
process.out.outputCommands += RA2Official  
process.out.outputCommands.append( 'keep recoCaloMETs_*_*_*' )
process.out.outputCommands.append( 'keep recoPFMETs_*_*_*' )

from CMGTools.Common.EventContent.runInfoAccounting_cff import runInfoAccounting
process.out.outputCommands += runInfoAccounting

# process.out.outputCommands.append('keep *')

#process.TFileService = cms.Service(
#    "TFileService",
#    fileName = cms.string("susyJetMET_histograms_%s.root" %  outFileNameExt )
#    )

print process.out.dumpPython()

print 'output file: ', process.out.fileName


# print process.schedule

# message logger setup 
process.MessageLogger.cerr.FwkReport.reportEvery = 1
process.options = cms.untracked.PSet( wantSummary = cms.untracked.bool(True) )




