import FWCore.ParameterSet.Config as cms

trigger = cms.untracked.vstring(
    'keep cmgTriggerObjects_cmgTriggerObjectSel*_*_*',
    'keep *_TriggerResults_*_ANA',
    'keep *_TriggerResults_*_PAT',
    )

