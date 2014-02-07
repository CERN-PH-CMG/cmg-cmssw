import FWCore.ParameterSet.Config as cms

patJetLead = cms.EDProducer(
    "LeadingPatJetSelector",
    inputCollection = cms.InputTag("selectedPatJetsAK5"),
    #copy all entries
    index = cms.int32(2)
    )

