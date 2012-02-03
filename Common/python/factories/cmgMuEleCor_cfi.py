import FWCore.ParameterSet.Config as cms

cmgMuEleCorFactory = cms.PSet(
    # leg1 and leg2 are dummy collections here
    leg1Collection = cms.InputTag(''),
    leg2Collection = cms.InputTag(''),
    metCollection = cms.InputTag('recoilCorrectedMET'),
    diObjectCollection = cms.InputTag('cmgMuEleSel')
)

cmgMuEleCor = cms.EDFilter(
    "MuEleUpdatePOProducer",
    cfg = cmgMuEleCorFactory.clone(),
    cuts = cms.PSet()
    )
