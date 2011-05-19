from PhysicsTools.PatAlgos.patTemplate_cfg import *
import FWCore.ParameterSet.Config as cms
import os.path


runOnMC = True
process.setName_('CMG')


#from CMGTools.TauTest.getGlobalTag import getGlobalTag
#process.GlobalTag.globaltag = cms.string(getGlobalTag(runOnMC))


#process.source.fileNames = cms.untracked.vstring(os.path.expandvars('file:/tmp/${USER}/patTuple.root'))

#from CMGTools.Common.sources.relval.RelValZTT.CMSSW_4_1_4.START311_V2.source_cff import *
#process.load('CMGTools.Common.sources.relval.RelValZTT.CMSSW_4_1_4.START311_V2.source_cff')

#from CMGTools.TauTest.sources.PAT.GluGluHToTauTau_M_115.source_cff import *
#process.load('CMGTools.TauTest.PAT.sources.GluGluHToTauTau_M_115.source_cff')

#from CMGTools.TauTest.sources.PAT.DYToTauTau.source_cff import *
#process.load('CMGTools.TauTest.PAT.sources.DYToTauTau.source_cff')

#from CMGTools.TauTest.sources.PAT.RelValZTT.source_cff import *
#process.load('CMGTools.TauTest.sources.PAT.RelValZTT.source_cff')

process.source.fileNames = cms.untracked.vstring(['file:/afs/cern.ch/user/b/benitezj/scratch0/Tuples/tmp/patTuple_RelValTTbar.root'])

process.maxEvents = cms.untracked.PSet(input = cms.untracked.int32(-1))
process.out.fileName = cms.untracked.string(os.path.expandvars('file:/afs/cern.ch/user/b/benitezj/scratch0/Tuples/tmp/cmgTuple_RelValTTbar.root'))
process.MessageLogger.cerr.FwkReport.reportEvery = 100


######
process.load('CMGTools.Common.tau_cff')
process.load('CMGTools.Common.runInfoAccounting_cfi')
process.load('CMGTools.Common.cutSummary_cff')


#tauFactory = cms.PSet(
#       inputCollection = cms.InputTag("selectedPatTausAK5"),
#       leptonFactory = process.leptonFactory.clone()
#       )

#process.cmgTau = cms.EDFilter("TauPOProducer",
#                      cfg = tauFactory.clone(),
#                      cuts = cms.PSet(pt_default = cms.string('pt() > 0.0 '),
#                                      )    
#                      
#                      )

process.tauFactory.inputCollection = cms.InputTag("selectedPatTausAK5")
process.cmgTau.cfg = process.tauFactory.clone()

########
process.out.outputCommands = cms.untracked.vstring( 'drop *')
process.out.outputCommands.append( 'keep cmgTaus_cmgTauSel__CMG' )


#########
#output file for histograms etc
#process.TFileService = cms.Service("TFileService",fileName = cms.string("histograms.root"))

######
process.analysisSequence = cms.Sequence(
    process.tauSequence
#    + process.runInfoAccounting #exits when there are no candidates
#    + process.cutSummarySequence
#    + process.cutSummaryTau
)
process.p = cms.Path(process.analysisSequence)
