import FWCore.ParameterSet.Config as cms

cmgPatMuonProducer = cms.EDProducer(
    "CMGPATMuonProducer",
    src = cms.InputTag( 'selectedPatMuonsAK5' )
)
