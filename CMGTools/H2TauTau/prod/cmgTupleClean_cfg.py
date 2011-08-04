from PhysicsTools.PatAlgos.patTemplate_cfg import *
import FWCore.ParameterSet.Config as cms
import os.path

##########
runOnMC = True
process.source.fileNames = cms.untracked.vstring(['/store/cmst3/user/benitezj/CMG/DYToTauTau_M-20_TuneZ2_7TeV-pythia6-tauola/Summer11-PU_S3_START42_V11-v2/AODSIM/V2/PAT_CMG_V2_0_1/tree_CMG_1.root'])
#process.source.fileNames = cms.untracked.vstring(['file:/tmp/tree_CMG.root'])
#process.source.fileNames = cms.untracked.vstring(['file:tree_CMG_2.root'])
process.out.fileName = cms.untracked.string(os.path.expandvars('file:/tmp/tree_CMGClean.root'))
process.TFileService = cms.Service("TFileService",fileName = cms.string("file:/tmp/tree_CMGCleanHistograms.root"))
process.maxEvents = cms.untracked.PSet(input = cms.untracked.int32(-1))
process.MessageLogger.cerr.FwkReport.reportEvery = 10

#######
from CMGTools.H2TauTau.cmgTupleClean import configureCmgTupleClean
configureCmgTupleClean(process, runOnMC)

