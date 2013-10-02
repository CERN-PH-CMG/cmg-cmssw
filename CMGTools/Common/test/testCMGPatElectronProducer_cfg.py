from PhysicsTools.PatAlgos.patTemplate_cfg import *
import FWCore.ParameterSet.Config as cms
from CMGTools.Common.Tools.getGlobalTag import getGlobalTag

sep_line = "-" * 50
print
print sep_line
print "CMGPATElectronProducer test"
print sep_line



process.setName_('ANA')

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(2000)
    )

process.maxLuminosityBlocks = cms.untracked.PSet( 
    input = cms.untracked.int32(-1)
    )


from CMGTools.Production.datasetToSource import *
process.source = datasetToSource(
    'cmgtools',
    '/VBF_HToTauTau_M-120_7TeV-powheg-pythia6-tauola/Summer11-PU_S4_START42_V11-v1/AODSIM/V2/PAT_CMG_V2_4_0',
    # '/DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola/Summer11-PU_S4_START42_V11-v1/AODSIM/V2/PAT_CMG_V2_4_0',
    #     '/TauPlusX/Run2011A-May10ReReco-v1/AOD/V2/PAT_CMG_V2_4_0',
    'patTuple_PF2PAT.*root'
    ) 

# reading the first 10 files:
nFiles = 10
print 'WARNING: RESTRICTING INPUT TO THE FIRST', nFiles, 'FILES'
process.source.fileNames = process.source.fileNames[:nFiles] 

print process.source.fileNames

process.out.fileName = cms.untracked.string('patTuple_testCMGPatElectronProducer.root')
process.out.outputCommands = cms.untracked.vstring( 'drop *',
                                                    'keep patElectrons_*_*_*',
                                                    )
process.out.dropMetaData = cms.untracked.string('PRIOR')


process.load('CMGTools.Common.miscProducers.cmgPatElectronProducer_cfi')

process.p = cms.Path(
    process.cmgPatElectronProducer
    ) 


# process.GlobalTag.globaltag = cms.string(getGlobalTag(runOnMC))

process.schedule = cms.Schedule(
    process.p,
    process.outpath
    )

process.MessageLogger.cerr.FwkReport.reportEvery = 100
process.options = cms.untracked.PSet( wantSummary = cms.untracked.bool(True) ) 

