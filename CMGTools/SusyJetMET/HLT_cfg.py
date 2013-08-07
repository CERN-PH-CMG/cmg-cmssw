from PhysicsTools.PatAlgos.patTemplate_cfg import *
import FWCore.ParameterSet.Config as cms

import pprint



process = cms.Process("ANA")

process.maxEvents = cms.untracked.PSet(
        input = cms.untracked.int32(-1)
        )


# ext = 'sueAnn'
# ext = 'steven'
# ext = 'sueAnnHighRes'
# ext = 'sueAnnReReco39'
ext = 'testPatTuple'

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring()
)

if ext == 'sueAnn':
    process.load("CMGTools.SusyJetMET.Sources.HotSkims.sueann_highMHT_skim_cff" )
if ext == 'sueAnnHighRes':
    process.load("CMGTools.SusyJetMET.Sources.HotSkims.sueann_highRes_skim_cff" )
if ext == 'sueAnnReReco39':
    process.source.fileNames = cms.untracked.vstring('file:reco.root')
if ext == 'steven':
    process.source.fileNames = cms.untracked.vstring('file:HotSkims/steven.root')
if ext == 'LM0':
    process.source.fileNames = cms.untracked.vstring('file:LM0.root')
if ext == 'LM1':
    process.load("PFAnalyses.DijetAnalysis.Sources.LM1.Spring10_START3X_V26_S09_v1.GEN_SIM_RECO.source_cff")
if ext == 'testPatTuple':
    process.source.fileNames = cms.untracked.vstring('file:patuple_PFtest_withMuons.root')




print 'processing: '

from PFAnalyses.CommonTools.Tools.inputFiles import restrictInput as restrictInput
# process.source.fileNames = restrictInput( process.source.fileNames, 1)


print process.source.fileNames


process.cmgPFJetSelector = cms.EDFilter(
    "GenericPhysicsObjectSelector",
    src = cms.InputTag("cmgPFJet"),
    cut = cms.string("pt()>50 && component(1).fraction()>0.5"),
)


process.load("CMGTools.SusyJetMET.HLT_cff")
process.load("CMGTools.SusyJetMET.susyJetMETAnalyzer_cfi")
process.load("CMGTools.Common.jet_cff")

process.cmgPFJet.cfg.inputCollection = 'userJetsAK5PF'


process.analysisSequence = cms.Sequence(
    # process.susyJetMETAnalyzer
    process.cmgPFJet +
    process.cmgPFJetSelector + 
    process.HLT_HT100U  
    )


process.p = cms.Path( process.analysisSequence )


# output

from CMGTools.Common.EventContent.everything_cff import everything
process.out = cms.OutputModule(
    "PoolOutputModule",
    fileName = cms.untracked.string('patTuple_skim_%s.root' % ext ),
    # save only events passing the full path
    SelectEvents   = cms.untracked.PSet( SelectEvents = cms.vstring('p') ),
    # save PAT Layer 1 output; you need a '*' to
    # unpack the list of commands 'patEventContent'
#    outputCommands = cms.untracked.vstring('keep *',
#                                           'drop FlavorStructure_*_*_*')
    outputCommands = cms.untracked.vstring('drop *',
                                           'keep *_userJetsAK5PF_*_*',
                                           'keep *_hltPrimitives_*_*',
                                           'keep cmgPFJets_*_*_*')
    )


#process.out.outputCommands = cms.untracked.vstring(
#    'keep *',
#    'drop *_generalTracks_*_*',
#    'drop FlavorStructure_*_*_*',
#    'drop CaloTowersSorted_towerMaker_*_*'
#    )
    

process.outpath = cms.EndPath(process.out)

# message logger

process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = 1
process.options = cms.untracked.PSet( wantSummary = cms.untracked.bool(False) ) 

