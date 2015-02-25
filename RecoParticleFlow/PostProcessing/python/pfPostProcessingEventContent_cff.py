import FWCore.ParameterSet.Config as cms

pfPostProcessingEventContent = cms.untracked.vstring(
    'keep *_inconsistentMuons*_*_*',
    'keep *_greedyMuons*_*_*',
    'keep *_pfEventFilter*_*_*',
    'drop recoPFCandidates_pfEventFilter*_AfterMuonCleaning_*'
    )
