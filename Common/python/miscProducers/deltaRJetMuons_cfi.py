import FWCore.ParameterSet.Config as cms

deltaRJetMuons = cms.EDProducer(
    "DeltaRVetoProducerPFJet",
    inputCollection = cms.InputTag('cmgPFJetSel'),
    vetoCollection = cms.InputTag('cmgMuonSel'),
    minDeltaR = cms.double(0.3)
)
