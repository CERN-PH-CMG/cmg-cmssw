from PhysicsTools.PatAlgos.patTemplate_cfg import *
import os.path

##########
runOnMC = True
#process.source.fileNames = cms.untracked.vstring(os.path.expandvars('/store/cmst3/user/benitezj/CMG/DYToTauTau_M-20_TuneZ2_7TeV-pythia6-tauola/Summer11-PU_S3_START42_V11-v2/AODSIM/PrunedAOD/PAT/CMG/CMGClean/cmgTuple_1.root'))
process.load("CMGTools.Common.sources.DYToTauTau_M_20_TuneZ2_7TeV_pythia6_tauola.Summer11_PU_S3_START42_V11_v2.AODSIM.V2.PAT_CMG_V2_0_1.source_cff")
process.maxEvents = cms.untracked.PSet(input = cms.untracked.int32(100))
process.MessageLogger.cerr.FwkReport.reportEvery = 10
process.out.fileName = cms.untracked.string(os.path.expandvars('cmgTupleDiTau.root'))


#######
from CMGTools.H2TauTau.cmgTupleDiTau_cfi import configureCmgTupleDiTau
configureCmgTupleDiTau(process)



