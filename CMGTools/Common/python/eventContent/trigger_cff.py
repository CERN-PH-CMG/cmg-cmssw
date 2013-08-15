import FWCore.ParameterSet.Config as cms

trigger = cms.untracked.vstring(
    'drop cmgTriggerObjects_*_*_*',
    'keep cmgTriggerObjects_cmgTriggerObjectSel_*_*',
    'keep cmgTriggerObjects_cmgTriggerObjectListSel_*_*',
    'keep cmgTriggerObjects_cmgL1TriggerObjectSel_*_*',
    'keep *_TriggerResults_*_ANA',
    'keep *_TriggerResults_*_PAT',
    'keep *_l1extraParticles_*_*'
    )

