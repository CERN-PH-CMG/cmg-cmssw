from PhysicsTools.PatAlgos.patTemplate_cfg import *
import FWCore.ParameterSet.Config as cms

import pprint

#warning
process.maxEvents = cms.untracked.PSet(
        input = cms.untracked.int32(-1)
        )


selectEvents = False
doSkimHighMET = False
cut_highMET = 'pt()>50'


# sourceExt = 'StevenNov9'
# sourceExt = 'LM1'
# sourceExt = 'LM0'
# sourceExt = 'QCD_50to80'
# sourceExt = 'QCD_80to120'
# sourceExt = 'QCD_120to170'
# sourceExt = 'QCD_170to300'
sourceExt = 'QCD_300to470'
# sourceExt = 'QCD_470to600'
# sourceExt = 'QCD_600to800'
# sourceExt = 'QCD_800to1000'
# sourceExt = 'Zinvisible'
# sourceExt = 'TT'

# sourceExt = 'Z3J_100to300'
# sourceExt = 'Z3J_300to800'
# sourceExt = 'Z3J_800to1600'
# sourceExt = 'Test'

if sourceExt == 'StevenNov9':
    process.load("CMGTools.SusyJetMET.Sources.MultiJetRun2010BPromptReco3.susypat_cff")
if sourceExt == 'SueAnnHot':
#    process.load("CMGTools.SusyJetMET.Sources.HotSkims.sueann_highMHT_skim_cff")
    process.source.fileNames = cms.untracked.vstring('file:susypat_sueann_Hot_RA2.root')
if sourceExt == 'Test':
    process.source.fileNames = cms.untracked.vstring('file:susypat_RA2.root')

if sourceExt == 'LM0':
    process.load("CMGTools.SusyJetMET.Sources.LM0_SUSY_sftsht_7TeV_pythia6.Fall10_START38_V12_v1.GEN_SIM_RECO.source_cff")
if sourceExt == 'LM1':
    process.load("CMGTools.SusyJetMET.Sources.LM1_SUSY_sftsht_7TeV_pythia6.Fall10_START38_V12_v1.GEN_SIM_RECO.source_cff")

if sourceExt == 'PUQCD_120to170':
    process.load("CMGTools.SusyJetMET.Sources.QCD_Pt_120to170_TuneZ2_7TeV_pythia6.Fall10_E7TeV_ProbDist_2010Data_BX156_START38_V12_v1.GEN_SIM_RECO.source_cff")
if sourceExt == 'PUQCD_170to300':
    process.load("CMGTools.SusyJetMET.Sources.QCD_Pt_170to300_TuneZ2_7TeV_pythia6.Fall10_E7TeV_ProbDist_2010Data_BX156_START38_V12_v1.GEN_SIM_RECO.source_cff")
if sourceExt == 'PUQCD_300to470':
    process.load("CMGTools.SusyJetMET.Sources.QCD_Pt_300to470_TuneZ2_7TeV_pythia6.Fall10_E7TeV_ProbDist_2010Data_BX156_START38_V12_v1.GEN_SIM_RECO.source_cff")
if sourceExt == 'PUQCD_470to600':
    process.load("CMGTools.SusyJetMET.Sources.QCD_Pt_470to600_TuneZ2_7TeV_pythia6.Fall10_E7TeV_ProbDist_2010Data_BX156_START38_V12_v1.GEN_SIM_RECO.source_cff")
if sourceExt == 'PUQCD_600to800':
    process.load("CMGTools.SusyJetMET.Sources.QCD_Pt_600to800_TuneZ2_7TeV_pythia6.Fall10_E7TeV_ProbDist_2010Data_BX156_START38_V12_v1.GEN_SIM_RECO.source_cff")



if sourceExt == 'QCD_50to80':
    process.load("CMGTools.SusyJetMET.Sources.QCD_Pt_50to80_TuneZ2_7TeV_pythia6.Fall10_START38_V12_v1.GEN_SIM_RECO.source_cff")
if sourceExt == 'QCD_80to120':
    process.load("CMGTools.SusyJetMET.Sources.QCD_Pt_80to120_TuneZ2_7TeV_pythia6.Fall10_START38_V12_v1.GEN_SIM_RECO.source_cff")
if sourceExt == 'QCD_120to170':
    process.load("CMGTools.SusyJetMET.Sources.QCD_Pt_120to170_TuneZ2_7TeV_pythia6.Fall10_START38_V12_v1.GEN_SIM_RECO.source_cff")
