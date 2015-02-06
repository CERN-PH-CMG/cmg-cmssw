import FWCore.ParameterSet.Config as cms


HBHENoiseFilter2011NonIsoRecommended = cms.EDFilter(
    'HBHENoiseFilter',
    noiselabel = cms.InputTag('hcalnoise'),
    minRatio = cms.double(-999.0),
    maxRatio = cms.double(999.0),
    useTS4TS5 = cms.bool(True), #use R45
    minHPDHits = cms.int32(17),
    minRBXHits = cms.int32(999),
    minHPDNoOtherHits = cms.int32(10),
    minZeros = cms.int32(10),
    minHighEHitTime = cms.double(-9999.0),
    maxHighEHitTime = cms.double(9999.0),
    maxRBXEMF = cms.double(-999.0),
    minNumIsolatedNoiseChannels = cms.int32(999999), #do not use isolation
    minIsolatedNoiseSumE = cms.double(999999.),
    minIsolatedNoiseSumEt = cms.double(999999.)
    )
