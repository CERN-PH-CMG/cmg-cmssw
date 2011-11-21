import FWCore.ParameterSet.Config as cms

# from CMGTools.Common.selections.vbtfmuon_cfi import *

# muCuts = vbtfmuon.clone()
# muCuts.dxy = cms.string('abs(dxy)<0.045')
# muCuts.dz = cms.string('abs(dz)<0.2')

muCuts = cms.PSet(
    kinematics = cms.PSet(
      pt = cms.string('leg2().pt()>15'),
      eta = cms.string('abs(leg2().eta())<2.1')
    ),
    id = cms.PSet(
      isGlobal = cms.string('leg2().isGlobal()'),
      isTracker = cms.string('leg2().isTracker()'),
      numberOfValidTrackerHits = cms.string('leg2().numberOfValidTrackerHits() > 10'),
      numberOfValidPixelHits = cms.string('leg2().numberOfValidPixelHits() > 0'),
      numberOfValidMuonHits = cms.string('leg2().numberOfValidMuonHits() > 0'),
      numberOfMatches = cms.string('leg2().numberOfMatches() > 1'),
      normalizedChi2 = cms.string('leg2().normalizedChi2() < 10'),
      dxy = cms.string('abs(leg2().dxy()) < 0.045'),
      dz = cms.string('abs(leg2().dz()) < 0.2')
      ),
    iso = cms.PSet(
      relIsoDBeta = cms.string('leg2().relIso(0.5)<0.1')
    )
    )
