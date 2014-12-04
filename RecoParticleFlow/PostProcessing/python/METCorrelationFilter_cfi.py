import FWCore.ParameterSet.Config as cms

METCorrelationFilter = cms.EDFilter(
    "METCorrelationFilter",
    MET1 = cms.InputTag("pfMet"),
    MET2 = cms.InputTag("met"),
    aPar = cms.double(1.),
    bPar = cms.double(200.),
    verbose = cms.untracked.bool( True )
    )
