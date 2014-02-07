import FWCore.ParameterSet.Config as cms

#electron identification
vetoNoVtx = cms.PSet(
  deltaEtaSuperClusterTrackAtVtx = cms.string('(sourcePtr().isEB() && sourcePtr().deltaEtaSuperClusterTrackAtVtx() < 0.007 ) || ( sourcePtr().isEE() && sourcePtr().deltaEtaSuperClusterTrackAtVtx() < 0.010 )'),
  deltaPhiSuperClusterTrackAtVtx = cms.string('(sourcePtr().isEB() && sourcePtr().deltaPhiSuperClusterTrackAtVtx() < 0.800 ) || ( sourcePtr().isEE() && sourcePtr().deltaPhiSuperClusterTrackAtVtx() < 0.700 )'),
  sigmaIetaIeta = cms.string('(sourcePtr().isEB() && sourcePtr().sigmaIetaIeta() < 0.010 ) || ( sourcePtr().isEE() && sourcePtr().sigmaIetaIeta() < 0.030 )'),
  hadronicOverEm = cms.string('(sourcePtr().isEB() && sourcePtr().hadronicOverEm() < 0.150 ) || ( sourcePtr().isEE() && sourcePtr().hadronicOverEm() < 999.9)'),
  ooemoop = cms.string('( 1.0/sourcePtr().ecalEnergy() - sourcePtr().eSuperClusterOverP()/sourcePtr().ecalEnergy() ) < 999.9'), 
  passConversionVeto = cms.string('sourcePtr().passConversionVeto() == 1 || sourcePtr().passConversionVeto() != 1'), # not applied
  mHits = cms.string('sourcePtr().gsfTrack().get().trackerExpectedHitsInner().numberOfHits() <= 999'), # not applied
)

looseNoVtx = cms.PSet(
  deltaEtaSuperClusterTrackAtVtx = cms.string('(sourcePtr().isEB() && sourcePtr().deltaEtaSuperClusterTrackAtVtx() < 0.007 ) || ( sourcePtr().isEE() && sourcePtr().deltaEtaSuperClusterTrackAtVtx() < 0.009 )'),
  deltaPhiSuperClusterTrackAtVtx = cms.string('(sourcePtr().isEB() && sourcePtr().deltaPhiSuperClusterTrackAtVtx() < 0.150 ) || ( sourcePtr().isEE() && sourcePtr().deltaPhiSuperClusterTrackAtVtx() < 0.100 )'),
  sigmaIetaIeta = cms.string('(sourcePtr().isEB() && sourcePtr().sigmaIetaIeta() < 0.010 ) || ( sourcePtr().isEE() && sourcePtr().sigmaIetaIeta() < 0.030 )'),
  hadronicOverEm = cms.string('(sourcePtr().isEB() && sourcePtr().hadronicOverEm() < 0.120 ) || ( sourcePtr().isEE() && sourcePtr().hadronicOverEm() < 0.100)'),
  ooemoop = cms.string('( 1.0/sourcePtr().ecalEnergy() - sourcePtr().eSuperClusterOverP()/sourcePtr().ecalEnergy() ) < 0.050'),                                                                       
  passConversionVeto = cms.string('sourcePtr().passConversionVeto() == 1'),
  mHits = cms.string('sourcePtr().gsfTrack().get().trackerExpectedHitsInner().numberOfHits() <= 1'),
)

