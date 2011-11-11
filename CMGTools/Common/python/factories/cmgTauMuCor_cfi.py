import FWCore.ParameterSet.Config as cms

cmgTauMuCorFactory = cms.PSet(
    leg1Collection = cms.InputTag(''),
    leg2Collection = cms.InputTag(''),
    metCollection = cms.InputTag('recoilCorrectedMET'),
    diObjectCollection = cms.InputTag('cmgTauMuSel')
)

cmgTauMuCor = cms.EDFilter(
    "TauMuUpdatePOProducer",
    cfg = cmgTauMuCorFactory.clone(),
    cuts = cms.PSet()
    )