if sourceExt == 'QCD_170to300':
    process.load("CMGTools.SusyJetMET.Sources.QCD_Pt_170to300_TuneZ2_7TeV_pythia6.Fall10_START38_V12_v1.GEN_SIM_RECO.source_cff")
if sourceExt == 'QCD_300to470':
    process.load("CMGTools.SusyJetMET.Sources.QCD_Pt_300to470_TuneZ2_7TeV_pythia6.Fall10_START38_V12_v1.GEN_SIM_RECO.source_cff")
if sourceExt == 'QCD_470to600':
    process.load("CMGTools.SusyJetMET.Sources.QCD_Pt_470to600_TuneZ2_7TeV_pythia6.Fall10_START38_V12_v1.GEN_SIM_RECO.source_cff")
if sourceExt == 'QCD_600to800':
    process.load("CMGTools.SusyJetMET.Sources.QCD_Pt_600to800_TuneZ2_7TeV_pythia6.Fall10_START38_V12_v2.GEN_SIM_RECO.source_cff")
if sourceExt == 'QCD_800to1000':
    process.load("CMGTools.SusyJetMET.Sources.QCD_Pt_800to1000_TuneZ2_7TeV_pythia6.Fall10_START38_V12_v1.GEN_SIM_RECO.source_cff")

if sourceExt == 'Zinvisible':
    process.load("CMGTools.SusyJetMET.Sources.ZinvisibleJets_madgraph.Spring10_START3X_V26_S09_v1.GEN_SIM_RECO.source_cff")
if sourceExt == 'TT':
    process.load("CMGTools.SusyJetMET.Sources.TTbarJets_madgraph.Spring10_START3X_V26_S09_v1.GEN_SIM_RECO.source_cff")


if sourceExt == 'Z3J_100to300': # ok
    process.load("CMGTools.SusyJetMET.Sources.Z3Jets_Pt100to300_alpgen.Spring10_START3X_V26_S09_v1.GEN_SIM_RECO.source_cff")
if sourceExt == 'Z3J_300to800': # Need to reprocess pattuples
    process.load("CMGTools.SusyJetMET.Sources.Z3Jets_Pt300to800_alpgen.Spring10_START3X_V26_S09_v1.GEN_SIM_RECO.source_cff")
if sourceExt == 'Z3J_800to1600': # ok
    process.load("CMGTools.SusyJetMET.Sources.Z3Jets_Pt800to1600_alpgen.Spring10_START3X_V26_S09_v1.GEN_SIM_RECO.source_cff")

# need to reprocess pattuples for the whole Z4Jets? 
if sourceExt == 'Z4J_100to300':
    process.load("CMGTools.SusyJetMET.Sources.Z4Jets_Pt100to300_alpgen.Spring10_START3X_V26_S09_v1.GEN_SIM_RECO.source_cff")
if sourceExt == 'Z4J_300to800':
    process.load("CMGTools.SusyJetMET.Sources.Z4Jets_Pt300to800_alpgen.Spring10_START3X_V26_S09_v1.GEN_SIM_RECO.source_cff")
if sourceExt == 'Z4J_800to1600': 
    process.load("CMGTools.SusyJetMET.Sources.Z4Jets_Pt800to1600_alpgen.Spring10_START3X_V26_S09_v1.GEN_SIM_RECO.source_cff")


# WJets

if sourceExt == 'W3J_100to300': # ok
    process.load("CMGTools.SusyJetMET.Sources.W3Jets_Pt100to300_alpgen.Spring10_START3X_V26_S09_v1.GEN_SIM_RECO.source_cff")
if sourceExt == 'W3J_300to800': # Need to reprocess pattuples
    process.load("CMGTools.SusyJetMET.Sources.W3Jets_Pt300to800_alpgen.Spring10_START3X_V26_S09_v1.GEN_SIM_RECO.source_cff")
if sourceExt == 'W3J_800to1600': # ok
    process.load("CMGTools.SusyJetMET.Sources.W3Jets_Pt800to1600_alpgen.Spring10_START3X_V26_S09_v1.GEN_SIM_RECO.source_cff")




ext = 'RA2_CMG'


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
process.load("RecoParticleFlow.PostProcessing.allPFMuons_cfi")

process.load("CMGTools.Common.runInfoAccounting_cfi")


process.cmgTuple = cms.Sequence(
    process.jetSequence +
    process.metSequence  
    #I don't have everything I need in the event content! need to keep all collections of reco tracks
    # process.selectGoodPFEventsSequence
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

