import FWCore.ParameterSet.Config as cms

#see CMS-AN-2010-264 pp 4
vbtfmuon = cms.PSet(
    isGlobal = cms.string('isGlobal()'),
    isTracker = cms.string('isTracker()'),
    numberOfValidTrackerHits = cms.string('numberOfValidTrackerHits() > 10'),
    numberOfValidPixelHits = cms.string('numberOfValidPixelHits() > 0'),
    numberOfValidMuonHits = cms.string('numberOfValidMuonHits() > 0'),
    numberOfMatches = cms.string('numberOfMatches() > 1'),
    normalizedChi2 = cms.string('normalizedChi2() < 10'),
    dxy = cms.string('abs(dxy()) < 0.2')
    )
