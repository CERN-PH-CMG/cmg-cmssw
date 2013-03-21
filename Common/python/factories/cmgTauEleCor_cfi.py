import FWCore.ParameterSet.Config as cms

cmgTauEleCorFactory = cms.PSet(
    # leg1 and leg2 are dummy collections here
    leg1Collection = cms.InputTag(''),
    leg2Collection = cms.InputTag(''),
    metCollection = cms.InputTag('recoilCorrectedMET'),
    diObjectCollection = cms.InputTag('cmgTauEleSel'),
    nSigma              = cms.double(0),
    uncertainty         = cms.double(0.03),
    shift1ProngNoPi0    = cms.double(0.),
    shift1Prong1Pi0     = cms.double(0.),
    ptDependence1Pi0    = cms.double(0.),
    shift3Prong         = cms.double(0.),
    ptDependence3Prong  = cms.double(0.)
)

cmgTauEleCor = cms.EDFilter(
    "TauEleUpdatePOProducer",
    cfg = cmgTauEleCorFactory.clone(),
    cuts = cms.PSet()
    )
