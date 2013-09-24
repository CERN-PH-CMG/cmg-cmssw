import FWCore.ParameterSet.Config as cms

generator = cms.EDProducer("FlatRandomPtGunProducer",
    PGunParameters = cms.PSet(
        MaxPt = cms.double(50.),
        MinPt = cms.double(0.),
        PartID = cms.vint32(22),
        MaxEta = cms.double(1.),
        MaxPhi = cms.double(3.14159265359),
        MinEta = cms.double(-1.),
        MinPhi = cms.double(-3.14159265359) ## in radians

    ),
    Verbosity = cms.untracked.int32(0), ## set to 1 (or greater)  for printouts
    psethack = cms.string('Blah'),
    AddAntiParticle = cms.bool(False),
    firstRun = cms.untracked.uint32(1)
)