mediumNoVtx = cms.PSet(
  deltaEtaSuperClusterTrackAtVtx = cms.string('(sourcePtr().isEB() && sourcePtr().deltaEtaSuperClusterTrackAtVtx() < 0.004 ) || ( sourcePtr().isEE() && sourcePtr().deltaEtaSuperClusterTrackAtVtx() < 0.007 )'),
  deltaPhiSuperClusterTrackAtVtx = cms.string('(sourcePtr().isEB() && sourcePtr().deltaPhiSuperClusterTrackAtVtx() < 0.060 ) || ( sourcePtr().isEE() && sourcePtr().deltaPhiSuperClusterTrackAtVtx() < 0.030 )'),
  sigmaIetaIeta = cms.string('(sourcePtr().isEB() && sourcePtr().sigmaIetaIeta() < 0.010 ) || ( sourcePtr().isEE() && sourcePtr().sigmaIetaIeta() < 0.030 )'),
  hadronicOverEm = cms.string('(sourcePtr().isEB() && sourcePtr().hadronicOverEm() < 0.120 ) || ( sourcePtr().isEE() && sourcePtr().hadronicOverEm() < 0.100)'),
  ooemoop = cms.string('( 1.0/sourcePtr().ecalEnergy() - sourcePtr().eSuperClusterOverP()/sourcePtr().ecalEnergy() ) < 0.050'),                                                                       
  passConversionVeto = cms.string('sourcePtr().passConversionVeto() == 1'),
  mHits = cms.string('sourcePtr().gsfTrack().get().trackerExpectedHitsInner().numberOfHits() <= 1'),
)

tightNoVtx = cms.PSet(
  deltaEtaSuperClusterTrackAtVtx = cms.string('(sourcePtr().isEB() && sourcePtr().deltaEtaSuperClusterTrackAtVtx() < 0.004 ) || ( sourcePtr().isEE() && sourcePtr().deltaEtaSuperClusterTrackAtVtx() < 0.005 )'),
  deltaPhiSuperClusterTrackAtVtx = cms.string('(sourcePtr().isEB() && sourcePtr().deltaPhiSuperClusterTrackAtVtx() < 0.030 ) || ( sourcePtr().isEE() && sourcePtr().deltaPhiSuperClusterTrackAtVtx() < 0.020 )'),
  sigmaIetaIeta = cms.string('(sourcePtr().isEB() && sourcePtr().sigmaIetaIeta() < 0.010 ) || ( sourcePtr().isEE() && sourcePtr().sigmaIetaIeta() < 0.030 )'),
  hadronicOverEm = cms.string('(sourcePtr().isEB() && sourcePtr().hadronicOverEm() < 0.120 ) || ( sourcePtr().isEE() && sourcePtr().hadronicOverEm() < 0.100)'),
  ooemoop = cms.string('( 1.0/sourcePtr().ecalEnergy() - sourcePtr().eSuperClusterOverP()/sourcePtr().ecalEnergy() ) < 0.050'),                                                                      
  passConversionVeto = cms.string('sourcePtr().passConversionVeto() == 1'),
  mHits = cms.string('sourcePtr().gsfTrack().get().trackerExpectedHitsInner().numberOfHits() <= 0'),
)

premvaTrig = cms.PSet(
  sigmaIetaIeta = cms.string('( abs(sourcePtr().superCluster().get().eta()) < 1.479 && sourcePtr().sigmaIetaIeta() < 0.014 ) || ( abs(sourcePtr().superCluster().get().eta()) >= 1.479 && sourcePtr().sigmaIetaIeta() < 0.035 )'),
  hadronicOverEm = cms.string('( abs(sourcePtr().superCluster().get().eta()) < 1.479 && sourcePtr().hadronicOverEm() < 0.15 ) || ( abs(sourcePtr().superCluster().get().eta()) >= 1.479 && sourcePtr().hadronicOverEm() < 0.10 )'),
  dr03Trk = cms.string('sourcePtr().dr03TkSumPt()/pt() < 0.2'), 
  dr03Ecal = cms.string('sourcePtr().dr03EcalRecHitSumEt()/pt() < 0.2'),
  dr03Hcal = cms.string('sourcePtr().dr03HcalTowerSumEt()/pt() < 0.2'),
  mHits = cms.string('sourcePtr().gsfTrack().get().trackerExpectedHitsInner().numberOfHits() == 0'),
)


