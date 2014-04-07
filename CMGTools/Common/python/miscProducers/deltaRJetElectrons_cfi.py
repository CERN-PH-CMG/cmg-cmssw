import FWCore.ParameterSet.Config as cms

deltaRJetElectrons = cms.EDProducer(
    "DeltaRVetoProducerPFJet",
     inputCollection = cms.InputTag('cmgPFJetSel'),
     MatchingParams = cms.VPSet(
                                cms.PSet(                                     
                                    vetoCollection=cms.InputTag("cmgElectronSel"),
                                    minDeltaR=cms.double(0.3),
                                    removeMatchedObject=cms.bool(True)
                                    )
                                ),
    verbose = cms.untracked.bool(False)
)
