from PhysicsTools.PatAlgos.patTemplate_cfg import *
import FWCore.ParameterSet.Config as cms

process.maxEvents = cms.untracked.PSet(
        input = cms.untracked.int32(-1)
)
process.source.fileNames = cms.untracked.vstring(
        'file:patTuple_PATandPF2PAT.root'
)
process.setName_('CMG')
process.out.fileName = cms.untracked.string('cmgTuple.root')

process.load('CMGTools.Common.cutsummary_cff')
process.load('CMGTools.Common.muon_cff')
process.load('CMGTools.Common.diMuon_cff')
process.load('CMGTools.Common.jet_cff')
process.load('CMGTools.Common.runInfoAccounting_cfi')
##process.zmumusummary = process.cutsummary.clone(inputCollection = cms.InputTag("cmgDiMuon"))

#config the pat trigger matching
process.load('CMGTools.ZmumuJetsTutorial.patTriggerMatching_cff')
process.cmgMuon.cfg.inputCollection = cms.InputTag("triggeredPatMuons")

from CMGTools.Common.EventContent.particleFlow_cff import particleFlow as particleFlowEventContent  
from CMGTools.Common.EventContent.particleFlow_cff import particleFlowBase as particleFlowEventContentBase  
process.out.outputCommands = cms.untracked.vstring( 'drop *')
process.out.outputCommands.extend( particleFlowEventContent ) 
process.out.outputCommands.extend( particleFlowEventContentBase )
process.out.outputCommands.append( 'keep *_triggeredPatMuons_*_*' ) # we keep the pat::Muon

#output file for histograms etc
process.TFileService = cms.Service("TFileService",
                                   fileName = cms.string("histograms.root"))

### Run the trigger matching selections
##from CMGTools.ZmumuJetsTutorial.selections.muontrigger_cfi import muontrigger
##process.cmgMuon.cuts.muontrigger = muontrigger.clone()
###

###The jet counting histograms
process.load('CMGTools.ZmumuJetsTutorial.histograms.jetcounting_cfi')
###

process.analysisSequence = cms.Sequence(
    process.patMuonTrigger +
    process.muonSequence + 
    process.diMuonSequence +
    process.pfJetSequence +
    process.runInfoAccounting + 
    process.cutsummary## + 
##    process.zmumusummary
##    process.jetCounting
)
process.p = cms.Path(process.analysisSequence)
process.MessageLogger.cerr.FwkReport.reportEvery = 100
