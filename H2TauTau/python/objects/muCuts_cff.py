import FWCore.ParameterSet.Config as cms

# from CMGTools.Common.selections.vbtfmuon_cfi import *

# muCuts = vbtfmuon.clone()
# muCuts.dxy = cms.string('abs(dxy)<0.045')
# muCuts.dz = cms.string('abs(dz)<0.2')

def getMuCuts( leg, channel='tauMu'):
    
    ptCut = None
    etaCut = None
    if channel == 'tauMu':
        ptCut = 17.
        etaCut = 2.1
    elif channel == 'muEle':
        ptCut = 10.
        etaCut = 2.1
    else:
        raise ValueError('bad channel specification:'+channel)

    kinematics = cms.PSet(
        pt = cms.string('{leg}().pt()>{ptCut}'.format(leg=leg, ptCut=ptCut) ),
        eta = cms.string('abs({leg}().eta())<{etaCut}'.format(leg=leg, etaCut=etaCut))
        )

##     id = cms.PSet(
##         isGlobal = cms.string('{leg}().isGlobal()'.format(leg=leg)),
##         isTracker = cms.string('{leg}().isTracker()'.format(leg=leg)),
##         numberOfValidTrackerHits = cms.string('{leg}().numberOfValidTrackerHits() > 10'.format(leg=leg)),
##         numberOfValidPixelHits = cms.string('{leg}().numberOfValidPixelHits() > 0'.format(leg=leg)),
##         numberOfValidMuonHits = cms.string('{leg}().numberOfValidMuonHits() > 0'.format(leg=leg)),
##         numberOfMatches = cms.string('{leg}().numberOfMatches() > 1'.format(leg=leg)),
##         normalizedChi2 = cms.string('{leg}().normalizedChi2() < 10'.format(leg=leg)),
##         dxy = cms.string('abs({leg}().dxy()) < 0.045'.format(leg=leg)),
##         dz = cms.string('abs({leg}().dz()) < 0.2'.format(leg=leg))
##         )

##     iso = cms.PSet(
##         relIsoDBeta = cms.string('{leg}().relIso(0.5, 1)<100.0'.format(leg=leg))
##         )
    
    muCuts = cms.PSet(
        kinematics = kinematics.clone(),
        )

    return muCuts


