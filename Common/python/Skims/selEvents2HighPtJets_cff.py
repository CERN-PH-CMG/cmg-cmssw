import FWCore.ParameterSet.Config as cms

highPtJets = cms.EDFilter(
    "CandViewSelector",
    src = cms.InputTag("cmgPFBaseJet"),
    cut = cms.string( "pt()>10" )
    )

filterHighPtJets = cms.EDFilter(
    "CandCountFilter",
    src = cms.InputTag("highPtJets"),
    minNumber = cms.uint32(2),
    )
    
selEvents2HighPtJetsSequence = cms.Sequence(
    highPtJets +
    filterHighPtJets
    )

