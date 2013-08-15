import FWCore.ParameterSet.Config as cms

cmgDiTauCorFactory = cms.PSet(
    # leg1 and leg2 are dummy collections here
    leg1Collection      = cms.InputTag(''),
    leg2Collection      = cms.InputTag(''),
    metCollection       = cms.InputTag('recoilCorrectedMET'),
    diObjectCollection  = cms.InputTag('cmgDiTauSel'),
    nSigma              = cms.double(0),
    uncertainty         = cms.double(0.03),
    shift1ProngNoPi0    = cms.double(0.),
    shift1Prong1Pi0     = cms.double(0.027), ## 1.5% +1.2% by Phil for summer 13 https://indico.cern.ch/getFile.py/access?contribId=24&sessionId=4&resId=0&materialId=slides&confId=252865
    ptDependence1Pi0    = cms.double(0.001),
    ############ for the embedded
    shift3Prong         = cms.double(0.012),
    ############ for the mc
    #shift3Prong         = cms.double(0.022),
    ptDependence3Prong  = cms.double(0.001)
)

cmgDiTauCor = cms.EDFilter(
    "DiTauUpdatePOProducer",
    cfg  = cmgDiTauCorFactory.clone(),
    cuts = cms.PSet()
    )
