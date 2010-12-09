from PhysicsTools.PatAlgos.patTemplate_cfg import *
import FWCore.ParameterSet.Config as cms

import pprint

process.out.SelectEvents.SelectEvents = cms.vstring()


process.maxEvents = cms.untracked.PSet(
        input = cms.untracked.int32(-1)
        )


ext = 'RA2'

doPATTuple = True
selectEvents = True
selectForDisplay = 'Greedy'
Spring10 = False

process.setName_('PAT2')

print 'processing:'

sourceExt = 'SueAnnHot'
# sourceExt = 'LM1'

if sourceExt == 'SueAnnHot':
    process.load("CMGTools.SusyJetMET.Sources.HotSkims.sueann_highMHT_skim_cff")
if sourceExt == 'SueAnnHot_0to5':
    process.load("CMGTools.SusyJetMET.Sources.HotSkims.sueann_highMHT_skim_0to5_cff")
if sourceExt == 'SueAnnHot_5to15':
    process.load("CMGTools.SusyJetMET.Sources.HotSkims.sueann_highMHT_skim_5to15_cff")
if sourceExt == 'SueAnnHot_15to30':
    process.load("CMGTools.SusyJetMET.Sources.HotSkims.sueann_highMHT_skim_15to30_cff")
if sourceExt == 'SueAnnHot_30to50':
    process.load("CMGTools.SusyJetMET.Sources.HotSkims.sueann_highMHT_skim_30to50_cff")
if sourceExt == 'SueAnnHot_50to80':
    process.load("CMGTools.SusyJetMET.Sources.HotSkims.sueann_highMHT_skim_50to80_cff")
if sourceExt == 'SueAnnHot_80to120':
    process.load("CMGTools.SusyJetMET.Sources.HotSkims.sueann_highMHT_skim_80to120_cff")
if sourceExt == 'SueAnnHot_120to170':
    process.load("CMGTools.SusyJetMET.Sources.HotSkims.sueann_highMHT_skim_120to170_cff")
if sourceExt == 'SueAnnHot_170to300':
    process.load("CMGTools.SusyJetMET.Sources.HotSkims.sueann_highMHT_skim_170to300_cff")
if sourceExt == 'SueAnnHot_300to470':
    process.load("CMGTools.SusyJetMET.Sources.HotSkims.sueann_highMHT_skim_300to470_cff")
if sourceExt == 'SueAnnHot_470to600':
    process.load("CMGTools.SusyJetMET.Sources.HotSkims.sueann_highMHT_skim_470to600_cff")
if sourceExt == 'SueAnnHot_600to800':
    process.load("CMGTools.SusyJetMET.Sources.HotSkims.sueann_highMHT_skim_600to800_cff")
if sourceExt == 'SueAnnHot_800to1000':
    process.load("CMGTools.SusyJetMET.Sources.HotSkims.sueann_highMHT_skim_800to1000_cff")
if sourceExt == 'SueAnnHot_1000to1400':
    process.load("CMGTools.SusyJetMET.Sources.HotSkims.sueann_highMHT_skim_1000to1400_cff")
if sourceExt == 'SueAnnHot_1400to1800':
    process.load("CMGTools.SusyJetMET.Sources.HotSkims.sueann_highMHT_skim_1400to1800_cff")
if sourceExt == 'SueAnnHot_1800toInf':
    process.load("CMGTools.SusyJetMET.Sources.HotSkims.sueann_highMHT_skim_1800toInf_cff")
if sourceExt == 'LM1':
    process.load("CMGTools.SusyJetMET.Sources.LM1_SUSY_sftsht_7TeV_pythia6.Fall10_START38_V12_v1.GEN_SIM_RECO.RECO.source_cff")


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

process.load("RecoParticleFlow.PostProcessing.METCorrelationFilter_cfi")
process.p = cms.Path(
    process.startupSequence +
    #WARNING
    # process.METCorrelationFilter + 
    process.pfseq +
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




