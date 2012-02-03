import FWCore.ParameterSet.Config as cms

# from CMGTools.Common.selections.vbtfmuon_cfi import *

# muCuts = vbtfmuon.clone()
# muCuts.dxy = cms.string('abs(dxy)<0.045')
# muCuts.dz = cms.string('abs(dz)<0.2')

def getMuCuts( leg ):
    muCuts = cms.PSet(
        kinematics = cms.PSet(
          pt = cms.string('{leg}().pt()>17'.format(leg=leg) ),
          eta = cms.string('abs({leg}().eta())<2.1'.format(leg=leg))
        ),
        id = cms.PSet(
          isGlobal = cms.string('{leg}().isGlobal()'.format(leg=leg)),
          isTracker = cms.string('{leg}().isTracker()'.format(leg=leg)),
          numberOfValidTrackerHits = cms.string('{leg}().numberOfValidTrackerHits() > 10'.format(leg=leg)),
          numberOfValidPixelHits = cms.string('{leg}().numberOfValidPixelHits() > 0'.format(leg=leg)),
          numberOfValidMuonHits = cms.string('{leg}().numberOfValidMuonHits() > 0'.format(leg=leg)),
          numberOfMatches = cms.string('{leg}().numberOfMatches() > 1'.format(leg=leg)),
          normalizedChi2 = cms.string('{leg}().normalizedChi2() < 10'.format(leg=leg)),
          dxy = cms.string('abs({leg}().dxy()) < 0.045'.format(leg=leg)),
          dz = cms.string('abs({leg}().dz()) < 0.2'.format(leg=leg))
          ),
        iso = cms.PSet(
          #COLIN the iso must be done on charged particles, not charged hadrons
          relIsoDBeta = cms.string('{leg}().relIso(0.5, 1)<100.0'.format(leg=leg))
        )
        )
    return muCuts

muCuts = getMuCuts('leg2')
