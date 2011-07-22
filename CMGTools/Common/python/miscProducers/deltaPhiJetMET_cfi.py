import FWCore.ParameterSet.Config as cms

deltaPhiJetMET = cms.EDProducer(
    "DeltaPhiProducer",
    met = cms.InputTag( 'cmgPFMET' ),
    objects = cms.InputTag( 'cmgPFJetSel' )
)
