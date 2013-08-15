import FWCore.ParameterSet.Config as cms

deltaRJetMuons = cms.EDProducer(
    "DeltaRVetoProducerPFJet",
    inputCollection = cms.InputTag('cmgPFJetSel'),
     MatchingParams = cms.VPSet(
                                cms.PSet(                                     
                                    vetoCollection=cms.InputTag("cmgMuonSel"),
                                    minDeltaR=cms.double(0.3),
                                    removeMatchedObject=cms.bool(True)
                                    )
                                ),
    verbose = cms.untracked.bool(False)
    )
