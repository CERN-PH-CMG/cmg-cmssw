import FWCore.ParameterSet.Config as cms

patJetLead = cms.EDProducer(
    "LeadingPatJetSelector",
    inputCollection = cms.untracked.InputTag("selectedPatJetsPFlow"),
    #copy all entries
    index = cms.untracked.int32(2)
    )

