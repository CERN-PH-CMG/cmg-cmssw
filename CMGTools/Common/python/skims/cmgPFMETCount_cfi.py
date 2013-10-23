import FWCore.ParameterSet.Config as cms

cmgPFMETCount = cms.EDFilter(
    "CandCountFilter",
    src = cms.InputTag("cmgPFMETSel"),
    minNumber = cms.uint32(1),
    )



