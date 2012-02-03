import FWCore.ParameterSet.Config as cms

cmgTauEleCorFactory = cms.PSet(
    # leg1 and leg2 are dummy collections here
    leg1Collection = cms.InputTag(''),
    leg2Collection = cms.InputTag(''),
    metCollection = cms.InputTag('recoilCorrectedMET'),
    diObjectCollection = cms.InputTag('cmgTauEleSel')
)

cmgTauEleCor = cms.EDFilter(
    "TauEleUpdatePOProducer",
    cfg = cmgTauEleCorFactory.clone(),
    cuts = cms.PSet()
    )
