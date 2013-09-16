import FWCore.ParameterSet.Config as cms

veryHighPtJets = cms.EDFilter(
    "CandViewSelector",
    src = cms.InputTag("selectedPatJetsAK5"),
    cut = cms.string( "pt()>1000" )
    )

filterVeryHighPtJet = cms.EDFilter(
    "CandCountFilter",
    src = cms.InputTag("veryHighPtJets"),
    minNumber = cms.uint32(1),
    )
    
selEvents1VeryHighPtJetSequence = cms.Sequence(
    veryHighPtJets +
    filterVeryHighPtJet
    )

