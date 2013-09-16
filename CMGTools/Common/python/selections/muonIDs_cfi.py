import FWCore.ParameterSet.Config as cms

# 2011 Muon POG ID

vbtfmuon = cms.PSet(
    isGlobal = cms.string('isGlobal()'),
    isTracker = cms.string('isTracker()'),
    #Updated for 2012 with new cut from the Muon POG
    #numberOfValidTrackerHits = cms.string('numberOfValidTrackerHits() > 10'),
    trackerLayersWithMeasurement = cms.string('trackerLayersWithMeasurement() > 8'),
    numberOfValidPixelHits = cms.string('numberOfValidPixelHits() > 0'),
    numberOfValidMuonHits = cms.string('numberOfValidMuonHits() > 0'),
    numberOfMatches = cms.string('numberOfMatches() > 1'),
    normalizedChi2 = cms.string('normalizedChi2() < 10'),
    dxy = cms.string('abs(dxy()) < 0.2')
    )

# 2012 Muon POG IDs
#//see https://twiki.cern.ch/twiki/bin/view/CMSPublic/SWGuideMuonId
# the vertexing requirement are not included in the precomputed cut variable
# so that they can be applied with/without the good vertex selection 


loosemuon = cms.PSet(
    isPF = cms.string('isPF()'),
    isNotSTA = cms.string('isGlobal()||isTracker()')
    )

softmuonNoVtx = cms.PSet(
    isTMOST = cms.string('isTMOST()'),
    trackerLayersWithMeasurement = cms.string('trackerLayersWithMeasurement() > 5'),
    pixelLayersWithMeasurement = cms.string('pixelLayersWithMeasurement() > 1'),
    tkNormalizedChi2 = cms.string('tkNormalizedChi2 < 1.8'),
    #dxy = cms.string('abs(dxy()) < 3')
    #dz = cms.string('abs(dz()) < 30')
    )


tightmuonNoVtx = cms.PSet(
    isGlobal = cms.string('isGlobal()'),
    isPF = cms.string('isPF()'),
    trackerLayersWithMeasurement = cms.string('trackerLayersWithMeasurement() > 5'),
    numberOfValidPixelHits = cms.string('numberOfValidPixelHits() > 0'),
    numberOfValidMuonHits = cms.string('numberOfValidMuonHits() > 0'),
    numberOfMatchedStations = cms.string('numberOfMatchedStations() > 1'),
    normalizedChi2 = cms.string('normalizedChi2() < 10'),
    #dxy = cms.string('abs(dxy()) < 0.2')
    #dz = cms.string('abs(dz()) < 0.5')
    )


highptmuonNoVtx = cms.PSet(
    isGlobal = cms.string('isGlobal()'),
    trackerLayersWithMeasurement = cms.string('trackerLayersWithMeasurement() > 8'),
    numberOfValidPixelHits = cms.string('numberOfValidPixelHits() > 0'),
    numberOfValidMuonHits = cms.string('numberOfValidMuonHits() > 0'),
    numberOfMatchedStations = cms.string('numberOfMatchedStations() > 1'),
    #dxy = cms.string('abs(dxy()) < 0.2')
    #dz = cms.string('abs(dz()) < 0.5')
    )
