import FWCore.ParameterSet.Config as cms

#TODO implement real electron cuts

def getEleCuts(leg, channel='tauEle', skim=False):

    ptCut = None
    etaCut = None
    if channel == 'tauEle':
        ptCut = 20.
        etaCut = 2.1 # TODO why did we choose that? 
    elif channel == 'muEle':
        ptCut = 20.
        etaCut = 2.3
    else:
        raise ValueError('bad channel specification:'+channel)

    if skim:
        print 'WARNING: skimming not implemented in getEleCuts!'

    eleCuts = cms.PSet(
        kinematics = cms.PSet(
          pt = cms.string('{leg}().pt()>{ptCut}'.format(leg=leg, ptCut=ptCut)),
          eta = cms.string('abs({leg}().eta())<{etaCut}'.format(leg=leg, etaCut=etaCut))
        ),
        iso = cms.PSet(
          #COLIN the iso elest be done on charged particles, not charged hadrons
          relIsoDBeta = cms.string('{leg}().relIso(0.5, 1)<100.0'.format(leg=leg))
        )
    )

    return eleCuts

# eleCuts = getEleCuts('leg2')
