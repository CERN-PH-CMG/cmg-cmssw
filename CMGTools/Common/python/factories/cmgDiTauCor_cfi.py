import FWCore.ParameterSet.Config as cms

cmgDiTauCorFactory = cms.PSet(
    # leg1 and leg2 are dummy collections here
    leg1Collection = cms.InputTag(''),
    leg2Collection = cms.InputTag(''),
    metCollection = cms.InputTag('recoilCorrectedMET'),
    diObjectCollection = cms.InputTag('cmgDiTauSel')
)

cmgDiTauCor = cms.EDFilter(
    "DiTauUpdatePOProducer",
    cfg = cmgDiTauCorFactory.clone(),
    cuts = cms.PSet()
    )
