import FWCore.ParameterSet.Config as cms

source = cms.Source(
        "PoolSource",

        noEventSort = cms.untracked.bool(True),
        duplicateCheckMode = cms.untracked.string("noDuplicateCheck"),
        fileNames = cms.untracked.vstring()
)
source.fileNames.extend([
    '/store/cmst3/user/cmgtools/Tutorial/CMSSW_4_2_5/SingleMu/Run2011A_May10ReReco_v1/AOD/PAT_CMG/patTuple_PF2PAT_0.root',
])
