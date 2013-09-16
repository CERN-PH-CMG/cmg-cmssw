import FWCore.ParameterSet.Config as cms

runInfoAccounting = cms.untracked.vstring(
#     'keep *_MEtoEDMConverter_*_*',
    'keep GenRunInfoProduct_*_*_*',
    'keep GenFilterInfo_*_*_*'
    )
