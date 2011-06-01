import FWCore.ParameterSet.Config as cms

patJetLead = cms.EDProducer(
    "LeadingPatJetSelector",
    inputCollection = cms.InputTag("selectedPatJets"),
    #copy all entries
    index = cms.int32(2)
    )

