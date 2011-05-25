import FWCore.ParameterSet.Config as cms

patJetLead = cms.EDProducer(
    "LeadingPatJetSelector",
    inputCollection = cms.untracked.InputTag("selectedPatJetsAK5"),
    #copy all entries
    index = cms.untracked.int32(2)
    )

