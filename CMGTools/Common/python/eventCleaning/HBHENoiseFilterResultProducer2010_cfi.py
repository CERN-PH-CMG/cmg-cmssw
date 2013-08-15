import FWCore.ParameterSet.Config as cms


HBHENoiseFilterResultProducer2010 = cms.EDProducer(
    'HBHENoiseFilterResultProducer',
    noiselabel = cms.InputTag('hcalnoise'),
    minRatio = cms.double(0.70),#use old E2/E10 cut
    maxRatio = cms.double(0.96),#use old E2/E10 cut
    useTS4TS5 = cms.bool(False),#do not use R45 cut 
    minHPDHits = cms.int32(17),
    minRBXHits = cms.int32(999),
    minHPDNoOtherHits = cms.int32(10),
    minZeros = cms.int32(10),
    minHighEHitTime = cms.double(-9999.0),
    maxHighEHitTime = cms.double(9999.0),
    maxRBXEMF = cms.double(-9999.0),
    minNumIsolatedNoiseChannels = cms.int32(999999), #do not use isolation
    minIsolatedNoiseSumE = cms.double(999999.),
    minIsolatedNoiseSumEt = cms.double(999999.)
    )


