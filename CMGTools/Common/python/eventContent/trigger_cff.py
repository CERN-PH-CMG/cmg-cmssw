import FWCore.ParameterSet.Config as cms

trigger = cms.untracked.vstring(
    'drop cmgTriggerObjects_*_*_*',
    'keep cmgTriggerObjects_cmgTriggerObjectSel_*_*',
    'keep *_TriggerResults_*_ANA',
    'keep *_TriggerResults_*_PAT',
    )

