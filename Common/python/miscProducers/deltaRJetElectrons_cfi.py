import FWCore.ParameterSet.Config as cms

deltaRJetElectrons = cms.EDProducer(
    "DeltaRVetoProducerPFJet",
    inputCollection = cms.InputTag('cmgPFJetSel'),
    vetoCollection = cms.InputTag('cmgElectronSel'),
    minDeltaR = cms.double(0.3)
)
