from PhysicsTools.PatAlgos.patTemplate_cfg import *
import FWCore.ParameterSet.Config as cms
import os.path

##########
runOnMC = True
process.source.fileNames = cms.untracked.vstring(['file:/afs/cern.ch/user/b/benitezj/scratch0/Tuples/CMGDiTau/DYToTauTau_M_20_TuneZ2_7TeV_pythia6_tauola_Summer11_PU_S3_START42_V11_v2/cmgTuple_1.root'])
process.out.fileName = cms.untracked.string(os.path.expandvars('cmgTuple.root'))
process.TFileService = cms.Service("TFileService",fileName = cms.string("cmgTupleHistograms.root"))
process.maxEvents = cms.untracked.PSet(input = cms.untracked.int32(100))
process.MessageLogger.cerr.FwkReport.reportEvery = 100

#######
from CMGTools.H2TauTau.cmgTupleDiTauClean import configureCmgTupleDiTauClean
configureCmgTupleDiTauClean(process)
