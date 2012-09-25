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
    iso = cms.string('{leg}().tauID("byRawIsoMVA") > 0.5'.format(leg=leg))
    if channel == 'tauMu':
        id = cms.PSet(
            decay = cms.string('{leg}().tauID("decayModeFinding")'.format(leg=leg)),
            )
    elif channel == 'tauEle':    
        id = cms.PSet(
            decay = cms.string('{leg}().tauID("decayModeFinding")'.format(leg=leg)),
            eleRejection1 = cms.string('{leg}().tauID("againstElectronMVA") > 0.5'.format(leg=leg)),
            eleRejection2 = cms.string('{leg}().tauID("againstElectronMedium") > 0.5'.format(leg=leg)),
            muRejection2 = cms.string('{leg}().tauID("againstMuonLoose") > 0.5'.format(leg=leg))
            )
    else :
        id = cms.PSet(
            decay = cms.string('{leg}().tauID("decayModeFinding")'.format(leg=leg)),
            )

    tauCuts = cms.PSet(
        kinematics = kinematics.clone(),
        id = id.clone(),
        iso = iso
        )
    
    return tauCuts
