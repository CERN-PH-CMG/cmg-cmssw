import FWCore.ParameterSet.Config as cms

# conversions (might use it later)
patConversions = cms.EDProducer(
    "PATConversionProducer",
    # input collection
    electronSource = cms.InputTag("patElectronsWithTrigger")
    # this should be your last selected electron collection name since currently index is used to match with electron later. We can fix this using reference pointer. ,
    )
