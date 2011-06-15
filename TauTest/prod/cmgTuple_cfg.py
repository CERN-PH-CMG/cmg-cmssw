from PhysicsTools.PatAlgos.patTemplate_cfg import *
import FWCore.ParameterSet.Config as cms
import os.path


runOnMC = True
process.setName_('CMG')
process.maxEvents = cms.untracked.PSet(input = cms.untracked.int32(100))
process.MessageLogger.cerr.FwkReport.reportEvery = 10
process.source.fileNames = cms.untracked.vstring(['file:patTuple.root'])
process.out.fileName = cms.untracked.string(os.path.expandvars('file:cmgTuple.root'))


##
process.load('CMGTools.Common.tau_cff')
process.tauFactory.inputCollection = cms.InputTag("selectedPatTausAK5")
process.cmgTau.cfg = process.tauFactory.clone()


######
process.analysisSequence = cms.Sequence(process.tauSequence
                                        )
process.p = cms.Path(process.analysisSequence)


########
process.out.outputCommands = cms.untracked.vstring( 'drop *')
process.out.outputCommands.append( 'keep cmgTaus_cmgTauSel__CMG' )


