from PhysicsTools.PatAlgos.patTemplate_cfg import *
import FWCore.ParameterSet.Config as cms
import os.path

##########
runOnMC = True
process.setName_('CMGClean')

#from CMGTools.TauTest.sources.CMG.RelValZTT.source_cff import *
#process.load('CMGTools.TauTest.sources.CMG.RelValZTT.source_cff')
process.source.fileNames = cms.untracked.vstring(['file:/afs/cern.ch/user/b/benitezj/scratch0/Tuples/tmp/cmgTuple_RelValTTbar.root'])
process.out.fileName = cms.untracked.string(os.path.expandvars('file:/afs/cern.ch/user/b/benitezj/scratch0/Tuples/tmp/cmgTupleClean_RelValTTbar.root'))
process.TFileService = cms.Service("TFileService",fileName = cms.string("file:/afs/cern.ch/user/b/benitezj/scratch0/Tuples/tmp/cmgTupleClean_RelValTTbar_histograms.root"))
process.maxEvents = cms.untracked.PSet(input = cms.untracked.int32(1000))
process.MessageLogger.cerr.FwkReport.reportEvery = 100


######
process.load('CMGTools.TauTest.skims.tauSelClean_cff')


######
process.analysisSequence = cms.Sequence(
    process.tauSelCleanSequence
    + process.tauSelCleanHistogramsSequence
)


process.p = cms.Path(process.analysisSequence)

########
process.out.outputCommands = cms.untracked.vstring( 'drop *')
process.out.outputCommands.append( 'keep cmgTaus_cmgTauSelClean__CMGClean' )


