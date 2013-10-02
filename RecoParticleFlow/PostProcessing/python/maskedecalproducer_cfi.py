import FWCore.ParameterSet.Config as cms

maskedECALProducer = cms.EDProducer('MaskedECALProducer'
  , minExcludeStatus      = cms.int32(1)
  , ecalTriggerPrimitives = cms.InputTag("ecalTPSkim")
)

