import FWCore.ParameterSet.Config as cms

highMET = cms.EDFilter(
    "CandViewSelector",
    src = cms.InputTag("cmgMETPFCandidates"),
    cut = cms.string("pt > 100")
    )

filterHighMET = cms.EDFilter(
    "CandCountFilter",
    src = cms.InputTag("highMET"),
    minNumber = cms.uint32(1),
    )

selEventsHighMETSequence = cms.Sequence(
    highMET +
    filterHighMET    
    )

