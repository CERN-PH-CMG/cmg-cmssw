import FWCore.ParameterSet.Config as cms

#merge different collections
cmgCandMerge = cms.EDProducer(
    "CandViewMerger",
    src = cms.VInputTag(
                    "cmgPFJet",
                    "cmgMuons"
                    ),
    )


