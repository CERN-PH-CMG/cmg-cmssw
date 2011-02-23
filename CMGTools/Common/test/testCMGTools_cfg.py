from PhysicsTools.PatAlgos.patTemplate_cfg import *
import FWCore.ParameterSet.Config as cms

process.setName_('ANA')

process.maxEvents = cms.untracked.PSet(
        input = cms.untracked.int32(-1)
        )

process.source.fileNames = cms.untracked.vstring(
    'file:/afs/cern.ch/user/c/cbern/public/patTuple_PATandPF2PAT.root'
    )


# output module for EDM event (ntuple)
process.out.fileName = cms.untracked.string('testCMGTools.root')
from CMGTools.Common.eventContent.particleFlow_cff import particleFlow as particleFlowEventContent  
from CMGTools.Common.eventContent.particleFlow_cff import particleFlowBase as particleFlowEventContentBase  
from CMGTools.Common.eventContent.traditional_cff import traditional as traditionalEventContent  
process.out.outputCommands = cms.untracked.vstring( 'drop *')
process.out.outputCommands.extend( particleFlowEventContent ) 
process.out.outputCommands.extend( particleFlowEventContentBase )
process.out.outputCommands.extend( traditionalEventContent ) 
process.out.outputCommands.append( 'keep cmgBaseMETs_*_*_*' )
    

#output file for histograms etc
process.TFileService = cms.Service("TFileService",
                                   fileName = cms.string("histograms.root"))


    
process.load('CMGTools.Common.analysis_cff')

process.cmgPFJetSel.verbose = cms.untracked.bool( False )

process.baseMETSelector = cms.EDFilter(
    'BaseMETSelector',
    src = cms.InputTag('cmgMHTPFJets'),
    cut = cms.string('pt()>20')
    )

process.analysisSequence = cms.Sequence(
    process.analysis 
    )

process.p = cms.Path(
    process.analysisSequence
)

process.MessageLogger.cerr.FwkReport.reportEvery = 100
process.options = cms.untracked.PSet( wantSummary = cms.untracked.bool(False) ) 
