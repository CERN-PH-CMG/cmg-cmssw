import FWCore.ParameterSet.Config as cms
from CMGTools.Common.factories.cmgDiObject_cfi import diObjectFactory

muEleFactory = diObjectFactory.clone(
    leg1Collection = cms.InputTag('cmgMuonSel'),
    leg2Collection = cms.InputTag('cmgElectronSel'),
    metCollection = cms.InputTag("")
)

cmgMuEle = cms.EDFilter(
    "MuElePOProducer",
    cfg = muEleFactory.clone(),
    cuts = cms.PSet(),
    )
