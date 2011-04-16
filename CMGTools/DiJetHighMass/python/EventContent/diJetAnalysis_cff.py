import FWCore.ParameterSet.Config as cms

#"""Event contents for Colin's MET studies in di-jet events"""

diJetHighMass = cms.untracked.vstring(
    'drop *',
    'keep  cmgBaseJets_*_*_*',
    'keep  cmgPFJets_*_*_*',
    'keep  cmgBaseJetcmgBaseJetcmgDiObjects_*_*_*',
    'keep  cmgPFJetcmgPFJetcmgDiObjects_*_*_*',
    'keep  cmgBaseMETs_*_*_*',
    'keep  *_TriggerResults_*_*',
    'keep *_hltTriggerSummaryAOD_*_*'
    )
