from PhysicsTools.PatAlgos.patTemplate_cfg import *
import FWCore.ParameterSet.Config as cms
import os.path

##########
runOnMC = True
process.maxEvents = cms.untracked.PSet(input = cms.untracked.int32(100))
process.MessageLogger.cerr.FwkReport.reportEvery = 10
process.source.fileNames = cms.untracked.vstring(os.path.expandvars('/store/cmst3/user/benitezj/CMG/DYToTauTau_M-20_TuneZ2_7TeV-pythia6-tauola/Summer11-PU_S3_START42_V11-v2/AODSIM/PrunedAOD/PAT/CMG/CMGClean/cmgTuple_1.root'))
process.out.fileName = cms.untracked.string(os.path.expandvars('cmgTupleDiTau.root'))


#######
from CMGTools.H2TauTau.cmgTupleDiTau import configureCmgTupleDiTau
configureCmgTupleDiTau(process)



