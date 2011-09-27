import FWCore.ParameterSet.Config as cms

metSignificance = cms.EDProducer(
    "METSignificanceProducer",
    pfJets = cms.InputTag( 'pfJetsAK5' )
)
