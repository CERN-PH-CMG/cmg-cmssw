from PhysicsTools.PatAlgos.patTemplate_cfg import *
import FWCore.ParameterSet.Config as cms

import pprint


process.maxEvents = cms.untracked.PSet(
        input = cms.untracked.int32(-1)
        )

ext = 'MHT'

# process.source.fileNames = cms.untracked.vstring('file:Samples/MultiJetRun2010BPromptReco3/NoMETSkim/METgt50/susyJetMET_tree_MultiJetRun2010BPromptReco3_HighMET.root')

# process.load("CMGTools.SusyJetMET.Sources.MultiJetRun2010BPromptReco3.localTree_cff")

# output to be stored


sourceExt = 'StevenNov9'
# sourceExt = 'LM1'
# sourceExt = 'LM0'
# sourceExt = 'QCD_50to80'
# sourceExt = 'QCD_80to120'
# sourceExt = 'QCD_120to170'
# sourceExt = 'QCD_170to300'
# sourceExt = 'QCD_300to470'
# sourceExt = 'QCD_470to600'
# sourceExt = 'QCD_600to800'
# sourceExt = 'QCD_800to1000'
# sourceExt = 'Test'

if sourceExt == 'StevenNov9': #
    process.load("CMGTools.SusyJetMET.Sources.MultiJetRun2010BPromptReco3.NoMETSkim.source_cff")


if sourceExt == 'SueAnnHot':
#    process.load("CMGTools.SusyJetMET.Sources.HotSkims.sueann_highMHT_skim_cff")
    process.source.fileNames = cms.untracked.vstring('file:susypat_sueann_Hot_RA2.root')
if sourceExt == 'Test':
    process.source.fileNames = cms.untracked.vstring('file:susypat_RA2.root')
    
if sourceExt == 'LM0': #
    process.load("CMGTools.SusyJetMET.Sources.LM0_SUSY_sftsht_7TeV_pythia6.Fall10_START38_V12_v1.GEN_SIM_RECO.NoMETSkim.source_cff")
if sourceExt == 'LM1': #
    process.load("CMGTools.SusyJetMET.Sources.LM1_SUSY_sftsht_7TeV_pythia6.Fall10_START38_V12_v1.GEN_SIM_RECO.NoMETSkim.source_cff")

if sourceExt == 'QCD_50to80': #
    process.load("CMGTools.SusyJetMET.Sources.QCD_Pt_50to80_TuneZ2_7TeV_pythia6.Fall10_START38_V12_v1.GEN_SIM_RECO.NoMETSkim.source_cff")
if sourceExt == 'QCD_80to120': #
    process.load("CMGTools.SusyJetMET.Sources.QCD_Pt_80to120_TuneZ2_7TeV_pythia6.Fall10_START38_V12_v1.GEN_SIM_RECO.NoMETSkim.source_cff")
if sourceExt == 'QCD_120to170': # 
    process.load("CMGTools.SusyJetMET.Sources.QCD_Pt_120to170_TuneZ2_7TeV_pythia6.Fall10_START38_V12_v1.GEN_SIM_RECO.NoMETSkim.source_cff")
if sourceExt == 'QCD_170to300': #
    process.load("CMGTools.SusyJetMET.Sources.QCD_Pt_170to300_TuneZ2_7TeV_pythia6.Fall10_START38_V12_v1.GEN_SIM_RECO.PatTuple.NoMETSkim.source_cff")
if sourceExt == 'QCD_300to470': # 
    process.load("CMGTools.SusyJetMET.Sources.QCD_Pt_300to470_TuneZ2_7TeV_pythia6.Fall10_START38_V12_v1.GEN_SIM_RECO.PatTuple.NoMETSkim.source_cff")
if sourceExt == 'QCD_470to600': # 
    process.load("CMGTools.SusyJetMET.Sources.QCD_Pt_470to600_TuneZ2_7TeV_pythia6.Fall10_START38_V12_v1.GEN_SIM_RECO.NoMETSkim.source_cff")
if sourceExt == 'QCD_600to800': #
    process.load("CMGTools.SusyJetMET.Sources.QCD_Pt_600to800_TuneZ2_7TeV_pythia6.Fall10_START38_V12_v1.GEN_SIM_RECO.NoMETSkim.source_cff")
if sourceExt == 'QCD_800to1000': # 
    process.load("CMGTools.SusyJetMET.Sources.QCD_Pt_800to1000_TuneZ2_7TeV_pythia6.Fall10_START38_V12_v1.GEN_SIM_RECO.NoMETSkim.source_cff")



print 'processing:'


# from CMGTools.Common.Tools.inputFiles import restrictInput as restrictInput
# process.source.fileNames = restrictInput( process.source.fileNames, 1)

process.setName_( ext )

print process.source.fileNames


process.load('CMGTools.Common.met_cff')


def adaptToCMGInput( module, newInput ):
    module.cfg.inputCollection = newInput

adaptToCMGInput( process.cmgMHTPFJets, 'cmgPFBaseJet')
adaptToCMGInput( process.cmgMHTPFJets30, 'cmgPFBaseJet')
adaptToCMGInput( process.cmgMHTPFJets50, 'cmgPFBaseJet')
adaptToCMGInput( process.cmgMHTCaloJets, 'cmgCaloBaseJet')
adaptToCMGInput( process.cmgMHTCaloJets30, 'cmgCaloBaseJet')
adaptToCMGInput( process.cmgMHTCaloJets50, 'cmgCaloBaseJet')

process.p = cms.Path(
    process.cmgMHTPFJets +
    process.cmgMHTPFJets30 +
    process.cmgMHTPFJets50 +
    process.cmgMHTCaloJets +    
    process.cmgMHTCaloJets30 +    
    process.cmgMHTCaloJets50 +
    #
    process.mhtPFJetsLorentzVector +
    process.mhtPFJets30LorentzVector +
    process.mhtPFJets50LorentzVector +
    process.mhtCaloJetsLorentzVector +
    process.mhtCaloJets30LorentzVector +
    process.mhtCaloJets50LorentzVector 
    )

process.TFileService = cms.Service(
    "TFileService",
    fileName = cms.string("susyJetMET_histograms_%s.root" %  ext )
    )


process.out.fileName = cms.untracked.string('susyJetMET_tree_%s.root' %  ext )
process.out.outputCommands = cms.untracked.vstring('keep *')

print process.out.dumpPython()

# message logger setup 
process.MessageLogger.cerr.FwkReport.reportEvery = 1000
process.options = cms.untracked.PSet( wantSummary = cms.untracked.bool(True) )

