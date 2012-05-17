from CondCore.DBCommon.CondDBSetup_cfi import *

## Preview JEC 2012
def use2012JecPreview(process):
    process.load("CondCore.DBCommon.CondDBCommon_cfi")
    process.jec = cms.ESSource("PoolDBESSource",
                               DBParameters = cms.PSet(messageLevel = cms.untracked.int32(0)
                                                       ),
                               timetype = cms.string('runnumber'),
                               toGet = cms.VPSet(
        cms.PSet(
        record = cms.string('JetCorrectionsRecord'),
        tag    = cms.string('JetCorrectorParametersCollection_Jec12_V7_AK5PF'),
        label  = cms.untracked.string('AK5PF')
        ),
        ),
                               connect = cms.string('sqlite_fip:CMGTools/External/data/Jec12_V7.db')
                               )
    process.es_prefer_jec = cms.ESPrefer('PoolDBESSource','jec')
