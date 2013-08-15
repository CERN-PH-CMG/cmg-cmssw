import FWCore.ParameterSet.Config as cms

selectedZCand = cms.EDFilter(
    "DiMuonSelector",
    src = cms.InputTag("cmgDiMuon"),
    cut = cms.string( "getSelection(\"cuts_zmumu\")" )
    )

selectedZCandFilter = cms.EDFilter("CandViewCountFilter",
  src = cms.InputTag('selectedZCand'),
  minNumber = cms.uint32(1)
)

selectedZSequence = cms.Sequence(selectedZCand+selectedZCandFilter)


