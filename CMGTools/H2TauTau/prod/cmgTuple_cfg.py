from PhysicsTools.PatAlgos.patTemplate_cfg import *

runOnMC = True
process.MessageLogger.cerr.FwkReport.reportEvery = 10
process.options   = cms.untracked.PSet( wantSummary = cms.untracked.bool(False))
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )
process.out.fileName   = cms.untracked.string('file:/tmp/cmgTuple.root')
#process.source.fileNames = cms.untracked.vstring(['/store/relval/CMSSW_4_2_3/RelValZTT/GEN-SIM-RECO/START42_V12-v2/0062/4CEA9C47-287B-E011-BAB7-00261894396B.root'])
process.source.fileNames = cms.untracked.vstring(['/store/cmst3/user/benitezj/CMG/DYToTauTau_M-20_TuneZ2_7TeV-pythia6-tauola/Summer11-PU_S3_START42_V11-v2/AODSIM/PrunedAOD/prunedAOD_1.root'])
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
process.GlobalTag.globaltag = cms.string( 'START42_V12::All' )


###########
from CMGTools.H2TauTau.cmgTuple import configureCmgTuple
configureCmgTuple(process,runOnMC)

