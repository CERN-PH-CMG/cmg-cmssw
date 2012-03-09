import FWCore.ParameterSet.Config as cms

def getTauCuts( leg, channel='tauMu', skim=False):

    ptCut = 20.
    # ptCut = 15.
    etaCut = 2.3
    muVeto = None
    eVeto = None
    if channel == 'tauMu':
        muVeto = cms.string('{leg}().tauID("againstMuonTight")'.format(leg=leg))
        eVeto = cms.string('{leg}().tauID("againstElectronLoose")'.format(leg=leg))
    elif channel == 'tauEle':
        muVeto = cms.string('{leg}().tauID("againstMuonLoose")'.format(leg=leg))
        eVeto = cms.string('{leg}().tauID("againstElectronMVA")'.format(leg=leg))
    elif channel == 'diTau':
        muVeto = cms.string('{leg}().tauID("againstMuonLoose")'.format(leg=leg))
        eVeto = cms.string('{leg}().tauID("againstElectronLoose")'.format(leg=leg))
    else:
        raise ValueError('bad channel specification:'+channel)

    kinematics = cms.PSet(
        pt = cms.string('{leg}().pt()>{ptCut}'.format(leg=leg, ptCut=ptCut)),
        eta = cms.string('abs({leg}().eta())<{etaCut}'.format(leg=leg, etaCut=etaCut))
        )
    id = cms.PSet(
        decay = cms.string('{leg}().tauID("decayModeFinding")'.format(leg=leg)),
        muVeto = muVeto,
        eVeto = eVeto
        )
    iso = cms.string('{leg}().tauID("byLooseCombinedIsolationDeltaBetaCorr")'.format(leg=leg) )
    # iso = cms.string('{leg}().tauID("decayModeFinding")'.format(leg=leg) )


    if not skim:
        tauCuts = cms.PSet(
            kinematics = kinematics.clone(),
            id = id.clone(),
            iso = iso
            )
    else:
        tauCuts = cms.PSet(
            kinematics = kinematics.clone(),
            id = id.clone()
            )
    
    return tauCuts
