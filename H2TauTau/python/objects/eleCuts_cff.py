import FWCore.ParameterSet.Config as cms

#TODO implement real electron cuts

def getEleCuts(leg, channel='tauEle', skim=False):

    ptCut = None
    etaCut = None
#    lmvaID = -99999
    lmvaID1 = -99999
    lmvaID2 = -99999
    lmvaID3 = -99999
#    isoCut = 100
    if channel == 'tauEle':
        ptCut = 20.
        etaCut = 2.1
#        lmvaID = 0.9
        lmvaID1 = 0.925
        lmvaID2 = 0.975
        lmvaID3 = 0.985
#        isoCut = 0.3
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
        ID = cms.PSet(
            hitsnum = cms.string('{leg}().numberOfHits==0'.format(leg=leg)),
            convVeto = cms.string('{leg}().passConversionVeto()!=0'.format(leg=leg)),
            mvaID = cms.string('(abs({leg}().sourcePtr().superCluster().eta())<0.8 && {leg}().mvaNonTrigV0() > {lmvaID1}) || (abs({leg}().sourcePtr().superCluster().eta())>0.8 && abs({leg}().sourcePtr().superCluster().eta())<1.479 && {leg}().mvaNonTrigV0() > {lmvaID2}) || (abs({leg}().sourcePtr().superCluster().eta())>1.479 && {leg}().mvaNonTrigV0() > {lmvaID3})'.format(leg=leg, lmvaID1=lmvaID1, lmvaID2=lmvaID2, lmvaID3=lmvaID3))
#            mvaID = cms.string('{leg}().mvaNonTrigV0()  > {lmvaID}'.format(leg=leg, lmvaID=lmvaID))
        ),
#         iso = cms.PSet(
#           #COLIN the iso elest be done on charged particles, not charged hadrons
#           relIsoDBeta = cms.string('{leg}().relIso(0.5, 1)<{isoCut}'.format(leg=leg,isoCut=isoCut))
#         )
    )

    return eleCuts

