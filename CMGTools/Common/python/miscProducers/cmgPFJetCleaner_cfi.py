import FWCore.ParameterSet.Config as cms

cmgCleanedJetsEle = cms.EDProducer("DeltaRVetoProducerPFJet",
                                   inputCollection=cms.InputTag("cmgPFJetSel"),
                                   MatchingParams = cms.VPSet(
                                       cms.PSet(                                     
                                             vetoCollection=cms.InputTag("cmgElectronSel"),
                                             minDeltaR=cms.double(0.8),
                                             removeMatchedObject=cms.bool(False)
                                             ),
                                       cms.PSet(
                                             vetoCollection=cms.InputTag("cmgMuonSel"),
                                             minDeltaR=cms.double(0.8),
                                             removeMatchedObject=cms.bool(False)
                                             )
                                       ),
                                   verbose=cms.untracked.bool(True)
                                   )

