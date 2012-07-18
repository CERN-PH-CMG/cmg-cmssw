import FWCore.ParameterSet.Config as cms

def getTauCuts( leg, channel='tauMu'):

    ptCut = 18.
    etaCut = 2.3
    muVeto = None
    eVeto = None

    kinematics = cms.PSet(
        pt = cms.string('{leg}().pt()>{ptCut}'.format(leg=leg, ptCut=ptCut)),
        eta = cms.string('abs({leg}().eta())<{etaCut}'.format(leg=leg, etaCut=etaCut))
        )
    id = cms.PSet(
        decay = cms.string('{leg}().tauID("decayModeFinding")'.format(leg=leg)),
        )

    tauCuts = cms.PSet(
        kinematics = kinematics.clone(),
        id = id.clone(),
        # iso = iso
        )
    
    return tauCuts
