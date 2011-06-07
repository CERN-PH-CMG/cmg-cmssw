import FWCore.ParameterSet.Config as cms
from CMGTools.Common.factories.cmgDiObject_cfi import diObjectFactory

muEFactory = diObjectFactory.clone(
    leg1Collection = cms.InputTag('cmgMuon'),
    leg2Collection = cms.InputTag('cmgElectron')
)

cmgMuE = cms.EDFilter(
    "MuEPOProducer",
    cfg = muEFactory.clone(),
    cuts = cms.PSet(),
    )
