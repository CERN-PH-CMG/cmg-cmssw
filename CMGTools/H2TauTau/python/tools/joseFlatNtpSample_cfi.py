import FWCore.ParameterSet.Config as cms

tauMuFlatNtp = cms.EDAnalyzer(
    "TauMuFlatNtp",
    path = cms.untracked.string(""),
    firstRun =  cms.int32( 0 ),
    lastRun =  cms.int32( 0 ),
    dataType = cms.string(""),
    pupWeightName = cms.InputTag(""),
    trigPath1 = cms.InputTag(""),
    trigPath2 = cms.InputTag(""),
    trigPath3 = cms.InputTag(""),
    trigPath4 = cms.InputTag(""),
    trigPath5 = cms.InputTag(""),
    trigPath6 = cms.InputTag(""),
    trigPath7 = cms.InputTag(""),
    trigPath8 = cms.InputTag(""),
    trigPath9 = cms.InputTag(""),
    trigPath10 = cms.InputTag(""),
    diTauTag =  cms.InputTag(""),
    sampleGenEventType =   cms.int32( 0 ),
    sampleTruthEventType =  cms.int32( 0 ),
    )
