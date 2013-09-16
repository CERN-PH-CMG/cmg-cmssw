import FWCore.ParameterSet.Config as cms

cmgTauEleCorFactory = cms.PSet(
    diObjectCollection  = cms.InputTag('cmgTauEleSel'),
    nSigma              = cms.double(0),
    uncertainty         = cms.double(0.03),
    shift1ProngNoPi0    = cms.double(0.),
    shift1Prong1Pi0     = cms.double(0.015),
    ptDependence1Pi0    = cms.double(0.001),
    # 0.012 according to the TWiki
    shift3Prong         = cms.double(0.012),
    ptDependence3Prong  = cms.double(0.001)
)

cmgTauEleCor = cms.EDFilter(
    "TauEleUpdatePOProducer",
    cfg = cmgTauEleCorFactory.clone(),
    cuts = cms.PSet()
    )
