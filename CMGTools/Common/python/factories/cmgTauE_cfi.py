import FWCore.ParameterSet.Config as cms
from CMGTools.Common.factories.cmgDiObject_cfi import diObjectFactory

tauEFactory = diObjectFactory.clone(
       leg1Collection = cms.InputTag('cmgTau'),
       leg2Collection = cms.InputTag('cmgElectron')
)

cmgTauE = cms.EDFilter(
    "TauEPOProducer",
    cfg = tauEFactory.clone(),
    cuts = cms.PSet(),
    )
