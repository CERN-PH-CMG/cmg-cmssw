from PhysicsTools.PatAlgos.patTemplate_cfg import *
import FWCore.ParameterSet.Config as cms

import os.path

process.maxEvents = cms.untracked.PSet(
        input = cms.untracked.int32(-1)
)
process.source.fileNames = cms.untracked.vstring(
        os.path.expandvars('file:/tmp/${USER}/patTuple_PATandPF2PAT.root')
)
process.setName_('CMG')
process.out.fileName = cms.untracked.string('cmgTuple.root')

process.load('CMGTools.Common.cutsummary_cff')
process.load('CMGTools.Common.muon_cff')
process.load('CMGTools.Common.diMuon_cff')
process.load('CMGTools.Common.jet_cff')
process.load('CMGTools.Common.runInfoAccounting_cfi')

#config the pat trigger matching
process.load('CMGTools.ZmumuJetsTutorial.patTriggerMatching_cff')
process.cmgMuon.cfg.inputCollection = cms.InputTag("triggeredPatMuons")

from CMGTools.Common.eventContent.particleFlow_cff import particleFlow as particleFlowEventContent  
from CMGTools.Common.eventContent.particleFlow_cff import particleFlowBase as particleFlowEventContentBase  
process.out.outputCommands = cms.untracked.vstring( 'drop *')
process.out.outputCommands.extend( particleFlowEventContent ) 
process.out.outputCommands.extend( particleFlowEventContentBase )
process.out.outputCommands.append( 'keep *_triggeredPatMuons_*_*' ) # we keep the pat::Muon

#output file for histograms etc
process.TFileService = cms.Service("TFileService",
                                   fileName = cms.string("histograms.root"))

### Produce a summary of cuts on the DiMuon
##process.zmumusummary = process.cutsummary.clone(inputCollection = cms.InputTag("cmgDiMuon"))

### Run the trigger matching selections
##from CMGTools.ZmumuJetsTutorial.selections.muontrigger_cfi import muontrigger
##process.cmgMuon.cuts.muontrigger = muontrigger.clone()
###

###The jet counting histograms
##process.load('CMGTools.ZmumuJetsTutorial.histograms.jetcounting_cfi')
##process.load('CMGTools.ZmumuJetsTutorial.skims.selEventsPFJet_cff')
##process.jetCountingClean = process.jetCounting.clone(inputCollection = cms.InputTag("jetIDJet"))
##process.jetCountingSequence = cms.Sequence(process.jetCounting+process.jetIDJet+process.jetCountingClean)
### The Z skimming
##process.load('CMGTools.ZmumuJetsTutorial.skims.selEventsZ_cff')
###

process.analysisSequence = cms.Sequence(
    process.patMuonTrigger +
    process.muonSequence + 
    process.diMuonSequence +
    process.pfJetSequence +
    process.runInfoAccounting +
    process.cutsummary## + 
##    process.zmumusummary +
##    process.jetCountingSequence +
##    process.selectedZSequence
)
process.p = cms.Path(process.analysisSequence)
process.MessageLogger.cerr.FwkReport.reportEvery = 100
